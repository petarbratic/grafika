#pragma once

#include <irrKlang.h>
#include "KeyData.h"

namespace Audio {

    // Globalni engine za ceo program (inicijalizuje se i gasi u Audio.cpp)
    extern irrklang::ISoundEngine* engine;

    bool init();     // createIrrKlangDevice
    void shutdown(); // drop()

    // Obrada pritiska/tastera (poziva se iz key callback-a)
    void handleKey(int key, int action);

} // namespace Audio
