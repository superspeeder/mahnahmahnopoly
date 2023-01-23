#include "Window.hpp"
#include <algorithm>
#include <cmath>

namespace kat {


    void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        void* ptr = glfwGetWindowUserPointer(window);
        if (ptr) {
            auto* w = (Window*)ptr;
            w->kcbk(key, scancode, action, mods);
        }
    }

    void mousecallback(GLFWwindow* window, int button, int action, int mods) {
        void* ptr = glfwGetWindowUserPointer(window);
        if (ptr) {
            auto* w = (Window*)ptr;
            w->mcbk(button, action, mods);
        }
    }

    bool glfw_get_mouse_monitor(
            GLFWmonitor** monitor,
            GLFWwindow* window
    ) {
        bool success = false;

        double cursor_position[2] = {0};
        glfwGetCursorPos(window, &cursor_position[0], &cursor_position[1]);

        int window_position[2] = {0};
        glfwGetWindowPos(window, &window_position[0], &window_position[1]);

        int monitors_size = 0;
        GLFWmonitor** monitors = glfwGetMonitors(&monitors_size);

        // convert cursor position from window coordinates to screen coordinates
        cursor_position[0] += window_position[0];
        cursor_position[1] += window_position[1];

        for (int i = 0; ((!success) && (i < monitors_size)); ++i)
        {
            int monitor_position[2] = {0};
            glfwGetMonitorPos(monitors[i], &monitor_position[0], &monitor_position[1]);

            const GLFWvidmode* monitor_video_mode = glfwGetVideoMode(monitors[i]);

            if (
                    (cursor_position[0] < monitor_position[0]) ||
                    (cursor_position[0] > (monitor_position[0] + monitor_video_mode->width)) ||
                    (cursor_position[1] < monitor_position[1]) ||
                    (cursor_position[1] > (monitor_position[1] + monitor_video_mode->height))
                    ) {
                *monitor = monitors[i];
                success = true;
            }
        }

        // true: monitor contains the monitor the mouse is on
        // false: monitor is unmodified
        return success;
    }

    bool glfw_get_window_monitor(
            GLFWmonitor** monitor,
            GLFWwindow* window
    ) {
        bool success = false;

        int window_rectangle[4] = {0};
        glfwGetWindowPos(window, &window_rectangle[0], &window_rectangle[1]);
        glfwGetWindowSize(window, &window_rectangle[2], &window_rectangle[3]);

        int monitors_size = 0;
        GLFWmonitor** monitors = glfwGetMonitors(&monitors_size);

        GLFWmonitor* closest_monitor = NULL;
        int max_overlap_area = 0;

        for (int i = 0; i < monitors_size; ++i)
        {
            int monitor_position[2] = {0};
            glfwGetMonitorPos(monitors[i], &monitor_position[0], &monitor_position[1]);

            const GLFWvidmode* monitor_video_mode = glfwGetVideoMode(monitors[i]);

            int monitor_rectangle[4] = {monitor_position[0], monitor_position[1], monitor_video_mode->width, monitor_video_mode->height};

            if (
                    !(
                            ((window_rectangle[0] + window_rectangle[2]) < monitor_rectangle[0]) ||
                            (window_rectangle[0] > (monitor_rectangle[0] + monitor_rectangle[2])) ||
                            ((window_rectangle[1] + window_rectangle[3]) < monitor_rectangle[1]) ||
                            (window_rectangle[1] > (monitor_rectangle[1] + monitor_rectangle[3]))
                    )
                    ) {
                int intersection_rectangle[4] = {0};

                // x, width
                if (window_rectangle[0] < monitor_rectangle[0])
                {
                    intersection_rectangle[0] = monitor_rectangle[0];

                    if ((window_rectangle[0] + window_rectangle[2]) < (monitor_rectangle[0] + monitor_rectangle[2]))
                    {
                        intersection_rectangle[2] = (window_rectangle[0] + window_rectangle[2]) - intersection_rectangle[0];
                    }
                    else
                    {
                        intersection_rectangle[2] = monitor_rectangle[2];
                    }
                }
                else
                {
                    intersection_rectangle[0] = window_rectangle[0];

                    if ((monitor_rectangle[0] + monitor_rectangle[2]) < (window_rectangle[0] + window_rectangle[2]))
                    {
                        intersection_rectangle[2] = (monitor_rectangle[0] + monitor_rectangle[2]) - intersection_rectangle[0];
                    }
                    else
                    {
                        intersection_rectangle[2] = window_rectangle[2];
                    }
                }

                // y, height
                if (window_rectangle[1] < monitor_rectangle[1])
                {
                    intersection_rectangle[1] = monitor_rectangle[1];

                    if ((window_rectangle[1] + window_rectangle[3]) < (monitor_rectangle[1] + monitor_rectangle[3]))
                    {
                        intersection_rectangle[3] = (window_rectangle[1] + window_rectangle[3]) - intersection_rectangle[1];
                    }
                    else
                    {
                        intersection_rectangle[3] = monitor_rectangle[3];
                    }
                }
                else
                {
                    intersection_rectangle[1] = window_rectangle[1];

                    if ((monitor_rectangle[1] + monitor_rectangle[3]) < (window_rectangle[1] + window_rectangle[3]))
                    {
                        intersection_rectangle[3] = (monitor_rectangle[1] + monitor_rectangle[3]) - intersection_rectangle[1];
                    }
                    else
                    {
                        intersection_rectangle[3] = window_rectangle[3];
                    }
                }

                int overlap_area = intersection_rectangle[2] * intersection_rectangle[3];
                if (overlap_area > max_overlap_area)
                {
                    closest_monitor = monitors[i];
                    max_overlap_area = overlap_area;
                }
            }
        }

        if (closest_monitor)
        {
            *monitor = closest_monitor;
            success = true;
        }

        // true: monitor contains the monitor the window is most on
        // false: monitor is unmodified
        return success;
    }

    glm::ivec2 getMonitorSize(GLFWmonitor *monitor) {
        const GLFWvidmode* vm = glfwGetVideoMode(monitor);
        return {vm->width, vm->height};
    }

    GLFWmonitor *getPrimaryMonitor() {
        return glfwGetPrimaryMonitor();
    }

    int getMonitorCount() {
        int c;
        glfwGetMonitors(&c);
        return c;
    }

    std::vector<MonitorInfo> getMonitorInfo() {
        std::vector<MonitorInfo> minfo;
        int c;
        auto ms = glfwGetMonitors(&c);

        for (int i = 0 ; i < c ; i++) {
            minfo.push_back(getMonitorInfo(ms[i]));
        }

        return minfo;
    }

    MonitorInfo getMonitorInfo(GLFWmonitor *mon) {
        MonitorInfo inf{};
        inf.monitorName = glfwGetMonitorName(mon);
        inf.res = getMonitorSize(mon);
        return inf;
    }

    Window::Window(const glm::ivec2& size, const std::string& title) : m_WindowedSize(size) {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, true);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_Window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        glfwSetWindowUserPointer(m_Window, this);

        glfwSetKeyCallback(m_Window, keycallback);
        glfwSetMouseButtonCallback(m_Window, mousecallback);

        glfwGetWindowPos(m_Window, &m_WindowedLocation.x, &m_WindowedLocation.y);
    }

    Window::~Window() {
        glfwDestroyWindow(m_Window);
    }

    bool Window::isOpen() {
        return !glfwWindowShouldClose(m_Window);
    }

    void Window::poll() {
        m_KeysJustPressed.clear();
        glfwPollEvents();
    }

    void Window::swap() {
        glfwSwapBuffers(m_Window);
    }

    void Window::close() {
        glfwSetWindowShouldClose(m_Window, true);
    }

    void Window::setFullscreen() {
        setFullscreen(glfwGetPrimaryMonitor());
    }

    void Window::setFullscreen(GLFWmonitor *monitor) {
        if (!m_IsFullscreen) {
            m_IsFullscreen = true;
            glfwGetWindowPos(m_Window, &m_WindowedLocation.x, &m_WindowedLocation.y);

            glm::ivec2 pos{};
            glfwGetMonitorPos(monitor, &pos.x, &pos.y);

            const auto *vm = glfwGetVideoMode(monitor);
            glfwSetWindowAttrib(m_Window, GLFW_DECORATED, false);
            glfwSetWindowMonitor(m_Window, nullptr, pos.x, pos.y, vm->width, vm->height, -1);
        }
    }

    void Window::setFullscreen(int i) {
        setFullscreen(getMonitor(i));
    }

    void Window::setWindowed() {
        setWindowed(m_WindowedSize);
    }

    void Window::setWindowed(const glm::ivec2 &size) {
        setWindowed(size, m_WindowedLocation);
    }

    void Window::setWindowed(const glm::ivec2 &size, const glm::ivec2 &pos) {
        if (m_IsFullscreen) {
            m_IsFullscreen = false;
            glfwSetWindowAttrib(m_Window, GLFW_DECORATED, true);
            glfwSetWindowMonitor(m_Window, nullptr, pos.x, pos.y, size.x, size.y, -1);
        }
    }

    void Window::setWindowedMaximized() {
        glfwSetWindowAttrib(m_Window, GLFW_MAXIMIZED, true);
        glfwGetWindowPos(m_Window, &m_WindowedLocation.x, &m_WindowedLocation.y);
        glfwGetWindowSize(m_Window, &m_WindowedSize.x, &m_WindowedSize.y);

    }

    GLFWmonitor *Window::getCurrentMonitor() {
        GLFWmonitor* mon;
        glfw_get_window_monitor(&mon, m_Window);
        return mon;
    }

    GLFWmonitor* Window::getMonitor(int i) {
        int c;
        auto ms = glfwGetMonitors(&c);
        return ms[std::clamp(i, 0, c - 1)];
    }

    bool Window::isKeyPressed(int key) {
        return glfwGetKey(m_Window, key);
    }

    bool Window::isMouseButtonPressed(int button) {
        return glfwGetMouseButton(m_Window, button);
    }

    glm::dvec2 Window::getMousePosition() {
        glm::dvec2 p;
        glfwGetCursorPos(m_Window, &p.x, &p.y);
        return p;
    }

    bool Window::isKeyJustPressed(int key) {
        return m_KeysJustPressed.contains(key);
    }

    bool Window::isKeyJustReleased(int key) {
        return m_KeysJustReleased.contains(key);
    }

    bool Window::isMouseButtonJustPressed(int button) {
        return m_ButtonsJustPressed.contains(button);
    }

    bool Window::isMouseButtonJustReleased(int button) {
        return m_ButtonsJustReleased.contains(button);
    }

    void Window::kcbk(int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            m_KeysJustPressed.insert(key);
        } else if (action == GLFW_RELEASE) {
            m_KeysJustReleased.insert(key);
        }
    }

    void Window::mcbk(int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            m_ButtonsJustPressed.insert(button);
        } else if (action == GLFW_RELEASE) {
            m_ButtonsJustReleased.insert(button);
        }
    }

    void Window::toggleFullscreen() {
        if (m_IsFullscreen) {
            setWindowed();
        } else {
            setFullscreen();
        }
    }

} // kat