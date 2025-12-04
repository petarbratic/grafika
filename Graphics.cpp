#include "Graphics.h"

#include <vector>
#include <cmath>

#include "Util.h"
#include "KeyData.h"

// --------------------------------------
// Konstante za frame timing
// --------------------------------------
static const double FRAME_TIME = 1.0 / 75.0;

// --------------------------------------
// Šejderi
// --------------------------------------
static unsigned int basicShader = 0;
static unsigned int rectShader = 0;

// --------------------------------------
// VAO/VBO za dirke (desna ruka)
// --------------------------------------
static unsigned int VAOkeys = 0;
static unsigned int VBOkeys = 0;

// --------------------------------------
// Bas dugmići (leva ruka)
// --------------------------------------
static const int BASS_SEGMENTS = 40;
static unsigned int VAObass = 0;
static unsigned int VBObass = 0;
static unsigned int VAObassFrame = 0;
static unsigned int VBObassFrame = 0;

// --------------------------------------
// Ime pločica (tekstura ime.png)
// --------------------------------------
static unsigned int VAOime = 0;
static unsigned int VBOime = 0;
static unsigned int imeTex = 0;

// --------------------------------------
// Kursor (tekstura cursor.png)
// --------------------------------------
static unsigned int VAOcursor = 0;
static unsigned int VBOcursor = 0;
static unsigned int cursorTex = 0;

// --------------------------------------
// Pomeraj leve strane (basa)
// --------------------------------------
static float g_leftOffsetX = 0.0f;


// Meh – da li radi i u kom smeru
static bool  g_bellowsActive = false;
static float g_bellowDir = 1.0f;   // +1 desno, -1 levo
static const float BELLOW_SPEED = 0.2f; // brzina meha (ti menjaš kasnije)
static const float MAX_SHIFT_RIGHT = 0.40f;   // desno (napolje) – može veće
static const float MAX_SHIFT_LEFT = 0.0f;  // levo (unutra) – manje, po tvojoj želji

// -------------------------------------------------
// Pomoćne funkcije za inicijalizaciju geometrije
// -------------------------------------------------

// Napred deklaracija da bismo mogli da zovemo funkciju pre definicije
static void updateBassBuffers();

// Desne dirke
static void initKeysGeometry() {
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

    glGenVertexArrays(1, &VAOkeys);
    glGenBuffers(1, &VBOkeys);

    glBindVertexArray(VAOkeys);
    glBindBuffer(GL_ARRAY_BUFFER, VBOkeys);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // pozicija
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // boja
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

// Bas dugmići: kreiramo VAO/VBO i prvi put punimo podatke
static void initBassGeometry() {
    // Bas dugmići
    glGenVertexArrays(1, &VAObass);
    glGenBuffers(1, &VBObass);

    glBindVertexArray(VAObass);
    glBindBuffer(GL_ARRAY_BUFFER, VBObass);

    // Podaci će se puniti u updateBassBuffers()
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Okvir oko bas sekcije
    glGenVertexArrays(1, &VAObassFrame);
    glGenBuffers(1, &VBObassFrame);

    glBindVertexArray(VAObassFrame);
    glBindBuffer(GL_ARRAY_BUFFER, VBObassFrame);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Prvi put upiši podatke
    updateBassBuffers();
}

// Pločica sa imenom
static void initImeGeometry() {
    float imeVertices[] = {
         0.55f, -0.80f,    0.0f, 0.0f,
         0.55f, -0.95f,    0.0f, 1.0f,
         0.95f, -0.95f,    1.0f, 1.0f,
         0.95f, -0.80f,    1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAOime);
    glGenBuffers(1, &VBOime);

    glBindVertexArray(VAOime);
    glBindBuffer(GL_ARRAY_BUFFER, VBOime);
    glBufferData(GL_ARRAY_BUFFER, sizeof(imeVertices), imeVertices, GL_STATIC_DRAW);

    // pozicija
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // tex
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

// Kursor
static void initCursorGeometry() {
    float cursorVertices[] = {
        -0.05f,  0.05f,   0.0f, 0.0f,
        -0.05f, -0.05f,   0.0f, 1.0f,
         0.05f, -0.05f,   1.0f, 1.0f,
         0.05f,  0.05f,   1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAOcursor);
    glGenBuffers(1, &VBOcursor);

    glBindVertexArray(VAOcursor);
    glBindBuffer(GL_ARRAY_BUFFER, VBOcursor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cursorVertices), cursorVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

// --------------------------------------
// updateBassBuffers – koristi g_leftOffsetX
// --------------------------------------
static void updateBassBuffers()
{
    // --- bas dugmići ---
    std::vector<float> bassVertices;
    bassVertices.reserve(bassCount * (BASS_SEGMENTS + 2) * 5);

    for (int i = 0; i < bassCount; i++) {
        float cx = bassButtons[i].cx + g_leftOffsetX;
        float cy = bassButtons[i].cy;
        float r = bassButtons[i].r;

        // centar kruga
        bassVertices.push_back(cx);
        bassVertices.push_back(cy);
        bassVertices.push_back(1.0f);
        bassVertices.push_back(1.0f);
        bassVertices.push_back(1.0f);

        // ivica kruga
        for (int seg = 0; seg <= BASS_SEGMENTS; seg++) {
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

    glBindVertexArray(VAObass);
    glBindBuffer(GL_ARRAY_BUFFER, VBObass);
    glBufferData(GL_ARRAY_BUFFER,
        bassVertices.size() * sizeof(float),
        bassVertices.data(),
        GL_DYNAMIC_DRAW);

    // --- okvir oko basova ---
    float bassFrameVertices[] = {
        -0.46f + g_leftOffsetX, 0.8f, 0.0f, 0.0f, 0.0f,
        -0.46f + g_leftOffsetX, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.26f + g_leftOffsetX, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.26f + g_leftOffsetX, 0.8f, 0.0f, 0.0f, 0.0f
    };

    glBindVertexArray(VAObassFrame);
    glBindBuffer(GL_ARRAY_BUFFER, VBObassFrame);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(bassFrameVertices),
        bassFrameVertices,
        GL_DYNAMIC_DRAW);
}

// -------------------------------------------------
// API implementacija
// -------------------------------------------------

bool Graphics::init() {
    basicShader = createShader("basic.vert", "basic.frag");
    rectShader = createShader("rect.vert", "rect.frag");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.5f, 0.6f, 1.0f, 1.0f);

    initKeysGeometry();
    initBassGeometry();
    initImeGeometry();
    initCursorGeometry();

    imeTex = loadTexture("media/ime.png");
    cursorTex = loadTexture("media/cursor.png");

    return true;
}

void Graphics::renderFrame(GLFWwindow* window, double& previousTime) {
    double currentTime = glfwGetTime();
    double delta = currentTime - previousTime;

    if (delta < FRAME_TIME) {
        return; // frame limiter
    }
    previousTime = currentTime;

   
    glClear(GL_COLOR_BUFFER_BIT);


    // Automatsko pomeranje meha dok su dirke/basovi pritisnuti
    if (g_bellowsActive) {
        g_leftOffsetX += g_bellowDir * BELLOW_SPEED * static_cast<float>(delta);

        // ako pređe desnu granicu – vraćaj ulevo
        if (g_leftOffsetX > MAX_SHIFT_RIGHT) {
            g_leftOffsetX = MAX_SHIFT_RIGHT;
            g_bellowDir = -1.0f;
        }
        // ako pređe levu granicu – guraj udesno
        else if (g_leftOffsetX < MAX_SHIFT_LEFT) {
            g_leftOffsetX = MAX_SHIFT_LEFT;
            g_bellowDir = 1.0f;
        }
    }


    // ---------------- DESNE DIRKE ----------------
    glUseProgram(basicShader);
    glBindVertexArray(VAOkeys);

    int verticesPerKey = 4;

    for (int i = 0; i < notesCount; i++) {
        int firstVert = i * verticesPerKey;

        glDisableVertexAttribArray(1);

        if (i < 8) {
            // bele
            if (notes[i].isPressed)
                glVertexAttrib3f(1, 0.7f, 0.7f, 0.7f);
            else
                glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f);
        }
        else {
            // crne
            if (notes[i].isPressed)
                glVertexAttrib3f(1, 0.3f, 0.3f, 0.3f);
            else
                glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
        }

        glDrawArrays(GL_TRIANGLE_FAN, firstVert, 4);

        // border
        glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
        glDrawArrays(GL_LINE_LOOP, firstVert, 4);

        glEnableVertexAttribArray(1);
    }

    // ---------------- BAS SEKCIJA ----------------
    // pre crtanja osveži pozicije basova prema g_leftOffsetX
    updateBassBuffers();

    // pozadina
    glBindVertexArray(VAObassFrame);
    glDisableVertexAttribArray(1);

    glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // bas dugmići
    glBindVertexArray(VAObass);
    glDisableVertexAttribArray(1);

    int vertsPerBass = BASS_SEGMENTS + 2;

    for (int i = 0; i < bassCount; i++) {
        int firstVert = i * vertsPerBass;

        if (bassButtons[i].isPressed)
            glVertexAttrib3f(1, 0.7f, 0.7f, 0.7f);
        else
            glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f);

        glDrawArrays(GL_TRIANGLE_FAN, firstVert, vertsPerBass);

        // crni okvir
        glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
        glDrawArrays(GL_LINE_LOOP, firstVert + 1, vertsPerBass - 1);
    }

    // okvir oko basova
    glBindVertexArray(VAObassFrame);
    glDisableVertexAttribArray(1);
    glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glEnableVertexAttribArray(1);

    // ---------------- PLOČICA SA IMENOM ----------------
    glUseProgram(rectShader);
    glBindVertexArray(VAOime);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, imeTex);
    glUniform1i(glGetUniformLocation(rectShader, "uTex"), 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // ---------------- KURSOR ----------------
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    int winW, winH;
    glfwGetWindowSize(window, &winW, &winH);

    float ndcX = 2.0f * static_cast<float>(mx) / static_cast<float>(winW) - 1.0f;
    float ndcY = 1.0f - 2.0f * static_cast<float>(my) / static_cast<float>(winH);

    float hw = 0.05f;
    float hh = 0.05f;

    float cursorVerticesUpdated[] = {
        ndcX - hw, ndcY + hh, 0.0f, 0.0f,
        ndcX - hw, ndcY - hh, 0.0f, 1.0f,
        ndcX + hw, ndcY - hh, 1.0f, 1.0f,
        ndcX + hw, ndcY + hh, 1.0f, 0.0f
    };

    glBindVertexArray(VAOcursor);
    glBindBuffer(GL_ARRAY_BUFFER, VBOcursor);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cursorVerticesUpdated), cursorVerticesUpdated);

    glUseProgram(rectShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cursorTex);
    glUniform1i(glGetUniformLocation(rectShader, "uTex"), 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Graphics::shutdown() {
    glDeleteVertexArrays(1, &VAOkeys);
    glDeleteBuffers(1, &VBOkeys);

    glDeleteVertexArrays(1, &VAObass);
    glDeleteBuffers(1, &VBObass);

    glDeleteVertexArrays(1, &VAObassFrame);
    glDeleteBuffers(1, &VBObassFrame);

    glDeleteVertexArrays(1, &VAOime);
    glDeleteBuffers(1, &VBOime);

    glDeleteVertexArrays(1, &VAOcursor);
    glDeleteBuffers(1, &VBOcursor);

    glDeleteProgram(basicShader);
    glDeleteProgram(rectShader);

    glDeleteTextures(1, &imeTex);
    glDeleteTextures(1, &cursorTex);
}

void Graphics::moveLeftSide(float dx)
{
    g_leftOffsetX += dx;

    if (g_leftOffsetX > MAX_SHIFT_RIGHT)
        g_leftOffsetX = MAX_SHIFT_RIGHT;

    if (g_leftOffsetX < MAX_SHIFT_LEFT)
        g_leftOffsetX = MAX_SHIFT_LEFT;
}

void Graphics::setBellowsActive(bool active)
{
    g_bellowsActive = active;
}
