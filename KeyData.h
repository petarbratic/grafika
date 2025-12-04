#pragma once

#include <irrKlang.h>
using namespace irrklang;

// Jedna desna dirka
struct KeyNote {
    int key;                // GLFW key code
    const char* filePath;   // putanja do wav/ogg fajla
    irrklang::ISound* sound; // trenutno aktivni zvuk (ili nullptr)
    bool isPressed;         // vizuelni feedback
};

// Jedno bas dugme
struct BassButton {
    int key;               // GLFW key code za bas
    const char* filePath;  // putanja do wav fajla
    irrklang::ISound* sound; // aktivni zvuk
    bool isPressed;        // da li je pritisnut
    float cx, cy;          // centar kruga
    float r;               // poluprečnik
};

// Deklaracije nizova i brojeva elemenata
extern KeyNote   notes[];
extern const int notesCount;

extern BassButton bassButtons[];
extern const int  bassCount;
