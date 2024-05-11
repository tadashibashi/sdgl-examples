#include "Paddle.h"
#include "Ball.h"

const InputManager *Paddle::s_input;

static constexpr float PaddleSpeedMax = 4.f;

Paddle::Paddle(int width, int height, int yMin, int yMax, Color color, Id id):
    m_updateCallback(id == Player ? &Paddle::updatePlayer : &Paddle::updateComputer),
    m_position(), m_yMin(yMin), m_yMax(yMax), m_size(width, height),
    m_color(color), m_id(id), m_ball()
{
    SDGL_ASSERT(m_updateCallback, "update callback must be set");
}

void Paddle::update(float deltaTime)
{
    std::invoke(m_updateCallback, *this, deltaTime);

    // constrain position
    m_position.y = mathf::clamp(m_position.y,
        static_cast<float>(m_yMin), static_cast<float>(m_yMax) - static_cast<float>(m_size.y));
}

void Paddle::draw(SpriteBatch2D *batch)
{
    batch->drawRectangle({(int)m_position.x, (int)m_position.y, m_size.x, m_size.y}, m_color);
}

Rectangle Paddle::bounds() const
{
    return {(int)m_position.x, (int)m_position.y, m_size.x, m_size.y};
}

void Paddle::updateComputer(float delta)
{
    const auto paddleCenterY = m_position.y + (static_cast<float>(m_size.y) / 2.f);
    const auto ballCenterY = m_ball->position().y + BallWidth / 2.f;

    if (const auto diff = ballCenterY - (paddleCenterY - static_cast<float>(m_size.y) / 4.f);
        diff < 0)
    {
        m_position.y += std::round(std::max(-PaddleSpeedMax, diff));
    }
    else if (const auto diff2 = ballCenterY - (paddleCenterY + static_cast<float>(m_size.y) / 4.f);
        diff2 > 0)
    {
        m_position.y += std::round(std::min(PaddleSpeedMax, diff));
    }
}

void Paddle::updatePlayer(float deltaTime)
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
