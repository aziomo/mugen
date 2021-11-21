//
// Created by alberto on 11/20/21.
//

#ifndef MUGEN_CPP_WAVEFORMMENU_H
#define MUGEN_CPP_WAVEFORMMENU_H

#include "Texture.h"
#include "SelectControl.h"
#include <vector>

class MainWindow;



class WaveformMenu {
public:
    WaveformMenu(MainWindow* mainWindow);
    void init();
    void render();

    void highlightShit();

    void setFocusedControl();

//private:
    int currentOsc = 0, maxOsc = 0;
    double currentFrequency = 440.000, currentAmplitude = 0.5;
    SDL_Color textColor;
    SelectControl wavetypeSelector, frequencySelector, amplitudeSelector;
    std::vector<SelectControl*> controls {&wavetypeSelector, &frequencySelector, &amplitudeSelector};
    int focusedSelectorIndex = 0;
    Texture screenTitle, oscCounter,
        downArrow, upArrow,
        wavetypeLabel, wavetypeSign,
        sineImage, squareImage, triangleImage, sawdownImage, sawupImage, noiseImage,
        frequencyLabel, frequencyValue, amplitudeLabel, amplitudeValue;

    MainWindow* window;
    std::vector<Texture> textures;

    void setTextLabel(Texture* texture, std::string text);


    std::string doubleToStr(double d, int precision);

    void focusNextControl();

    void focusPrevControl();
};

#endif //MUGEN_CPP_WAVEFORMMENU_H
