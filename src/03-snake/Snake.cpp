#include "Snake.h"

#include <sdgl/assert.h>
#include <sdgl/logging.h>


#include "Grid.h"


Snake::Snake(SnakeGrid *grid): m_interval(.75f), m_grid(grid)
{
    SDGL_ASSERT(grid, "Grid should not be null");
}

void Snake::update(float deltaTime)
{

}

void Snake::render(SpriteBatch2D *spriteBatch)
{

}



void Snake::eraseFromGrid()
{
    for (auto part : m_body)
    {
        m_grid->set(part.x, part.y, ItemType::None);
    }
}
