#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Graphics {

    // Inicijalizacija OpenGL stanja, šejdera, VAO/VBO, tekstura
    bool init();

    // Crta JEDAN frejm, uz jednostavan frame limiter
    // previousTime je referenca na promenljivu koja se prati u main-u
    void renderFrame(GLFWwindow* window, double& previousTime);

    // Čišćenje resursa
    void shutdown();

}
