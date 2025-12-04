#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")
#include "Util.h"
#include <vector>
#include <cmath>

const double FRAME_TIME = 1.0 / 75.0;
static double previousTime = glfwGetTime();

ISoundEngine* engine = nullptr;

struct KeyNote {
    int key;                // GLFW key code
    const char* filePath;   // putanja do wav/ogg fajla
    ISound* sound;          // trenutno aktivni zvuk (ili nullptr)
    bool isPressed;         // vizuelni feedback
};

struct BassButton {
    int key;               // GLFW key code za bas
    const char* filePath;  // putanja do wav fajla
    ISound* sound;         // aktivni zvuk
    bool isPressed;        // da li je pritisnut
    float cx, cy;          // centar kruga
    float r;               // poluprečnik
};

KeyNote notes[] = {
    { GLFW_KEY_Z, "semplHarmonika/c.wav",  nullptr, false }, // C
    { GLFW_KEY_X, "semplHarmonika/d.wav",  nullptr, false }, // D
    { GLFW_KEY_C, "semplHarmonika/e.wav",  nullptr, false }, // E
    { GLFW_KEY_V, "semplHarmonika/f.wav",  nullptr, false }, // F
    { GLFW_KEY_B, "semplHarmonika/g.wav",  nullptr, false }, // G
    { GLFW_KEY_N, "semplHarmonika/a.wav",  nullptr, false }, // A
    { GLFW_KEY_M, "semplHarmonika/h.wav",  nullptr, false }, // H
    { GLFW_KEY_COMMA, "semplHarmonika/c2.wav", nullptr, false }, // C2

    { GLFW_KEY_S, "semplHarmonika/c#.wav", nullptr, false }, // C#
    { GLFW_KEY_D, "semplHarmonika/d#.wav", nullptr, false }, // D#
    { GLFW_KEY_G, "semplHarmonika/f#.wav", nullptr, false }, // F#
    { GLFW_KEY_H, "semplHarmonika/g#.wav", nullptr, false }, // G#
    { GLFW_KEY_J, "semplHarmonika/Bb.wav", nullptr, false }, // A#
};

// 6 bas dugmića
BassButton bassButtons[] = {
    { GLFW_KEY_U, "semplHarmonika/Cbas.wav", nullptr, false, -0.4f, 0.4f,   0.04f },
    { GLFW_KEY_7, "semplHarmonika/Cdur.wav", nullptr, false, -0.32f, 0.465f, 0.04f },
    { GLFW_KEY_Y, "semplHarmonika/Gbas.wav", nullptr, false, -0.4f,  0.5f,   0.04f },

    { GLFW_KEY_6, "semplHarmonika/Gdur.wav", nullptr, false, -0.32f, 0.565f, 0.04f },
    { GLFW_KEY_I, "semplHarmonika/Fbas.wav", nullptr, false, -0.4f,  0.3f,   0.04f },
    { GLFW_KEY_8, "semplHarmonika/Fdur.wav", nullptr, false, -0.32f, 0.365f, 0.04f },
};

const int bassCount = sizeof(bassButtons) / sizeof(bassButtons[0]);
const int notesCount = sizeof(notes) / sizeof(notes[0]);

int endProgram(std::string message) {
    std::cout << message << std::endl;
    glfwTerminate();
    return -1;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // --- BAS dugmići ---
    for (int i = 0; i < bassCount; i++)
    {
        if (key == bassButtons[i].key)
        {
            if (action == GLFW_PRESS)
            {
                bassButtons[i].isPressed = true;

                if (!bassButtons[i].sound && engine)
                {
                    bassButtons[i].sound = engine->play2D(
                        bassButtons[i].filePath,
                        true,   // loop
                        false,  // startPaused
                        true    // track (vrati ISound*)
                    );
                }
            }
            else if (action == GLFW_RELEASE)
            {
                bassButtons[i].isPressed = false;

                if (bassButtons[i].sound)
                {
                    bassButtons[i].sound->stop();
                    bassButtons[i].sound->drop();
                    bassButtons[i].sound = nullptr;
                }
            }

            return; // pogodili smo bas dugme, ne treba dalje da proveravamo
        }
    }

    // --- DESNE DIRKE ---
    for (int i = 0; i < notesCount; i++)
    {
        if (key == notes[i].key)
        {
            if (action == GLFW_PRESS)
            {
                notes[i].isPressed = true;

                if (!notes[i].sound && engine)
                {
                    notes[i].sound = engine->play2D(notes[i].filePath, true, false, true);
                }
            }
            else if (action == GLFW_RELEASE)
            {
                notes[i].isPressed = false;

                if (notes[i].sound)
                {
                    notes[i].sound->stop();
                    notes[i].sound->drop();
                    notes[i].sound = nullptr;
                }
            }

            break;
        }
    }
}

int main()
{
    // ----------------------------------------------------------------------------------
    engine = createIrrKlangDevice();
    if (!engine)
        return 0;

    // Inicijalizacija GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Klasičan prozor 900x900
    GLFWwindow* window = glfwCreateWindow(900, 900, "Harmonika", NULL, NULL);
    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);

    // Sakrij sistemski kursor – koristimo naš teksturisani
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Inicijalizacija GLEW
    if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

    // Blending za providne teksture (ime.png, cursor.png)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int basicShader = createShader("basic.vert", "basic.frag");
    unsigned int rectShader = createShader("rect.vert", "rect.frag");

    // ----------------------------------------------------------------------------------
    // Verteksi za desne dirke

    float vertices[] = {

        // 8 BELIH DIRKI (C do C2) -------------------------

        // C (0)
        -0.8f, 0.8f, 1,1,1,
        -0.8f, 0.7f, 1,1,1,
        -0.5f, 0.7f, 1,1,1,
        -0.5f, 0.8f, 1,1,1,

        // D (1)
        -0.8f, 0.7f, 1,1,1,
        -0.8f, 0.6f, 1,1,1,
        -0.5f, 0.6f, 1,1,1,
        -0.5f, 0.7f, 1,1,1,

        // E (2)
        -0.8f, 0.6f, 1,1,1,
        -0.8f, 0.5f, 1,1,1,
        -0.5f, 0.5f, 1,1,1,
        -0.5f, 0.6f, 1,1,1,

        // F (3)
        -0.8f, 0.5f, 1,1,1,
        -0.8f, 0.4f, 1,1,1,
        -0.5f, 0.4f, 1,1,1,
        -0.5f, 0.5f, 1,1,1,

        // G (4)
        -0.8f, 0.4f, 1,1,1,
        -0.8f, 0.3f, 1,1,1,
        -0.5f, 0.3f, 1,1,1,
        -0.5f, 0.4f, 1,1,1,

        // A (5)
        -0.8f, 0.3f, 1,1,1,
        -0.8f, 0.2f, 1,1,1,
        -0.5f, 0.2f, 1,1,1,
        -0.5f, 0.3f, 1,1,1,

        // H (6)
        -0.8f, 0.2f, 1,1,1,
        -0.8f, 0.1f, 1,1,1,
        -0.5f, 0.1f, 1,1,1,
        -0.5f, 0.2f, 1,1,1,

        // C2 (7)
        -0.8f, 0.1f, 1,1,1,
        -0.8f, 0.0f, 1,1,1,
        -0.5f, 0.0f, 1,1,1,
        -0.5f, 0.1f, 1,1,1,

        // CRNE DIRKE (C#, D#, F#, G#, A#) ----------------

        // C# (iznad C–D)
        -0.74f, 0.73f, 0,0,0,
        -0.74f, 0.67f, 0,0,0,
        -0.5f,  0.67f, 0,0,0,
        -0.5f,  0.73f, 0,0,0,

        // D# (iznad D–E)
        -0.74f, 0.63f, 0,0,0,
        -0.74f, 0.57f, 0,0,0,
        -0.5f,  0.57f, 0,0,0,
        -0.5f,  0.63f, 0,0,0,

        // F# (iznad F–G)
        -0.74f, 0.43f, 0,0,0,
        -0.74f, 0.37f, 0,0,0,
        -0.5f,  0.37f, 0,0,0,
        -0.5f,  0.43f, 0,0,0,

        // G# (iznad G–A)
        -0.74f, 0.33f, 0,0,0,
        -0.74f, 0.27f, 0,0,0,
        -0.5f,  0.27f, 0,0,0,
        -0.5f,  0.33f, 0,0,0,

        // A# (iznad A–H)
        -0.74f, 0.23f, 0,0,0,
        -0.74f, 0.17f, 0,0,0,
        -0.5f,  0.17f, 0,0,0,
        -0.5f,  0.23f, 0,0,0,
    };

    // VAO/VBO za dirke
    unsigned int VAO, VBO;
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

    glClearColor(0.5f, 0.6f, 1.0f, 1.0f);

    // ----- BAS KRUGOVI -----
    const int BASS_SEGMENTS = 40;
    std::vector<float> bassVertices;
    bassVertices.reserve(bassCount * (BASS_SEGMENTS + 2) * 5);

    for (int i = 0; i < bassCount; i++)
    {
        float cx = bassButtons[i].cx;
        float cy = bassButtons[i].cy;
        float r = bassButtons[i].r;

        // centar kruga
        bassVertices.push_back(cx);
        bassVertices.push_back(cy);
        bassVertices.push_back(1.0f);
        bassVertices.push_back(1.0f);
        bassVertices.push_back(1.0f);

        // ivica kruga
        for (int seg = 0; seg <= BASS_SEGMENTS; seg++)
        {
            float angle = 2.0f * 3.1415926f * seg / BASS_SEGMENTS;
            float x = cx + r * std::cos(angle);
            float y = cy + r * std::sin(angle);

            bassVertices.push_back(x);
            bassVertices.push_back(y);
            bassVertices.push_back(1.0f);
            bassVertices.push_back(1.0f);
            bassVertices.push_back(1.0f);
        }
    }

    unsigned int VAObass, VBObass;
    glGenVertexArrays(1, &VAObass);
    glGenBuffers(1, &VBObass);

    glBindVertexArray(VAObass);
    glBindBuffer(GL_ARRAY_BUFFER, VBObass);
    glBufferData(GL_ARRAY_BUFFER, bassVertices.size() * sizeof(float),
        bassVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ----- OKVIR OKO BASOVA -----
    float bassFrameVertices[] = {
        // x,      y,     r,g,b (placeholder)
        -0.46f, 0.8f, 0.0f, 0.0f, 0.0f,  // gore levo
        -0.46f, 0.0f, 0.0f, 0.0f, 0.0f,  // dole levo
        -0.26f, 0.0f, 0.0f, 0.0f, 0.0f,  // dole desno
        -0.26f, 0.8f, 0.0f, 0.0f, 0.0f   // gore desno
    };

    unsigned int VAObassFrame, VBObassFrame;
    glGenVertexArrays(1, &VAObassFrame);
    glGenBuffers(1, &VBObassFrame);

    glBindVertexArray(VAObassFrame);
    glBindBuffer(GL_ARRAY_BUFFER, VBObassFrame);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bassFrameVertices), bassFrameVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ----- PLOČICA SA IMENOM (tekstura ime.png) -----
    float imeVertices[] = {
        //   x,      y,       u,   v
         0.55f, -0.80f,    0.0f, 0.0f,  // gore levo
         0.55f, -0.95f,    0.0f, 1.0f,  // dole levo
         0.95f, -0.95f,    1.0f, 1.0f,  // dole desno
         0.95f, -0.80f,    1.0f, 0.0f   // gore desno
    };

    unsigned int VAOime, VBOime;
    glGenVertexArrays(1, &VAOime);
    glGenBuffers(1, &VBOime);

    glBindVertexArray(VAOime);
    glBindBuffer(GL_ARRAY_BUFFER, VBOime);
    glBufferData(GL_ARRAY_BUFFER, sizeof(imeVertices), imeVertices, GL_STATIC_DRAW);

    // pozicija (location = 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // tex koordinate (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ----- KURSOR (tekstura cursor.png) -----
    float cursorVertices[] = {
        // inicijalno oko (0,0), kasnije menjamo u petlji
        -0.05f,  0.05f,   0.0f, 0.0f,  // gore levo
        -0.05f, -0.05f,   0.0f, 1.0f,  // dole levo
         0.05f, -0.05f,   1.0f, 1.0f,  // dole desno
         0.05f,  0.05f,   1.0f, 0.0f   // gore desno
    };

    unsigned int VAOcursor, VBOcursor;
    glGenVertexArrays(1, &VAOcursor);
    glGenBuffers(1, &VBOcursor);

    glBindVertexArray(VAOcursor);
    glBindBuffer(GL_ARRAY_BUFFER, VBOcursor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cursorVertices), cursorVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // vrati se na VAO dirki
    glBindVertexArray(VAO);

    // Učitavanje tekstura
    unsigned int imeTex = loadTexture("media/ime.png");
    unsigned int cursorTex = loadTexture("media/cursor.png");

    // ----------------- MAIN LOOP -----------------
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double delta = currentTime - previousTime;

        if (delta < FRAME_TIME) {
            continue;  // preskoči crtanje, čekamo
        }

        previousTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT);

        // --- DESNE DIRKE ---
        glUseProgram(basicShader);
        glBindVertexArray(VAO);

        int verticesPerKey = 4;

        for (int i = 0; i < notesCount; i++)
        {
            int firstVert = i * verticesPerKey;

            glDisableVertexAttribArray(1);

            if (i < 8) {
                // BELE dirke
                if (notes[i].isPressed)
                    glVertexAttrib3f(1, 0.7f, 0.7f, 0.7f);
                else
                    glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f);
            }
            else {
                // CRNE dirke
                if (notes[i].isPressed)
                    glVertexAttrib3f(1, 0.3f, 0.3f, 0.3f);
                else
                    glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
            }

            glDrawArrays(GL_TRIANGLE_FAN, firstVert, 4);

            // Border – uvek crn
            glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
            glDrawArrays(GL_LINE_LOOP, firstVert, 4);

            glEnableVertexAttribArray(1);
        }

        // --- POZADINA BAS SEKCIJE (crni pravougaonik) ---
        glBindVertexArray(VAObassFrame);
        glDisableVertexAttribArray(1);

        glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);   // crno popunjavanje
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);     // ispunjeni pravougaonik

        // --- BAS dugmići (krugovi) ---
        glBindVertexArray(VAObass);
        glDisableVertexAttribArray(1);

        int vertsPerBass = BASS_SEGMENTS + 2; // centar + svi segmenti

        for (int i = 0; i < bassCount; i++)
        {
            int firstVert = i * vertsPerBass;

            if (bassButtons[i].isPressed)
                glVertexAttrib3f(1, 0.7f, 0.7f, 0.7f); // osenčen
            else
                glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f); // belo

            glDrawArrays(GL_TRIANGLE_FAN, firstVert, vertsPerBass);

            // crni okvir po obodu
            glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
            glDrawArrays(GL_LINE_LOOP, firstVert + 1, vertsPerBass - 1);
        }

        // --- OKVIR OKO BASOVA ---
        glBindVertexArray(VAObassFrame);
        glDisableVertexAttribArray(1);

        glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f); // crna linija
        glDrawArrays(GL_LINE_LOOP, 0, 4);

        glEnableVertexAttribArray(1);

        // --- PLOČICA SA IMENOM (tekstura ime.png) ---
        glUseProgram(rectShader);
        glBindVertexArray(VAOime);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, imeTex);
        glUniform1i(glGetUniformLocation(rectShader, "uTex"), 0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // --- KURSOR (tekstura cursor.png) ---
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        int winW, winH;
        glfwGetWindowSize(window, &winW, &winH);

        float ndcX = 2.0f * static_cast<float>(mx) / static_cast<float>(winW) - 1.0f;
        float ndcY = 1.0f - 2.0f * static_cast<float>(my) / static_cast<float>(winH);

        float hw = 0.05f;  // pola širine kursora
        float hh = 0.05f;  // pola visine kursora

        float cursorVerticesUpdated[] = {
            ndcX - hw, ndcY + hh, 0.0f, 0.0f,  // gore levo
            ndcX - hw, ndcY - hh, 0.0f, 1.0f,  // dole levo
            ndcX + hw, ndcY - hh, 1.0f, 1.0f,  // dole desno
            ndcX + hw, ndcY + hh, 1.0f, 0.0f   // gore desno
        };

        glBindVertexArray(VAOcursor);
        glBindBuffer(GL_ARRAY_BUFFER, VBOcursor);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cursorVerticesUpdated), cursorVerticesUpdated);

        glUseProgram(rectShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cursorTex);
        glUniform1i(glGetUniformLocation(rectShader, "uTex"), 0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // ------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    engine->drop();
    return 0;
}
