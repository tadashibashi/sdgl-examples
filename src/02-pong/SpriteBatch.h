#pragma once
#include <sdgl/graphics/SpriteBatch2D.h>

using namespace sdgl;

/// Extension of SpriteBatch2D with convenience functions
class SpriteBatch : public SpriteBatch2D {
public:

    void init() override
    {
        pixel.loadBytes({Color::White}, 1, 1, TextureFilter::Nearest);
        SpriteBatch2D::init();
    }

    void drawRectangle(Rectangle rect, Color tint = Color::White,
        Vector2 scale = {1.f, 1.f}, Vector2 anchor = {0, 0},
        float angle = 0, float depth = 0);

    /// Draw entire texture
    void drawTexture(const Texture2D &texture, Vector2 position,
                        Color tint = Color::White, Vector2 scale = {1.f, 1.f},
                        Vector2 anchor = {0, 0}, float angle = 0,
                        float depth = 0);

    /// Draw texture mapped to destination rectangle
    void drawTexture(const Texture2D &texture, Rectangle source,
                        Rectangle destination, Color tint = Color::White,
                        Vector2 anchor = {0, 0}, float angle = 0,
                        float depth = 0);
private:
    Texture2D pixel;
};
