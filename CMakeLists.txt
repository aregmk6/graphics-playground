cmake_minimum_required(VERSION 4.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(graphics-programming)
add_executable(jump main.cpp)

target_link_libraries(jump glfw)
target_include_directories(jump PRIVATE include/)
target_compile_definitions(jump PRIVATE PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}")

add_subdirectory(src)
