project(02-pong)

list(APPEND SOURCE
    Pong.h Pong.cpp
    Paddle.h Paddle.cpp
    SpriteBatch.h SpriteBatch.cpp
    Ball.h Ball.cpp
)

add_sdgl_executable(${PROJECT_NAME}
    CONTENT_ROOT assets
    CONTENT ""
    SOURCE ${SOURCE}
)