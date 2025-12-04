#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Audio.h"
#include "Graphics.h"

namespace {

    // Stvarni GLFW callback
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
            return;
        }
        // Pomeranje leve strane (bas sekcije) strelicama
        if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) &&
            (action == GLFW_PRESS || action == GLFW_REPEAT)) {

            float step = 0.02f; // koliko po pritisku
            if (key == GLFW_KEY_LEFT) {
                Graphics::moveLeftSide(-step);
            }
            else {
                Graphics::moveLeftSide(step);
            }

            return; // ne prosleđujemo ovo Audio modu
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
