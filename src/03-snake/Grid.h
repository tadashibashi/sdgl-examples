#pragma once
#include <sdgl/assert.h>
#include <sdgl/sdglib.h>
#include <sdgl/math/Vector2.h>

using namespace sdgl;

struct ItemType {
    enum Enum : ubyte {
        None,
        Snake,
        Fruit,
    };
};

/// A grid of things that takes up game world space
template <typename T, size_t Width, size_t Height>
    requires std::is_trivially_copy_assignable_v<T>
class Grid {
    static_assert(Width > 0 && Height > 0);
public:
    /// Create grid with specified dimensions
    /// @param tileWidth  width of one tile in pixels
    /// @param tileHeight width of one tile in pixels
    /// @param offsetX    x position to start drawing the grid
    /// @param offsetY    y position to start drawing the grid
    /// @param spaceX     x space in pixels between each column
    /// @param spaceY     y space in pixels between each row
    Grid(int tileWidth, int tileHeight, int offsetX, int offsetY, int spaceX, int spaceY) :
        m_grid(Width * Height, ItemType::None), m_offset(offsetX, offsetY),
        m_tileSize(tileWidth, tileHeight), m_spacing(spaceX, spaceY)
    {
        SDGL_ASSERT(tileWidth > 0 && tileHeight > 0);
    }

    /// Get item type at grid position
    [[nodiscard]]
    T get(int x, int y) const
    {
        static_assert(x >= 0 && x < Width && y >= 0 && y < Height);
        return m_grid[y * Width + x];
    }

    void set(int x, int y, T value)
    {
        static_assert(x >= 0 && x < Width && y >= 0 && y < Height);

        m_grid[y * Width + x] = value;
    }

    /// Set internal array data to 0
    void clear()
    {
        std::memset(m_grid.data(), 0, m_grid.size() * sizeof(T));
    }

    /// Get world position of a cell of the grid
    Point positionAt(int x, int y)
    {
        static_assert(x >= 0 && x < Width && y >= 0 && y < Height);
        const auto offsetX = m_offset.x * x;
        const auto offsetY = m_offset.y * y;

    }

    /// Dimensions of each individual tile in this grid
    [[nodiscard]]
    auto tileSize() const { return m_tileSize; }
private:
    /// Flattened grid in order by row
    array<T, Width * Height> m_grid;
    Point m_offset, m_tileSize, m_spacing;
};