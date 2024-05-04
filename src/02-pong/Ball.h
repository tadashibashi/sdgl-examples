#pragma once
#include "SpriteBatch.h"
#include "Paddle.h"

using namespace sdgl;

const float BallWidth = 16.f;

class Ball {
public:
    Ball(int left, int top, int right, int bottom);
    void update(float deltaTime);
    void draw(SpriteBatch *batch);

    void position(Vector2 value) { m_position = value; }

    void direction(float degrees) { m_direction = degrees; }

    void speed(float value) { m_speed = value; }

    Delegate<Paddle::PaddleId> onScore;

    [[nodiscard]]
    Vector2 position() const { return m_position; }
    [[nodiscard]]
    float direction() const { return m_direction; }
    [[nodiscard]]
    float speed() const { return m_speed; }
private:
    Vector2 m_position; // current position in pixels
    int m_speed;        // speed in pixels per second
    float m_direction;  // angle in degrees, where 0 is up

    int m_left, m_top, m_right, m_bottom;
};