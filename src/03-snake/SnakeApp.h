#pragma once
#include <sdgl/App.h>
#include <sdgl/graphics/SpriteBatch2D.h>

using namespace sdgl;

class SnakeApp : public App {
public:
    SnakeApp() : App("Snake", 480, 480) { }
    ~SnakeApp() = default;
private:
    SpriteBatch2D batch;

protected:
    bool init() override
    {
        batch.init();

        return true;
    }

    void update() override
    {

    }

    void render() override
    {

    }

    void shutdown() override
    {

    }

private:
};