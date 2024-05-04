#pragma once
#include <sdgl/sdgl.h>
#include "SpriteBatch.h"

#include "Paddle.h"
#include "Ball.h"

using namespace sdgl;

class Pong final : public sdgl::App {
public:
    Pong() : App("Pong", 640, 480) { }
    ~Pong() override = default;

protected:
    SpriteBatch batch{};
    Paddle *player{}, *computer{};
    Ball *ball{};
    Camera2D cam{};

    bool init() override
    {
        batch.init();
        Paddle::provideInput(window()->input());
        player = new Paddle(32, 256, Color::White, Paddle::Player);
        computer = new Paddle(32, 256, Color::White, Paddle::Computer);

        int fw, fh;
        window()->getFrameBufferSize(&fw, &fh);
        cam.setViewport({0, 0, fw, fh});
        cam.setOrigin({0, 0});
        ball = new Ball(0, 0, fw, fh);
        ball->position({(float)fw/2.f, (float)fh/2.f});
        ball->direction(mathf::rand(360.f));

        ball->onScore += [](Paddle::PaddleId id) {
            DEBUG_LOG("Player {} scored!", (int)id);
        };

        player->position({
            8,
            static_cast<float>(fh) / 2.f - 128.f
        });
        computer->position({
            static_cast<float>(fw) - 40.f,
            static_cast<float>(fh) / 2.f - 128.f
        });

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

        batch.begin(cam.getMatrix());
            player->draw(&batch);
            computer->draw(&batch);
            ball->draw(&batch);
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
