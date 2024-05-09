#pragma once
#include <sdgl/sdgl.h>
#include "SpriteBatch.h"

using namespace sdgl;

constexpr auto PaddleBaseSpeed = 600.f;

class Paddle {
public:

    enum PaddleId : int {
        Player = 0,
        Computer,
    };

    Paddle(int width, int height, int yMin, int yMax, Color color, PaddleId id) :
        m_size(width, height), m_color(color), m_id(id), m_yMin(yMin), m_yMax(yMax),
        m_updateCallback(id == Player ? &Paddle::updatePlayer : &Paddle::updateComputer)
    {
        SDGL_ASSERT(m_updateCallback, "update callback must be set");
    }

    void init(const class Ball *ball)
    {
        m_ball = ball;
    }

    static void provideInput(const InputManager *input) { s_input = input; }

    void update(float deltaTime)
    {
        std::invoke(m_updateCallback, *this, deltaTime);

        // constrain position
        m_position.y = mathf::clampi(m_position.y, m_yMin, m_yMax - m_size.y);
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

    Rectangle bounds() const { return {(int)m_position.x, (int)m_position.y, m_size.x, m_size.y}; }
private:
    void updateComputer(float deltaTime);

    void updatePlayer(float deltaTime)
    {
        float yMotion = 0;
        if (s_input->isDown(Key::Down) || s_input->isDown(0, GamepadBtn::Down))
        {
            yMotion += PaddleBaseSpeed * deltaTime;
        }

        if (s_input->isDown(Key::Up) || s_input->isDown(0, GamepadBtn::Up))
        {
            yMotion -= PaddleBaseSpeed * deltaTime;
        }

        if (yMotion == 0)
        {
            if (const auto yAxis = s_input->getAxis(0, GamepadAxis::LeftY); abs(yAxis) > .1f)
            {
                yMotion += PaddleBaseSpeed * deltaTime * yAxis;
            }
        }

        m_position.y += yMotion;
    }

    void (Paddle::*m_updateCallback)(float deltaTime);
    Vector2 m_position;
    int m_yMin, m_yMax;
    Point m_size;
    Color m_color;
    PaddleId m_id;
    const class Ball *m_ball;
    static const InputManager *s_input;
};
