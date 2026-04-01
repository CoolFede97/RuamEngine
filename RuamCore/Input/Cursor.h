#pragma once

enum MouseCode {
    Mouse_Left,
    Mouse_Right,
    Mouse_Middle,
    Mouse_Last = 7
};

enum CursorMode
{
    // These values come from the ones of "glfw3.h"
    MouseNormal = 0x00034001,
    MouseHidden = 0x00034002,
    MouseDisabled = 0x00034003,
    MouseCaptured = 0x00034004
};
