#include "Input.h"

#include "EventManager.h"
#include "GLFW/glfw3.h"
#include "KeyCode.h"

namespace RuamEngine
{
    GLFWwindow* Input::s_window = nullptr;
    Vec2 Input::s_lastMousePosPix = Vec2(0.0f, 0.0f);
    Vec2 Input::s_lastMousePosNorm = Vec2(0.0f, 0.0f);
    std::map<KeyCode, bool> Input::s_previousKeys;
    const KeyCode Input::s_supportedKeys[] =
    {
   		SpaceBar_Key,
        Quote_Key,
        Comma_Key,
        Minus_Key,
        Period_Key,
        Slash_Key,

        // Upper numbers
        Key_0, Key_1, Key_2, Key_3, Key_4,
        Key_5, Key_6, Key_7, Key_8, Key_9,

        Semicolon_Key,
        Equals_Key,

        // Letters
        A_Key, B_Key, C_Key, D_Key, E_Key,
        F_Key, G_Key, H_Key, I_Key, J_Key,
        K_Key, L_Key, M_Key, N_Key, O_Key,
        P_Key, Q_Key, R_Key, S_Key, T_Key,
        U_Key, V_Key, W_Key, X_Key, Y_Key, Z_Key,

        LeftBracket_Key,
        Backslash_Key,
        RightBracket_Key,
        BackQuote_Key,

        // Function keys
        Escape_Key,
        Enter_Key,
        Tab_Key,
        Backspace_Key,
        Insert_Key,
        Delete_Key,
        Right_Arrow,
        Left_Arrow,
        Down_Arrow,
        Up_Arrow,
        PageUp_Key,
        PageDown_Key,
        Home_Key,
        End_Key,

        CapsLock_Key,
        ScrollLock_Key,
        NumLock_Key,
        PrintScreen_Key,
        PauseBreak_Key,

        F1_Key, F2_Key, F3_Key, F4_Key, F5_Key, F6_Key,
        F7_Key, F8_Key, F9_Key, F10_Key, F11_Key, F12_Key,

        // Numpad
        Keypad_0, Keypad_1, Keypad_2, Keypad_3, Keypad_4,
        Keypad_5, Keypad_6, Keypad_7, Keypad_8, Keypad_9,
        Keypad_Period,
        Keypad_Divide,
        Keypad_Multiply,
        Keypad_Minus,
        Keypad_Plus,
        Keypad_Enter,
        Keypad_Equals,

        // Modifiers
        LeftShift_Key,
        LeftControl_Key,
        LeftAlt_Key,
        LeftCommand_Key,
        RightShift_Key,
        RightControl_Key,
        RightAlt_Key,
        RightCommand_Key,

        Menu_Key
    };

    bool Input::NullWindow()
    {
        if (s_window == nullptr) return true;
        return false;
    }

    Vec2 Input::GetPixToNorm(Vec2 pix) {
        int width, height;
        glfwGetWindowSize(s_window, &width, &height);
        return Vec2((pix.x / (float)width) * 2.0f - 1.0f, (1.0f - (pix.y / (float)height)) * 2.0f - 1.0f);
    }

    Vec2 Input::GetNormToPix(Vec2 norm) {
        int width, height;
        glfwGetWindowSize(s_window, &width, &height);
        return Vec2(((norm.x + 1.0f) / 2.0f) * (float)width, ((1.0f - norm.y) / 2.0f) * (float)height);
    }

    bool Input::GetKey(KeyCode key)
    {
        return glfwGetKey(s_window, key) == GLFW_PRESS;
    }

    bool Input::GetKeyDown(KeyCode key)
    {
    	return glfwGetKey(s_window, key) == GLFW_PRESS && !s_previousKeys[key];
    }

    bool Input::GetKeyUp(KeyCode key)
    {
    	return glfwGetKey(s_window, key) == GLFW_RELEASE && s_previousKeys[key];
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
        glfwSetInputMode(s_window, GLFW_CURSOR, mode);
    }

    CursorMode Input::GetCursorMode() {
        return static_cast<CursorMode>(glfwGetInputMode(s_window, GLFW_CURSOR));
    }

    bool Input::GetMouseButtonDown(MouseCode button) {
        // Return True if the mouse button is down

        return glfwGetMouseButton(s_window, button) == GLFW_PRESS;
    }

    bool Input::GetMouseButtonUp(const MouseCode button) {
        // Return True if the key is up

        return glfwGetMouseButton(s_window, button) == GLFW_RELEASE;
    }

    Vec2 Input::GetMouseDeltaPix() {
        return GetCursorPosPix() - s_lastMousePosPix;
    }

    Vec2 Input::GetMouseDeltaNorm() {
        return GetCursorPosNorm() - s_lastMousePosNorm;
    }

    Vec2 Input::GetCursorPosPix() {
        double xpos, ypos;
        glfwGetCursorPos(s_window, &xpos, &ypos);
        return Vec2((float)xpos, (float)ypos);
    }

    Vec2 Input::GetCursorPosNorm() {
        double xpos, ypos;
        glfwGetCursorPos(s_window, &xpos, &ypos);
        return GetPixToNorm(Vec2((float)xpos, (float)ypos));
    }

    void Input::SetCursorPosNorm(const Vec2& newPos) {
        Vec2 position = GetNormToPix(newPos);
        glfwSetCursorPos(s_window, position.x, position.y);
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
        s_window = window;
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

        glfwSetKeyCallback(s_window, KeyEvent);
        glfwSetCharCallback(s_window, CharEvent);
        glfwSetCursorPosCallback(s_window, CursorPosEvent);
        glfwSetMouseButtonCallback(s_window, MouseButtonEvent);
        glfwSetScrollCallback(s_window, ScrollEvent);
        glfwSetCursorEnterCallback(s_window, CursorEnterEvent);

    }

    void Input::UpdateInput() {
        if (NullWindow()) {
            return;
        }
        for (KeyCode keyCode : s_supportedKeys)
        {
       		s_previousKeys[keyCode] = GetKey(keyCode);
        }

        // Update mouse position
        s_lastMousePosPix = GetCursorPosPix();
        s_lastMousePosNorm = GetCursorPosNorm();
    }
}
