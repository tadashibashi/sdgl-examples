project(Pong)

list(APPEND SOURCE
    Pong.h Pong.cpp
    Paddle.h Paddle.cpp
    Ball.h Ball.cpp
)

add_sdgl_executable(${PROJECT_NAME}
    CONTENT_ROOT assets
    CONTENT
        # fonts
        pong.fnt pong_0.png

        # sounds
        paddle_hit_01.wav paddle_hit_02.wav wall_hit.wav
        win.wav lose.wav
        player_score.wav computer_score.wav
    ICON_FILE
        icon.png
    SOURCE ${SOURCE}
)
