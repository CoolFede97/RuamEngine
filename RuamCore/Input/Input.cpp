#include "Input.h"

#include "Cursor.h"
#include "EventManager.h"
#include "GLFW/glfw3.h"
#include "KeyCode.h"

namespace RuamEngine
{
    GLFWwindow* Input::s_window = nullptr;
    Vec2 Input::s_lastMousePosPix = Vec2(0.0f, 0.0f);
    Vec2 Input::s_lastMousePosNorm = Vec2(0.0f, 0.0f);
    Vec2 Input::s_mouseDeltaPix = Vec2(0.0f, 0.0f);
    Vec2 Input::s_mouseDeltaNorm = Vec2(0.0f, 0.0f);

    std::map<KeyCode, bool> Input::s_previousKeys;
    std::map<MouseCode, bool> Input::s_previousMouses;

    const KeyCode Input::s_supportedKeys[] =
    {
        KeyCode::SpaceBar_Key,
        KeyCode::Quote_Key,
        KeyCode::Comma_Key,
        KeyCode::Minus_Key,
        KeyCode::Period_Key,
        KeyCode::Slash_Key,

        // Upper numbers
        KeyCode::Key_0, KeyCode::Key_1, KeyCode::Key_2, KeyCode::Key_3, KeyCode::Key_4,
        KeyCode::Key_5, KeyCode::Key_6, KeyCode::Key_7, KeyCode::Key_8, KeyCode::Key_9,

        KeyCode::Semicolon_Key,
        KeyCode::Equals_Key,

        // Letters
        KeyCode::A_Key, KeyCode::B_Key, KeyCode::C_Key, KeyCode::D_Key, KeyCode::E_Key,
        KeyCode::F_Key, KeyCode::G_Key, KeyCode::H_Key, KeyCode::I_Key, KeyCode::J_Key,
        KeyCode::K_Key, KeyCode::L_Key, KeyCode::M_Key, KeyCode::N_Key, KeyCode::O_Key,
        KeyCode::P_Key, KeyCode::Q_Key, KeyCode::R_Key, KeyCode::S_Key, KeyCode::T_Key,
        KeyCode::U_Key, KeyCode::V_Key, KeyCode::W_Key, KeyCode::X_Key, KeyCode::Y_Key, KeyCode::Z_Key,

        KeyCode::LeftBracket_Key,
        KeyCode::Backslash_Key,
        KeyCode::RightBracket_Key,
        KeyCode::BackQuote_Key,

        // Function keys
        KeyCode::Escape_Key,
        KeyCode::Enter_Key,
        KeyCode::Tab_Key,
        KeyCode::Backspace_Key,
        KeyCode::Insert_Key,
        KeyCode::Delete_Key,
        KeyCode::Right_Arrow,
        KeyCode::Left_Arrow,
        KeyCode::Down_Arrow,
        KeyCode::Up_Arrow,
        KeyCode::PageUp_Key,
        KeyCode::PageDown_Key,
        KeyCode::Home_Key,
        KeyCode::End_Key,

        KeyCode::CapsLock_Key,
        KeyCode::ScrollLock_Key,
        KeyCode::NumLock_Key,
        KeyCode::PrintScreen_Key,
        KeyCode::PauseBreak_Key,

        KeyCode::F1_Key, KeyCode::F2_Key, KeyCode::F3_Key, KeyCode::F4_Key, KeyCode::F5_Key, KeyCode::F6_Key,
        KeyCode::F7_Key, KeyCode::F8_Key, KeyCode::F9_Key, KeyCode::F10_Key, KeyCode::F11_Key, KeyCode::F12_Key,

        // Numpad
        KeyCode::Keypad_0, KeyCode::Keypad_1, KeyCode::Keypad_2, KeyCode::Keypad_3, KeyCode::Keypad_4,
        KeyCode::Keypad_5, KeyCode::Keypad_6, KeyCode::Keypad_7, KeyCode::Keypad_8, KeyCode::Keypad_9,
        KeyCode::Keypad_Period,
        KeyCode::Keypad_Divide,
        KeyCode::Keypad_Multiply,
        KeyCode::Keypad_Minus,
        KeyCode::Keypad_Plus,
        KeyCode::Keypad_Enter,
        KeyCode::Keypad_Equals,

        // Modifiers
        KeyCode::LeftShift_Key,
        KeyCode::LeftControl_Key,
        KeyCode::LeftAlt_Key,
        KeyCode::LeftCommand_Key,
        KeyCode::RightShift_Key,
        KeyCode::RightControl_Key,
        KeyCode::RightAlt_Key,
        KeyCode::RightCommand_Key,

        KeyCode::Menu_Key
    };
    const MouseCode Input::s_supportedMouses[] =
    {
        MouseCode::Mouse_Left,
        MouseCode::Mouse_Right,
        MouseCode::Mouse_Middle,
        MouseCode::Mouse_Last
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
        return glfwGetKey(s_window, static_cast<int>(key)) == GLFW_PRESS;
    }

    bool Input::GetKeyDown(KeyCode key)
    {
    	return GetKey(key) && !s_previousKeys[key];
    }

    bool Input::GetKeyUp(KeyCode key)
    {
    	return !GetKey(key) && s_previousKeys[key];
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
        glfwSetInputMode(s_window, GLFW_CURSOR, static_cast<int>(mode));
    }

    CursorMode Input::GetCursorMode() {
        return static_cast<CursorMode>(glfwGetInputMode(s_window, GLFW_CURSOR));
    }

    bool Input::GetMouseButton(MouseCode button)
    {
        return glfwGetMouseButton(s_window, static_cast<int>(button)) == GLFW_PRESS;
    }

    bool Input::GetMouseButtonDown(MouseCode button) {
        return GetMouseButton(button) && !s_previousMouses[button];
    }

    bool Input::GetMouseButtonUp(const MouseCode button) {
       	return !GetMouseButton(button) && s_previousMouses[button];
    }

    Vec2 Input::GetMouseDeltaPix() {
        return s_mouseDeltaPix;
    }

    Vec2 Input::GetMouseDeltaNorm() {
        return s_mouseDeltaNorm;
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

        if (glfwRawMouseMotionSupported())
        {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }

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
        for (MouseCode mouseCode : s_supportedMouses)
        {
       		s_previousMouses[mouseCode] = GetMouseButton(mouseCode);
        }
        // Update mouse position
        Vec2 currentPix = GetCursorPosPix();
        Vec2 currentNorm = GetCursorPosNorm();

        s_mouseDeltaPix = currentPix - s_lastMousePosPix;
        s_mouseDeltaNorm = currentNorm - s_lastMousePosNorm;

        s_lastMousePosPix = currentPix;
        s_lastMousePosNorm = currentNorm;
    }
}
