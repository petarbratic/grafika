#include "Audio.h"
#include <GLFW/glfw3.h>
#include <stdio.h>



namespace Audio {

    ISoundEngine* engine = nullptr;

    bool init() {
        engine = createIrrKlangDevice();
        if (!engine) {
            printf("Greska: irrKlang engine nije mogao da se kreira.\n");
            return false;
        }
        return true;
    }

    void shutdown() {
        if (engine) {
            engine->drop();
            engine = nullptr;
        }
    }

    // Pomoćne funkcije – odvojeno za bas i desne dirke
    void handleBassKey(int key, int action) {
        for (int i = 0; i < bassCount; i++) {
            if (bassButtons[i].key == key) {
                if (action == GLFW_PRESS) {
                    bassButtons[i].isPressed = true;
                    if (!bassButtons[i].sound && engine) {
                        bassButtons[i].sound = engine->play2D(
                            bassButtons[i].filePath,
                            true,   // loop
                            false,  // startPaused
                            true    // track za ISound*
                        );
                    }
                }
                else if (action == GLFW_RELEASE) {
                    bassButtons[i].isPressed = false;
                    if (bassButtons[i].sound) {
                        bassButtons[i].sound->stop();
                        bassButtons[i].sound->drop();
                        bassButtons[i].sound = nullptr;
                    }
                }
                return; // pogodili smo neko bas dugme
            }
        }
    }

    void handleNoteKey(int key, int action) {
        for (int i = 0; i < notesCount; i++) {
            if (notes[i].key == key) {
                if (action == GLFW_PRESS) {
                    notes[i].isPressed = true;
                    if (!notes[i].sound && engine) {
                        notes[i].sound = engine->play2D(
                            notes[i].filePath,
                            true,
                            false,
                            true
                        );
                    }
                }
                else if (action == GLFW_RELEASE) {
                    notes[i].isPressed = false;
                    if (notes[i].sound) {
                        notes[i].sound->stop();
                        notes[i].sound->drop();
                        notes[i].sound = nullptr;
                    }
                }
                return;
            }
        }
    }

    void handleKey(int key, int action) {
        // prvo probaj da ga nađeš među bas dugmićima
        handleBassKey(key, action);
        // pa među desnim dirkama
        handleNoteKey(key, action);
    }

} // namespace Audio
