#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Vec2.h"
#include "Vec3.h"
#include "Renderer.h"
#include "../EventSystem/EventManager.h"
#include "KeyCode.h"
#include "Cursor.h"

class Input {
public:

    static void SetUp(GLFWwindow *window);
    static void UpdateInput();

    // Window
    static GLFWwindow* GetWindow() { return m_window; }
    static void SetWindow(GLFWwindow* newWindow) { m_window = newWindow; }
    static Vec2 GetPixToNorm(Vec2 pix);
    static Vec2 GetNormToPix(Vec2 Norm);

    // Keyboard
    static bool GetKeyDown(KeyCode key);
    static bool GetKeyUp(KeyCode key);

    //Mouse
    static void SetCursorMode(CursorMode mode);
    static CursorMode GetCursorMode();
    static bool IsCursorInWindow() { return glfwGetWindowAttrib(m_window, GLFW_HOVERED); }

    static bool GetMouseButtonDown(MouseCode button);
    static bool GetMouseButtonUp(MouseCode button);

    static Vec2 GetCursorPosPix();
    static Vec2 GetCursorPosNorm();
    static Vec2 GetMouseDeltaPix();
    static Vec2 GetMouseDeltaNorm();
    static void SetCursorPosPix(const Vec2& newPos) { glfwSetCursorPos(m_window, newPos.x, newPos.y); }
    static void SetCursorPosNorm(const Vec2& newPos);

private:
    static GLFWwindow* m_window;
    static Vec2 m_lastMousePosPix;
    static Vec2 m_lastMousePosNorm;

    static void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void CharEvent(GLFWwindow* window, unsigned int codepoint);
    static void CursorPosEvent(GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
    static void ScrollEvent(GLFWwindow* window, double xoffset, double yoffset);
    static void CursorEnterEvent(GLFWwindow* window, int entered);


    static bool NullWindow();
};
