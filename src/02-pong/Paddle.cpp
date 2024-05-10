#include "Paddle.h"
#include "Ball.h"

const InputManager *Paddle::s_input;

void Paddle::updateComputer(float delta)
{
    const auto paddleY = m_position.y + (m_size.y / 2);
    const auto ballY = m_ball->position().y + BallWidth / 2;
    if (ballY < paddleY - m_size.y / 4)
    {
        m_position.y -= 4.f;
    }
    else if (ballY > paddleY + m_size.y / 4)
    {
        m_position.y += 4.f;
    }
}
