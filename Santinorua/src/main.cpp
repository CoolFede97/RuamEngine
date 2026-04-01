#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Input.h"

int main(void) {
    // GLFWwindow* window;

    // /* Initialize the library */
    // if (!glfwInit())
    //     return -1;

    // /* Create a windowed mode window and its OpenGL context */
    // window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    // if (!window)
    // {
    //     glfwTerminate();
    //     return -1;
    // }
    // Input::SetWindow(window);
    // /* Make the window's context current */
    // glfwMakeContextCurrent(window);
    // /* Loop until the user closes the window */


    // // Test event subscriptions
    // EventManager::Subscribe<OnKeyPressEvent>([](const OnKeyPressEvent& event) {
    //     std::cout << "Key Pressed: " << event.key << std::endl;
    // });

    // EventManager::Subscribe<OnKeyReleaseEvent>([](const OnKeyReleaseEvent& event) {
    //     std::cout << "Key Released: " << event.key << std::endl;
    // });

    // EventManager::Subscribe<OnMouseMoveEvent>([](const OnMouseMoveEvent& event) {
    //     //std::cout << "Mouse position: " << event.positionNorm.x << ", " << event.positionNorm.y << std::endl;
    // });

    // EventManager::Subscribe<OnMouseButtonDownEvent>([](const OnMouseButtonDownEvent& event) {
    //     std::cout << "Mouse Button Down: " << event.button << " at " << event.positionNorm.x << ", " << event.positionNorm.y << std::endl;
    // });

    // EventManager::Subscribe<OnMouseButtonUpEvent>([](const OnMouseButtonUpEvent& event) {
    //     std::cout << "Mouse Button Up: " << event.button << " at " << event.positionNorm.x << ", " << event.positionNorm.y << std::endl;
    // });

    // EventManager::Subscribe<OnMouseScrollEvent>([](const OnMouseScrollEvent& event) {
    //     std::cout << "Mouse Scrolled: " << event.offset.x << ", " << event.offset.y << std::endl;
    // });

    // EventManager::Subscribe<OnCharEvent>([](const OnCharEvent& event) {
    //     std::cout << "Char Input: " << event.unicodeChar << std::endl;
    // });

    // //Input::SetCursorMode(MouseDisabled);
    // while (!glfwWindowShouldClose(window))
    // {

    //     /* Render here */
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     /* Swap front and back buffers */
    //     glfwSwapBuffers(window);

    //     /* Poll for and process events */
    //     glfwPollEvents();

    //     // Update input
    //     Input::UpdateInput();

    //     //std::cout << "Mouse Position: " << Input::GetCursorPosPix().x << ", " << Input::GetCursorPosPix().y << std::endl;
    //     //std::cout << "Mouse Normalized: " << Input::GetCursorPosNorm().x << ", " << Input::GetCursorPosNorm().y << std::endl;


    //     EventManager::HandleEvents();
    // }

    // glfwTerminate();
    return 0;
}
