#include "Ball.h"
#include <imgui.h>

Ball::Ball(int left, int top, int right, int bottom) :
    m_left(left), m_top(top), m_right(right), m_bottom(bottom),
    m_position(), m_speed(300.f)
{
}

void Ball::update(float deltaTime)
{
    const auto motion = mathf::trajectory(mathf::toRadians(m_direction), m_speed * deltaTime);

    if (ImGui::Begin("Ball Info"))
    {
        ImGui::Text("%s", format("Angle: {}, Movement: {}", m_direction, motion).c_str());
    }
    ImGui::End();

    m_position += motion;

    // simple algorithm, but potentially error prone with pass through collisions if speed > width

    // reflect top/bottom
    if (m_position.y <= m_top)
    {
        m_direction += (180.f - m_direction + 180.f) * 2.f;
    }
    else if (m_position.y >= m_bottom - BallWidth)
    {
        m_direction += (-m_direction + 180.f) * 2.f;
    }

    // left / right : TODO: remove bounce behavior
    if (m_position.x <= m_left)
    {
        m_direction += (90.f - m_direction + 180.f) * 2.f;
        onScore(Paddle::PaddleId::Computer);
    }
    else if (m_position.x >= m_right - BallWidth)
    {
        m_direction += (270.f - m_direction + 180.f) * 2.f;
        onScore(Paddle::PaddleId::Player);
    }

    m_direction = mathf::wrap(m_direction, 0, 360);
}

void Ball::draw(SpriteBatch *batch)
{
    batch->drawRectangle(
        {(int)m_position.x, (int)m_position.y, (int)BallWidth, (int)BallWidth}
    );
}