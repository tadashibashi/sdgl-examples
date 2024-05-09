project(02-pong)

list(APPEND SOURCE
    Pong.h Pong.cpp
    Paddle.h Paddle.cpp
    SpriteBatch.h SpriteBatch.cpp
    Ball.h Ball.cpp
)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

add_sdgl_executable(${PROJECT_NAME}
    CONTENT_ROOT assets
    CONTENT
        # fonts
        pong.fnt pong_0.png

        # sounds
        paddle_hit_01.wav paddle_hit_02.wav wall_hit.wav
        win.wav lose.wav
        player_score.wav computer_score.wav
    SOURCE ${SOURCE}
)
