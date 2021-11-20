//
// Created by alberto on 11/20/21.
//

#ifndef MUGEN_CPP_WAVEFORMMENU_H
#define MUGEN_CPP_WAVEFORMMENU_H

#include "Texture.h"
#include <vector>

class MainWindow;

class WaveformMenu {
public:
    WaveformMenu(MainWindow* mainWindow);
    void init();
    void render();

private:
    int currentOsc = 0, maxOsc = 0;
    SDL_Color textColor;
    Texture screenTitle, upArrow,
        downArrow, oscCounter,
        wavetypeLabel, wavetypeSign,
        frequencyLabel, frequencyValue,
        amplitudeLabel, amplitudeValue;

    MainWindow* window;
    std::vector<Texture> textures;

    void setTextLabel(Texture* texture, std::string text);
};

#endif //MUGEN_CPP_WAVEFORMMENU_H
