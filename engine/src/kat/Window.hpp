#pragma once

#include "kat/common.hpp"

#include <unordered_set>

namespace kat {

    struct MonitorInfo {
        std::string monitorName;
        glm::ivec2 res;
    };

    glm::ivec2 getMonitorSize(GLFWmonitor* monitor);

    GLFWmonitor* getPrimaryMonitor();
    int getMonitorCount();
    std::vector<MonitorInfo> getMonitorInfo();
    MonitorInfo getMonitorInfo(GLFWmonitor* mon);



    class Window {
    public:

        Window(const glm::ivec2& size, const std::string& title);
        ~Window();


        bool isOpen();
        void poll();
        void swap();
        void close();
        void setFullscreen();
        void setFullscreen(GLFWmonitor* monitor);
        void setFullscreen(int i);

        void setWindowed();
        void setWindowed(const glm::ivec2& size);
        void setWindowed(const glm::ivec2& size, const glm::ivec2& pos);
        void setWindowedMaximized();

        GLFWmonitor* getCurrentMonitor();

        bool isKeyPressed(int key);
        bool isMouseButtonPressed(int button);
        glm::dvec2 getMousePosition();

        bool isKeyJustPressed(int key);
        bool isKeyJustReleased(int key);

        bool isMouseButtonJustPressed(int button);
        bool isMouseButtonJustReleased(int button);

        void kcbk(int key, int scancode, int action, int mods);
        void mcbk(int button, int action, int mods);

        void toggleFullscreen();

    private:

        glm::ivec2 m_WindowedLocation;
        glm::ivec2 m_WindowedSize;

        GLFWwindow *m_Window;

        static GLFWmonitor* getMonitor(int i);

        std::unordered_set<int> m_KeysJustPressed;
        std::unordered_set<int> m_ButtonsJustPressed;

        std::unordered_set<int> m_KeysJustReleased;
        std::unordered_set<int> m_ButtonsJustReleased;


        bool m_IsFullscreen = false;
    };

} // kat
