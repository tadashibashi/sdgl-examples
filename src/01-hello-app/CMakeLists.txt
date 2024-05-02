project(01-hello-app)

add_sdgl_executable(01-hello-app
        CONTENT_ROOT assets
        CONTENT dice.png
        SOURCE hello_app.cpp
)
