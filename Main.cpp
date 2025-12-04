#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Util.h"
#include "Audio.h"
#include "Input.h"
#include "Graphics.h"

int endProgram(const std::string& message) {
    std::cout << message << std::endl;
    glfwTerminate();
    return -1;
}

int main() {
    // --- AUDIO ---
    if (!Audio::init()) {
        return endProgram("Audio nije uspeo da se inicijalizuje.");
    }

    // --- GLFW / OpenGL ---
    if (!glfwInit()) {
        Audio::shutdown();
        return endProgram("GLFW nije uspeo da se inicijalizuje.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(
        mode->width,
        mode->height,
        "Harmonika",
        monitor,   // fullscreen monitor
        NULL
    );
    if (!window) {
        Audio::shutdown();
        return endProgram("Prozor nije uspeo da se kreira.");
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (glewInit() != GLEW_OK) {
        Audio::shutdown();
        return endProgram("GLEW nije uspeo da se inicijalizuje.");
    }

    // Input callback
    Input::setup(window);

    // Graphics init
    if (!Graphics::init()) {
        Audio::shutdown();
        return endProgram("Graphics init nije uspeo.");
    }

    double previousTime = glfwGetTime();

    // --- GLAVNA PETLJA ---
    while (!glfwWindowShouldClose(window)) {
        Graphics::renderFrame(window, previousTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- ČIŠĆENJE ---
    Graphics::shutdown();
    Audio::shutdown();
    glfwTerminate();

    return 0;
}
