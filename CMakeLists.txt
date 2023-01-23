cmake_minimum_required(VERSION 3.24)
project(gaming)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Stb REQUIRED)

add_subdirectory(libs/glad)
add_subdirectory(libs/spdlog)

add_library(katlib engine/src/kat/common.cpp engine/src/kat/common.hpp game/src/main.cpp engine/src/kat/Window.cpp engine/src/kat/Window.hpp engine/src/kat/Renderer.cpp engine/src/kat/Renderer.hpp engine/src/kat/Mesh.cpp engine/src/kat/Mesh.hpp engine/src/kat/SpriteBatch.cpp engine/src/kat/SpriteBatch.hpp engine/src/kat/Shader.cpp engine/src/kat/Shader.hpp)
target_include_directories(katlib PUBLIC engine/src/ ${STB_INCLUDE_DIR})
target_link_libraries(katlib PUBLIC glfw glm::glm glad::glad spdlog::spdlog)


add_executable(katlib_test game/src/main.cpp)
target_include_directories(katlib_test PRIVATE game/src/)
target_link_libraries(katlib_test PRIVATE katlib)
