#include "KeyData.h"
#include <GLFW/glfw3.h>

// Desne dirke
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

const int notesCount = sizeof(notes) / sizeof(notes[0]);

// Bas dugmad
BassButton bassButtons[] = {
    { GLFW_KEY_U, "semplHarmonika/Cbas.wav", nullptr, false, -0.4f, 0.4f,   0.04f },
    { GLFW_KEY_7, "semplHarmonika/Cdur.wav", nullptr, false, -0.32f, 0.465f, 0.04f },
    { GLFW_KEY_Y, "semplHarmonika/Gbas.wav", nullptr, false, -0.4f,  0.5f,   0.04f },

    { GLFW_KEY_6, "semplHarmonika/Gdur.wav", nullptr, false, -0.32f, 0.565f, 0.04f },
    { GLFW_KEY_I, "semplHarmonika/Fbas.wav", nullptr, false, -0.4f,  0.3f,   0.04f },
    { GLFW_KEY_8, "semplHarmonika/Fdur.wav", nullptr, false, -0.32f, 0.365f, 0.04f },
};

const int bassCount = sizeof(bassButtons) / sizeof(bassButtons[0]);
