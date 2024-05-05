#pragma once
#include <sdgl/sdgl.h>

#include "SpriteBatch.h"

#include "Paddle.h"
#include "Ball.h"

#include <imgui.h>

using namespace sdgl;

class Pong final : public sdgl::App {
public:
    Pong() : App("Pong", 640, 480) { }
    ~Pong() override = default;

protected:
    SpriteBatch batch{};
    Paddle *player{}, *computer{};
    Ball *ball{};
    BitmapFont font;
    Camera2D cam{};

    int score[2] = {0};
    FontText scoreText[2];

    void reset()
    {
        score[0] = 1000;
        score[1] = 1234;

        int fw, fh;
        window()->getFrameBufferSize(&fw, &fh);
        ball->position({(float)fw/2.f, (float)fh/2.f});
        ball->direction(mathf::rand(360.f));
        ball->speed(400.f);

        player->position({
            8,
            static_cast<float>(fh) / 2.f - 128.f
        });
        computer->position({
            static_cast<float>(fw) - 40.f,
            static_cast<float>(fh) / 2.f - 128.f
        });

        scoreText[0].font(&font);
        scoreText[1].font(&font);
        scoreText[0].setText(std::to_string(score[0]));
        scoreText[1].setText(std::to_string(score[1]));
    }

    void handleScore(Paddle::PaddleId id)
    {
        ++score[id];
        scoreText[id].setText(std::to_string(score[id]));
    }

    bool init() override
    {
        batch.init();
        Paddle::provideInput(window()->input());

        // load assets
        SDGL_ASSERT(font.loadBMFont("assets/pong.fnt"));

        // create paddles
        player = new Paddle(32, 256, Color::White, Paddle::Player);
        computer = new Paddle(32, 256, Color::White, Paddle::Computer);

        int fw, fh;
        window()->getFrameBufferSize(&fw, &fh);

        // init camera
        cam.setViewport({0, 0, fw, fh});
        cam.setOrigin({0, 0});

        // create ball
        ball = new Ball(0, 0, fw, fh);
        ball->position({(float)fw/2.f, (float)fh/2.f});
        ball->direction(mathf::rand(360.f));
        ball->speed(1600.f);

        ball->onScore += Callback(this, &Pong::handleScore);

        reset();
        return true;
    }

    void update() override
    {
        const auto delta = static_cast<float>(getDeltaTime());
        player->update(delta);
        computer->update(delta);
        ball->update(delta);
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

            batch.drawText(scoreText[0], {16.f, 16.f});
            batch.drawText(scoreText[1], {(float)w - scoreText[1].currentSize().x - 16.f, 16.f});

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
