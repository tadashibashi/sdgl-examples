#pragma once
#include <sdgl/sdgl.h>

#include "SpriteBatch.h"
#include "Paddle.h"
#include "Ball.h"

#include <imgui.h>

const int PointsToWin = 7;

using namespace sdgl;

class Pong final : public sdgl::App {
public:
    Pong() : App("Pong", 640, 480) { }
    ~Pong() override = default;

protected:
    enum class State {
        Gameplay,
        Gameover,
    };

    SpriteBatch batch{};
    Camera2D cam{};

    Paddle *player{}, *computer{};
    Ball *ball{};
    SoundInstance *sndPaddleHit[2] {nullptr};
    SoundInstance *sndWallHit{};
    SoundInstance *sndWin[2] {nullptr};
    SoundInstance *sndScore[2] {nullptr};

    BitmapFont font;
    State state{State::Gameplay};

    int score[2] = {0};
    FontText scoreText[3];
    FontText centeredText;

    float pauseCountDown = 0;
    float textTimerCountDown = 0;

    const InputManager *input() const { return window()->input(); }

    void reset()
    {
        score[0] = 0;
        score[1] = 0;

        int fw, fh;
        window()->getFrameBufferSize(&fw, &fh);
        ball->position({(float)fw/2.f, (float)fh/2.f});
        ball->direction((mathf::rand(90.f) - 45.f) + (mathf::chance(.5f) ? 0 : 180.f));
        ball->speed(400.f);

        player->position({
            8,
            static_cast<float>(fh) / 2.f - 128.f
        });
        computer->position({
            static_cast<float>(fw) - 40.f,
            static_cast<float>(fh) / 2.f - 128.f
        });

        scoreText[0].setText(std::to_string(score[0]));
        scoreText[1].setText(std::to_string(score[1]));
        centeredText.setText("Ready?");

        pauseCountDown = 2.f;
        state = State::Gameplay;
    }

    bool init() override
    {
        batch.init();
        Paddle::provideInput(input());

        // load assets
        font.loadBMFont("assets/pong.fnt");
        SDGL_ASSERT(font.isLoaded());
        SDGL_LOG("Is font loaded?: {}", font.isLoaded());

        int fw, fh;
        window()->getFrameBufferSize(&fw, &fh);

        // create paddles
        player = new Paddle(32, 256, 0, fh, Color::White, Paddle::Player);
        computer = new Paddle(32, 256, 0, fh, Color::White, Paddle::Computer);

        // init camera
        cam.setViewport({0, 0, fw, fh});
        cam.setOrigin({0, 0});

        // create ball
        ball = new Ball(0, 0, fw, fh, player, computer);

        ball->onScore += Callback(this, &Pong::handleScore);
        ball->onPaddleHit += Callback(this, &Pong::handlePaddleHit);
        ball->onWallHit += Callback(this, &Pong::handleWallHit);

        // computer needs ball reference to track motion
        computer->init(ball);

        // set text font
        scoreText[0].font(&font);
        scoreText[1].font(&font);
        scoreText[2].font(&font); // TODO: may be convenient to create a default font embedded in sdgl?
        centeredText.font(&font);

        sndPaddleHit[0]  = audio()->createSound("assets/paddle_hit_01.wav");
        sndPaddleHit[1]  = audio()->createSound("assets/paddle_hit_02.wav");
        sndWallHit       = audio()->createSound("assets/wall_hit.wav");
        sndScore[0]      = audio()->createSound("assets/player_score.wav");
        sndWin[0]        = audio()->createSound("assets/win.wav");
        sndScore[1]      = audio()->createSound("assets/computer_score.wav");
        sndWin[1]        = audio()->createSound("assets/lose.wav");

        reset();
        return true;
    }

    void handleScore(Paddle::PaddleId id)
    {
        ++score[id];
        scoreText[id].setText(std::to_string(score[id]));

        if (score[id] >= PointsToWin)
        {
            const auto playerWon = score[0] > score[1];

            scoreText[2].setText(playerWon ? "Player wins" : "Computer wins");
            centeredText.setText(window()->input()->isGamepadConnected(0) ?
                "Press start to play again" : "Press return to play again");
            state = State::Gameover;

            if (playerWon)
                sndWin[0]->play();
            else
                sndWin[1]->play();
        }
        else
        {
            int fw, fh;
            window()->getFrameBufferSize(&fw, &fh);
            ball->position({(float)fw/2.f, (float)fh/2.f});
            ball->direction((mathf::rand(90.f) - 45.f) + (mathf::chance(.5f) ? 0 : 180.f));
            pauseCountDown = 2.f;

            sndScore[id]->play();
        }
    }

    void handlePaddleHit(Paddle::PaddleId id)
    {
        if (sndPaddleHit[id])
            sndPaddleHit[id]->play();
    }

    void handleWallHit()
    {
        if (sndWallHit)
            sndWallHit->play();
    }

    void update() override
    {
        const auto delta = static_cast<float>(getDeltaTime());

        if (state == State::Gameplay)
        {
            if (pauseCountDown > 0)
            {
                pauseCountDown -= delta;
            }
            else
            {
                ball->update(delta);
            }

            if (textTimerCountDown > 0)
                textTimerCountDown -= delta;

            computer->update(delta);
            player->update(delta);
        }
        else if (state == State::Gameover)
        {
            if (input()->isDown(Key::Return) || input()->isDown(0, GamepadBtn::Start))
            {
                reset();
                state = State::Gameplay; // already set in reset, but here to state intention
            }
        }
    }

    void render() override
    {
        window()->clear({20, 20, 20, 255});

        int w, h;
        window()->getFrameBufferSize(&w, &h);

        batch.begin(cam.getMatrix());


            player->draw(&batch);
            computer->draw(&batch);
            ball->draw(&batch);

            batch.drawText(scoreText[0], {128.f, 16.f});
            batch.drawText(scoreText[1], {(float)w - scoreText[1].currentSize().x - 128.f, 16.f});

            if (state == State::Gameover)
            {
                const auto textSize = scoreText[2].currentSize();
                batch.drawText(scoreText[2], {(float)w/2.f - textSize.x/2.f, 16.f});

                const auto centeredTextSize = centeredText.currentSize();
                const auto centeredTextPos =  Vector2{(float)w/2.f - (float)centeredTextSize.x/2.f, (float)h/2.f - (float)centeredTextSize.y/2.f};
                batch.drawRectangle(Rectangle{(int)centeredTextPos.x, (int)centeredTextPos.y, (int)centeredTextSize.x, (int)centeredTextSize.y}, {20, 20, 20, 255}, {1.f, 1.f}, {0, 0}, 0, -1.f);
                batch.drawText(centeredText, centeredTextPos, Color::White, -2.f);
            }
            else if (state == State::Gameplay)
            {
                if (pauseCountDown > 0 || textTimerCountDown > 0)
                {
                    const auto centeredTextSize = centeredText.currentSize();
                    const auto centeredTextPos =  Vector2{(float)w/2.f - (float)centeredTextSize.x/2.f, (float)h/2.f - (float)centeredTextSize.y/2.f};
                    batch.drawRectangle(Rectangle{(int)centeredTextPos.x, (int)centeredTextPos.y, (int)centeredTextSize.x, (int)centeredTextSize.y}, {20, 20, 20, 255}, {1.f, 1.f}, {0, 0}, 0, -1.f);
                    batch.drawText(centeredText, centeredTextPos, Color::White, -2.f);
                }
            }
        batch.end();
    }

    void shutdown() override
    {
        delete player;
        delete computer;
        delete ball;
        player = nullptr;
        computer = nullptr;
        ball = nullptr;
    }
};
