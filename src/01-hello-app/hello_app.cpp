#include <sdgl/App.h>
#include <sdgl/Camera2D.h>
#include <sdgl/logging.h>

#include <sdgl/graphics/SpriteBatch2D.h>
using namespace sdgl;

struct Dice {
    Vector2 position;
    float rotation{0};
    float scale{1.f};
    Texture2D image;
};

class HelloApp final : public App {
public:
    HelloApp() : App("Hello App", 640, 480)
    {

    }

private:
    SpriteBatch2D bat{};
    Camera2D cam{};
    Dice dice{};

    bool init() override
    {
        bat.init();
        cam.setOrigin({});

        int w, h;
        window()->getSize(&w, &h);
        cam.setViewport({0, 0, w, h});
        auto imageLoadResult = dice.image.loadFile("assets/dice.png");
        dice.position = {w/2.f, h/2.f};

        return true;
    }

    void update() override
    {
        auto input = window()->input();
        Vector2 movement;
        float rotation = 0;
        float scale = 1.f;

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

        if (auto axisTriggerLeft = input->getAxis(0, GamepadAxis::TriggerLeft))
        {
            rotation -= axisTriggerLeft * 4.f;
        }

        if (auto axisTriggerRight = input->getAxis(0, GamepadAxis::TriggerRight))
        {
            rotation += axisTriggerRight * 4.f;
        }

        if (input->isDown(0, GamepadBtn::A) || input->isDown(Key::Z))
        {
            scale += .1f;
        }

        if (input->isDown(0, GamepadBtn::B) || input->isDown(Key::X))
        {
            scale -= .1f;
        }

        dice.position += movement;
        dice.rotation += rotation;
        dice.scale *= scale;
    }

    void render() override
    {
        window()->clear({255, 255, 255, 255});

        bat.begin(cam.getMatrix());
        bat.drawTexture(dice.image, dice.position, Color::White,
            {dice.scale, dice.scale}, {dice.image.size().x/2.f, dice.image.size().y/2.f}, mathf::toRadians(dice.rotation), 0);
        bat.end();
    }

    void shutdown() override
    {
        dice.image.unload();
    }
};

int main(int argc, char *argv[])
{
    HelloApp().run(argc, argv);
    return 0;
}
