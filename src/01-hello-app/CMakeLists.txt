project(01-hello-app)

add_executable(${PROJECT_NAME} hello_app.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE sdgl)

# TODO: make an add_executable function that automatically sets this up
angles_copy_libs()
copy_gamepad_mappings()
