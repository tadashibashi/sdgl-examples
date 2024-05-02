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

    bool init() override
    {
        bat.init();
        cam.setOrigin({});
        return true;
    }

    void update() override
    {
        ImGui::ShowDemoWindow();

        auto &input = *window()->input();

        if (ImGui::Begin("Debug"))
        {
            ImGui::Text("Delta time: %f", getDeltaTime());
            ImGui::Text("App time: %f", getTime());

            if (input.isDown(Key::C))
            {
                ImGui::Text("C is down!");
            }

            if (input.isDown(MouseBtn::Right))
            {
                ImGui::Text("Mouse right is down");
            }

            if (input.isDown(0, GamepadBtn::StickLeft))
            {
                ImGui::Text("Pressed left stick!");
            }

            if (input.isDown(0, GamepadBtn::A))
            {
                ImGui::Text("Pressing X");
            }

            const auto leftTrigAxis = input.getAxis(0, GamepadAxis::TriggerLeft);
            ImGui::Text("Left trigger: %f", leftTrigAxis);

            const auto leftX = input.getAxis(0, GamepadAxis::LeftX);

            ImGui::Text("Left axis X: %f", leftX);

            float mouseX, mouseY;
            input.getMousePosition(&mouseX, &mouseY);
            ImGui::Text("Mouse position: %f, %f", mouseX, mouseY);


        }
        ImGui::End();

        if (input.isDoubleClicked(MouseBtn::Left))
        {
            DEBUG_LOG("Mouse left button was double-clicked!");
        }


    }

    void render() override
    {
        window()->clear({255, 255, 255, 255});
        bat.begin(cam.getMatrix());

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
