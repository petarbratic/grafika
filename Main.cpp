#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")
#include "Util.h"

int endProgram(std::string message) {
    std::cout << message << std::endl;
    glfwTerminate();
    return -1;
}

int main()
{

	ISoundEngine* engine = createIrrKlangDevice();
    if (!engine)
        return 0;

	engine->play2D("media/getout.ogg", true);
	


    // Inicijalizacija GLFW i postavljanje na verziju 3 sa programabilnim pajplajnom
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Formiranje prozora za prikaz sa datim dimenzijama i naslovom
    GLFWwindow* window = glfwCreateWindow(800, 800, "Vezba 2", NULL, NULL);
    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);

    // Inicijalizacija GLEW
    if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

    unsigned int basicShader = createShader("basic.vert", "basic.frag");

    // Tri temena čine jedan trougao
    float vertices[] = {
         -0.2f, 0.2f, 0.0f, 0.0f, 1.0f, // gornje levo teme
         -0.2f, -0.2f, 0.0f, 1.0f, 0.0f, // donje levo teme
         0.2f, -0.2f, 1.0f, 0.0f, 0.0f, // donje desno teme
    };

    // Inicijalizacija VAO i VBO, tipičnih struktura za čuvanje podataka o verteksima
    unsigned int VAO;
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atribut 0 (pozicija):
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atribut 1 (boja):
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glClearColor(0.5f, 0.6f, 1.0f, 1.0f); // Postavljanje boje pozadine

    while (!glfwWindowShouldClose(window))
    {



        glClear(GL_COLOR_BUFFER_BIT); // Bojenje pozadine, potrebno kako pomerajući objekti ne bi ostavljali otisak

        glUseProgram(basicShader); // Podešavanje da se crta koristeći dati šejder
        glBindVertexArray(VAO); // Podešavanje da se crta koristeći date vertekse

        // Crtaju se trouglovi, krećući se od nultog elementa u vertices i zaustavljajući se posle 3 temena
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window); // Zamena bafera - prednji i zadnji bafer se menjaju kao štafeta; dok jedan procesuje, drugi se prikazuje.
        glfwPollEvents(); // Sinhronizacija pristiglih događaja
    }

    glfwTerminate();

    engine->drop();


    return 0;
}