#include "Input.h"
#include <GLFW/glfw3.h>
#include "Audio.h"

namespace {

    // Stvarni GLFW callback
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
            return;
        }

        // Sve ostalo prosledi audiju
        Audio::handleKey(key, action);
    }

} // anonymous namespace

namespace Input {

    void setup(GLFWwindow* window) {
        glfwSetKeyCallback(window, keyCallback);
    }

}
