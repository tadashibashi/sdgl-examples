#pragma once
#include <sdgl/graphics.h>
#include "Paddle.h"

using namespace sdgl;

constexpr float BallWidth = 8.f;
constexpr float BallMaxSpeed = 1200.f;

class Ball {
public:
    Ball(int left, int top, int right, int bottom, Paddle *player, Paddle *computer);
    void update(float deltaTime);
    void draw(SpriteBatch2D *batch);

    void position(Vector2 value) { m_position = value; }

    void direction(float degrees) { m_direction = degrees; }

    void speed(float value) { m_speed = static_cast<int>(value); }

    Delegate<Paddle::Id> onScore;
    Delegate<Paddle::Id> onPaddleHit;
    Delegate<> onWallHit;

    [[nodiscard]]
    Vector2 position() const { return m_position; }

    [[nodiscard]]
    float direction() const { return m_direction; }

    [[nodiscard]]
    float speed() const { return static_cast<float>(m_speed); }
private:
    Vector2 m_position; ///< current position in pixels
    int m_speed;        ///< speed in pixels per second
    float m_direction;  ///< angle in degrees, where 0 is up

    /// gameplay field boundaries
    int m_left, m_top, m_right, m_bottom;

    /// reference to paddles for collision checks
    Paddle *m_player, *m_computer;
};