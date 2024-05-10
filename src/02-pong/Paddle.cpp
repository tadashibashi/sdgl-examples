#include "Paddle.h"
#include "Ball.h"

const InputManager *Paddle::s_input;

static const float PaddleSpeedMax = 4.f;

void Paddle::updateComputer(float delta)
{
    const auto paddleY = m_position.y + (m_size.y / 2.f);
    const auto ballY = m_ball->position().y + BallWidth / 2.f;

    if (const auto diff = ballY - (paddleY - m_size.y / 4.f); diff < 0)
    {
        m_position.y += std::round(std::max(-PaddleSpeedMax, diff));
    }
    else if (const auto diff2 = ballY - (paddleY + m_size.y / 4.f); diff2 > 0)
    {
        m_position.y += std::round(std::min(PaddleSpeedMax, diff));
    }
}
