#pragma once
#include <sdgl/sdgl.h>
#include "Paddle.h"
#include "Ball.h"

#include <imgui.h>

constexpr int PointsToWin = 7;
constexpr float MaxServeAngle = 25.f;

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

    SpriteBatch2D batch{};
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
        window()->getSize(&fw, &fh);
        ball->position({(float)fw/2.f, (float)fh/2.f});
        ball->direction((mathf::rand(MaxServeAngle) - MaxServeAngle / 2.f) + (mathf::chance(.5f) ? 0 : 180.f));
        ball->speed(250.f);

        player->position({
            4,
            static_cast<float>(fh) / 2.f - 64.f
        });
        computer->position({
            static_cast<float>(fw) - 20.f,
            static_cast<float>(fh) / 2.f - 64.f
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

        Point winSize;
        window()->getSize(&winSize.x, &winSize.y);

        // create paddles
        player = new Paddle(16, 128, 0, winSize.y, Color::White, Paddle::Player);
        computer = new Paddle(16, 128, 0, winSize.y, Color::White, Paddle::Computer);

        // init camera
        cam.setViewport({0, 0, winSize.x, winSize.y});
        cam.setOrigin({0, 0});

        // create ball
        ball = new Ball(0, 0, winSize.x, winSize.y, player, computer);

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

        reset(); // start the game
        return true;
    }

    void handleScore(Paddle::Id id)
    {
        // Increase points
        ++score[id];
        scoreText[id].setText(std::to_string(score[id]));

        if (score[id] >= PointsToWin) // player or computer won
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
        else                         // regular score
        {
            int fw, fh;
            window()->getSize(&fw, &fh);
            ball->position({(float)fw/2.f, (float)fh/2.f});
            ball->direction((mathf::rand(MaxServeAngle) - MaxServeAngle/2.f) + (mathf::chance(.5f) ? 0 : 180.f));
            pauseCountDown = 2.f;

            sndScore[id]->play();
        }
    }

    /// ball hit a paddle
    void handlePaddleHit(Paddle::Id id)
    {
        if (sndPaddleHit[id])
            sndPaddleHit[id]->play();
    }

    /// ball hit a wall
    void handleWallHit()
    {
        if (sndWallHit)
            sndWallHit->play();
    }

    void update() override
    {
        const auto delta = static_cast<float>(getDeltaTime());

        switch(state)
        {
            case State::Gameplay:
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
            } break;

            case State::Gameover:
            {
                if (input()->isDown(Key::Return) || input()->isDown(0, GamepadBtn::Start))
                {
                    reset();
                    state = State::Gameplay; // already set in reset, but here to state intention
                }
            } break;
        }
    }

    void render() override
    {
        window()->clear({20, 20, 20, 255});

        int w, h;
        window()->getSize(&w, &h);

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
        audio()->destroySound(sndPaddleHit[0]);
        audio()->destroySound(sndPaddleHit[1]);
        audio()->destroySound(sndWallHit);
        audio()->destroySound(sndScore[0]);
        audio()->destroySound(sndWin[0]);
        audio()->destroySound(sndScore[1]);
        audio()->destroySound(sndWin[1]);

        delete player;
        delete computer;
        delete ball;
        player = nullptr;
        computer = nullptr;
        ball = nullptr;
    }
};
