#pragma once
#include "Grid.h"

#include <sdgl/sdglib.h>
#include <sdgl/math/Vector2.h>
#include <sdgl/math/random.h>

namespace sdgl {
    class SpriteBatch2D;
}

using namespace sdgl;

constexpr int GridWidth = 12;
constexpr int GridHeight = 12;
using SnakeGrid = Grid<ItemType::Enum, GridWidth, GridHeight>;

constexpr float StartingInterval = .75f;
constexpr auto StartingPoint = Point(5, 5);

class Snake {
public:
    explicit Snake(SnakeGrid *grid);

    /// Call this to set all variables back to a new game
    void reset()
    {
        m_interval = StartingInterval;
        m_body.clear();
        m_body.emplace_back(StartingPoint);
    }

    void update(float deltaTime);
    void render(SpriteBatch2D *spriteBatch);

private:
    void eraseFromGrid();

    /// number of seconds before moving one square
    float m_interval;
    vector<Point> m_body;
    SnakeGrid *m_grid;
};
