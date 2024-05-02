#include <sdgl/App.h>
#include <sdgl/Camera2D.h>
#include <sdgl/logging.h>
#include <imgui.h>

#include "sdgl/graphics/SpriteBatch2D.h"

using namespace sdgl;
class MyApp final : public App {
public:
    MyApp() : App("Hello App", 640, 480)
    {

    }

private:
    SpriteBatch2D bat;
    Camera2D cam;
    Texture2D diceImage;
    Vector2 dicePos;

    bool init() override
    {
        bat.init();
        cam.setOrigin({});

        int w, h;
        window()->getSize(&w, &h);
        cam.setViewport({0, 0, w, h});
        diceImage.loadFile("assets/dice.png");
        dicePos = {w/2.f, h/2.f};
        return true;
    }

    void update() override
    {
        auto input = window()->input();
        Vector2 movement;

        if (input->isDown(Key::Right))
        {
            movement += Vector2(4.f, 0);
        }

        if (input->isDown(Key::Left))
        {
            movement -= Vector2(4.f, 0);
        }

        if (input->isDown(Key::Up))
        {
            movement -= Vector2(0, 4.f);
        }

        if (input->isDown(Key::Down))
        {
            movement += Vector2(0, 4.f);
        }

        if (auto axisX = input->getAxis(0, GamepadAxis::LeftX); std::abs(axisX) > .1f)
        {
            movement += Vector2(axisX * 4.f, 0);
        }

        if (auto axisY = input->getAxis(0, GamepadAxis::LeftY); std::abs(axisY) > .1f)
        {
            movement += Vector2(0, axisY * 4.f);
        }

        cam.setPosition(cam.getPosition() + movement);
    }

    void render() override
    {
        window()->clear({255, 255, 255, 255});

        bat.begin(cam.getMatrix());
        bat.drawTexture(diceImage, {0, 0, diceImage.size().x, diceImage.size().y}, dicePos, Color::White,
            {1.f, 1.f}, {diceImage.size().x/2.f, diceImage.size().y/2.f}, 0, 0);
        bat.end();
    }

    void shutdown() override
    {

    }
};

int main(int argc, char *argv[])
{
    MyApp().run(argc, argv);
    return 0;
}
