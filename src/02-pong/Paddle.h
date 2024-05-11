#pragma once
#include <sdgl/sdgl.h>

using namespace sdgl;

constexpr auto PaddleBaseSpeed = 600.f;

class Paddle {
public:

    enum Id : int {
        Player = 0,
        Computer,
    };

    Paddle(int width, int height, int yMin, int yMax, Color color, Id id);

    void init(const class Ball *ball)
    {
        m_ball = ball;
    }

    static void provideInput(const InputManager *input) { s_input = input; }

    void update(float deltaTime);

    void draw(SpriteBatch2D *batch);

    void position(Vector2 value) { m_position = value; }
    [[nodiscard]]
    Vector2 position() const { return m_position; }

    [[nodiscard]]
    Id id() const { return m_id; }

    [[nodiscard]]
    Rectangle bounds() const;

private:
    void updateComputer(float deltaTime);
    void updatePlayer(float deltaTime);

    void (Paddle::*m_updateCallback)(float deltaTime);
    Vector2 m_position;
    int m_yMin, m_yMax;
    Point m_size;
    Color m_color;
    Id m_id;
    const class Ball *m_ball;
    static const InputManager *s_input;
};
