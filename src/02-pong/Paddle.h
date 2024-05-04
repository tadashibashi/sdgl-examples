#pragma once
#include <sdgl/sdgl.h>
#include "SpriteBatch.h"

using namespace sdgl;

constexpr auto PaddleBaseSpeed = 600.f;

class Paddle {
public:

    enum PaddleId {
        Player = 0,
        Computer,
    };

    Paddle(int width, int height, Color color, PaddleId id) :
        m_size(width, height), m_color(color), m_id(id),
        m_updateCallback(id == Player ? &Paddle::updatePlayer : &Paddle::updateComputer)
    {
        SDGL_ASSERT(m_updateCallback, "update callback must be set");
    }

    static void provideInput(const InputManager *input) { s_input = input; }

    void update(float deltaTime)
    {
        std::invoke(m_updateCallback, *this, deltaTime);
    }

    void draw(SpriteBatch *batch)
    {
        batch->drawRectangle({(int)m_position.x, (int)m_position.y, m_size.x, m_size.y}, m_color);
    }

    void position(Vector2 value) { m_position = value; }
    [[nodiscard]]
    Vector2 position() const { return m_position; }

    [[nodiscard]]
    PaddleId id() const { return m_id; }

private:
    void updateComputer(float deltaTime)
    {

    }

    void updatePlayer(float deltaTime)
    {
        if (s_input->isDown(Key::Down))
        {
            m_position.y += PaddleBaseSpeed * deltaTime;
        }

        if (s_input->isDown(Key::Up))
        {
            m_position.y -= PaddleBaseSpeed * deltaTime;
        }
    }

    void (Paddle::*m_updateCallback)(float deltaTime);
    Vector2 m_position;
    Point m_size;
    Color m_color;
    PaddleId m_id;
    static const InputManager *s_input;
};