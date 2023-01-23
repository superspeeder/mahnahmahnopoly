#include <kat/common.hpp>
#include <kat/Window.hpp>
#include <memory>
#include <spdlog/spdlog.h>

int main() {
    glfwInit();

    std::shared_ptr<kat::Window> win = std::make_shared<kat::Window>(glm::ivec2(1600, 900), "Mahnahmahnopoly");
    while (win->isOpen()) {
        win->poll();

        if (win->isKeyJustPressed(GLFW_KEY_F11)) {
            win->toggleFullscreen();
            spdlog::info("Fullscreen Toggled");
        }

        win->swap();
    }


    glfwTerminate();
}