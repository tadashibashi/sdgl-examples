project(03-snake)

set(SOURCE
        SnakeApp.cpp SnakeApp.h Grid.h Snake.h Snake.cpp
)

add_sdgl_executable(${PROJECT_NAME}
    CONTENT_ROOT assets
    CONTENT ""
    SOURCE ${SOURCE}
)
