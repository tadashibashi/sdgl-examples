#include "Ball.h"
#include <imgui.h>

Ball::Ball(int left, int top, int right, int bottom, Paddle *player, Paddle *computer) :
    m_left(left), m_top(top), m_right(right), m_bottom(bottom),
    m_position(), m_speed(300.f), m_player(player), m_computer(computer)
{
}

static constexpr float MaxDeltaTime = 1.f / 60.f * 2.f;
static constexpr int CollisionGrain = 4;

static Rectangle intersection(const Rectangle &a, const Rectangle &b)
{
    auto x = std::max(a.left(), b.left());
    auto y = std::max(a.top(), b.top());
    auto right = std::min(a.right(), b.right());
    auto bottom = std::min(a.bottom(), b.bottom());

    return {x, y, std::max(right - x, 0), std::max(bottom - y, 0)};
}

void Ball::update(float deltaTime)
{
    // limit motion from deltaTime in case game freezes
    if (deltaTime > MaxDeltaTime)
    {
        deltaTime = MaxDeltaTime;
    }

    const auto currentAngle = mathf::toRadians(m_direction);
    const auto ballYCenter = m_position.y + BallWidth / 2;

    // check paddles for collision
    // cast collision check projection with grain
    // TODO: optimize by checking which side ball is on

    int hitPaddle = -1;
    for (int i = 1; i < CollisionGrain; ++i)
    {
        const auto trajectory = (Point)mathf::trajectory(currentAngle, m_speed * deltaTime / CollisionGrain * i);
        const auto checkPos = trajectory + (Point)m_position;
        const auto checkRect = Rectangle{checkPos.x, checkPos.y, (int)BallWidth, (int)BallWidth};

        // Check collision with player paddle
        const auto playerBounds = m_player->bounds();
        if (mathf::intersects(playerBounds, checkRect))
        {
            // simulate rounded paddle shape: calculate offset the paddle angle by which the ball bounces off
            constexpr float maxAngleOffset = 20.f;
            float angleOffsetPercent = (ballYCenter - ((float)playerBounds.y + (float)playerBounds.h / 2.f)) / (playerBounds.h / 2.f);
            angleOffsetPercent = mathf::clamp(angleOffsetPercent, -1.f, 1.f);

            const auto intersect = intersection(playerBounds, checkRect);
            if (intersect.w > intersect.h)
            {
                if (intersect.y == playerBounds.y) // collision from top
                {
                    m_position.y -= (m_position.y + BallWidth - playerBounds.top());
                    auto oldDir = m_direction;
                    if (trajectory.y > 0)
                        m_direction += (-m_direction + 180.f) * 2.f;
                }
                else                              // collision from bottom
                {
                    m_position.y -= (m_position.y - playerBounds.bottom());
                    auto oldDir = m_direction;

                    if (trajectory.y < 0)
                        m_direction += (-m_direction + 180.f) * 2.f;
                }
            }
            else
            {
                auto newDir = mathf::wrap((90.f + maxAngleOffset * angleOffsetPercent - m_direction + 180.f) * 2.f + m_direction, 0, 360);

                // make sure direction bounces back toward center
                if (newDir > 85 && newDir < 180)
                    newDir = 85;
                else if (newDir < 275 && newDir >= 180)
                    newDir = 275;
                m_direction = newDir;
                m_position.x -= (m_position.x - playerBounds.right());
            }

            hitPaddle = 0;
            break;
        }

        // Check collision with computer paddle
        const auto compBounds = m_computer->bounds();
        if (mathf::intersects(compBounds, checkRect))
        {
            // simulate rounded paddle shape: calculate offset of paddle angle by which the ball bounces off
            constexpr float maxAngleOffset = 20.f;
            float angleOffsetPercent = (ballYCenter - ((float)compBounds.y + (float)compBounds.h / 2.f)) / (compBounds.h / 2.f);
            angleOffsetPercent = mathf::clamp(angleOffsetPercent, -1.f, 1.f);

            const auto intersect = intersection(compBounds, checkRect);

            if (intersect.w > intersect.h) // vertical collision
            {
                if (intersect.y == compBounds.y) // from top
                {
                    m_position.y -= (m_position.y + BallWidth - compBounds.top());
                    auto oldDir = m_direction;
                    if (trajectory.y > 0)
                        m_direction += (-m_direction + 180.f) * 2.f;
                }
                else                             // from bottom
                {
                    m_position.y -= (m_position.y - compBounds.bottom());
                    auto oldDir = m_direction;

                    if (trajectory.y < 0)
                        m_direction += (-m_direction + 180.f) * 2.f;
                }
            }
            else                            // horizontal collision
            {

                auto newDir = mathf::wrap((90.f - maxAngleOffset * angleOffsetPercent - m_direction + 180.f) * 2.f + m_direction, 0, 360.f);
                m_direction = mathf::clamp(newDir, 95, 265.f); // make sure direction bounces back toward center

                m_position.x -= (m_position.x + BallWidth - compBounds.left());
            }

            hitPaddle = 1;
            break;
        }
    }

    if (hitPaddle != -1)
    {
        m_speed *= 1.02f;
        if (m_speed > BallMaxSpeed)
        {
            m_speed = BallMaxSpeed;
        }
        onPaddleHit((Paddle::PaddleId)hitPaddle);
    }

    // apply motion
    auto motion = mathf::trajectory(mathf::toRadians(m_direction), m_speed * deltaTime);
    if (abs(motion.x) < 2.f) // min x speed
        motion.x = motion.x < 0 ? -2.f : 2.f;

    m_position += motion;

    // bounce off top/bottom walls
    if (m_position.y < m_top)
    {
        m_direction += (180.f - m_direction + 180.f) * 2.f;
        m_position.y -= (m_position.y - m_top) * 2.f;
        onWallHit();
    }
    else if (m_position.y > m_bottom - BallWidth)
    {
        m_direction += (-m_direction + 180.f) * 2.f;
        m_position.y -= (m_position.y - (m_bottom - BallWidth)) * 2.f;
        onWallHit();
    }

    // score on left / right walls
    if (m_position.x < m_left)
    {
        m_direction += (90.f - m_direction + 180.f) * 2.f;
        m_position.x -= (m_position.x - m_left) * 2.f;
        onScore(Paddle::PaddleId::Computer);
    }
    else if (m_position.x > m_right - BallWidth)
    {
        m_direction += (270.f - m_direction + 180.f) * 2.f;
        m_position.x -= (m_position.x - (m_right - BallWidth)) * 2.f;
        onScore(Paddle::PaddleId::Player);
    }

    // constrain values
    m_position.x = mathf::clamp(m_position.x, m_left, m_right - BallWidth);
    m_position.y = mathf::clamp(m_position.y, m_top, m_bottom - BallWidth);
    m_direction = mathf::wrap(m_direction, 0, 360);
}

void Ball::draw(SpriteBatch *batch)
{
    batch->drawRectangle(
        {(int)m_position.x, (int)m_position.y, (int)BallWidth, (int)BallWidth}
    );
}