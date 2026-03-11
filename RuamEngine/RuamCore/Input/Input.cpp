#include "Input.h"

#include "EventManager.h"
#include "GLFW/glfw3.h"

namespace RuamEngine
{
    GLFWwindow* Input::m_window = nullptr;
    Vec2 Input::m_lastMousePosPix = Vec2(0.0f, 0.0f);
    Vec2 Input::m_lastMousePosNorm = Vec2(0.0f, 0.0f);

    bool Input::NullWindow()
    {
        if (m_window == nullptr) return true;
        return false;
    }

    Vec2 Input::GetPixToNorm(Vec2 pix) {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        return Vec2((pix.x / (float)width) * 2.0f - 1.0f, (1.0f - (pix.y / (float)height)) * 2.0f - 1.0f);
    }

    Vec2 Input::GetNormToPix(Vec2 norm) {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        return Vec2(((norm.x + 1.0f) / 2.0f) * (float)width, ((1.0f - norm.y) / 2.0f) * (float)height);
    }


    bool Input::GetKeyDown(KeyCode key) {
        // Return True if the key is down

        return glfwGetKey(m_window, key) == GLFW_PRESS;
    }

    bool Input::GetKeyUp(const KeyCode key) {
        // Return True if the key is up

        return glfwGetKey(m_window, key) == GLFW_RELEASE;
    }

    void Input::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Handle key events
        if (action == GLFW_PRESS) {
            // Key pressed
            EventManager::Publish(OnKeyPressEvent(key));
        } else if (action == GLFW_RELEASE) {
            // Key released
            EventManager::Publish(OnKeyReleaseEvent(key));
        }
    }

    void Input::CharEvent(GLFWwindow* window, unsigned int codepoint) {
        EventManager::Publish(OnCharEvent(codepoint));
    }

    void Input::SetCursorMode(const CursorMode mode)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, mode);
    }

    CursorMode Input::GetCursorMode() {
        return static_cast<CursorMode>(glfwGetInputMode(m_window, GLFW_CURSOR));
    }

    bool Input::GetMouseButtonDown(MouseCode button) {
        // Return True if the mouse button is down

        return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
    }

    bool Input::GetMouseButtonUp(const MouseCode button) {
        // Return True if the key is up

        return glfwGetMouseButton(m_window, button) == GLFW_RELEASE;
    }

    Vec2 Input::GetMouseDeltaPix() {
        return GetCursorPosPix() - m_lastMousePosPix;
    }

    Vec2 Input::GetMouseDeltaNorm() {
        return GetCursorPosNorm() - m_lastMousePosNorm;
    }

    Vec2 Input::GetCursorPosPix() {
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);
        return Vec2((float)xpos, (float)ypos);
    }

    Vec2 Input::GetCursorPosNorm() {
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);
        return GetPixToNorm(Vec2((float)xpos, (float)ypos));
    }

    void Input::SetCursorPosNorm(const Vec2& newPos) {
        Vec2 position = GetNormToPix(newPos);
        glfwSetCursorPos(m_window, position.x, position.y);
    }

    void Input::CursorPosEvent(GLFWwindow* window, double xpos, double ypos) {
        Vec2 position = Vec2(xpos, ypos);
        Vec2 positionNormalized = GetPixToNorm(position);

        EventManager::Publish(OnMouseMoveEvent(position, positionNormalized));
    }

    void Input::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods) {
        Vec2 positionPix = GetCursorPosPix();
        Vec2 positionNorm = GetCursorPosNorm();

        if (action == GLFW_PRESS) {
            EventManager::Publish(OnMouseButtonDownEvent(positionPix, positionNorm, static_cast<MouseCode>(button)));
        } else if (action == GLFW_RELEASE) {
            EventManager::Publish(OnMouseButtonUpEvent(positionPix, positionNorm, static_cast<MouseCode>(button)));
        }
    }

    void Input::ScrollEvent(GLFWwindow* window, double xoffset, double yoffset) {
        Vec2 positionPix = GetCursorPosPix();
        Vec2 positionNorm = GetPixToNorm(positionPix);
        EventManager::Publish(OnMouseScrollEvent(Vec2(xoffset, yoffset), positionPix, positionNorm));
    }

    void Input::CursorEnterEvent(GLFWwindow* window, int entered) {
        Vec2 positionPix = GetCursorPosPix();
        Vec2 positionNorm = GetPixToNorm(positionPix);
        if (entered) {
            EventManager::Publish(OnMouseEnterWindowEvent(positionPix, positionNorm));
        } else {
            EventManager::Publish(OnMouseLeaveWindowEvent(positionPix, positionNorm));
        }
    }

    void Input::SetUp(GLFWwindow* window) {
        // Set the window pointer
        m_window = window;
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

        glfwSetKeyCallback(m_window, KeyEvent);
        glfwSetCharCallback(m_window, CharEvent);
        glfwSetCursorPosCallback(m_window, CursorPosEvent);
        glfwSetMouseButtonCallback(m_window, MouseButtonEvent);
        glfwSetScrollCallback(m_window, ScrollEvent);
        glfwSetCursorEnterCallback(m_window, CursorEnterEvent);

    }

    void Input::UpdateInput() {
        if (NullWindow()) {
            return;
        }

        // Update mouse position
        m_lastMousePosPix = GetCursorPosPix();
        m_lastMousePosNorm = GetCursorPosNorm();
    }
}
