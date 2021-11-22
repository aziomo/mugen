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

    void focusNextControl();
    void setTextTexture(Texture* texture, std::string text);
    void focusPrevControl();
    void chooseFocusedControl();

    SDL_Renderer* renderer;
//private:
    int currentOsc = 0, maxOsc = 0;
    double currentFreqMod = 1.0, currentAmplitude = 1.0;
    SDL_Color textColor;
    SelectControl wavetypeSelector, frequencySelector, amplitudeSelector;
    std::vector<SelectControl*> controls {&wavetypeSelector, &frequencySelector, &amplitudeSelector};
    int focusedSelectorIndex = 0;
    Texture screenTitle, oscCounter, arrow,
        sineImage, squareImage, triangleImage,
        sawdownImage, sawupImage, noiseImage,
        wavetypeLabel, wavetypeSign,
        frequencyLabel, frequencyValue,
        amplitudeLabel, amplitudeValue;

    MainWindow* window;
    std::vector<Texture> textures;



    static std::string doubleToStr(double d, int precision);


    void setImageTexture(Texture *texture, std::string imagePath);

    void loadTextures();

    void handleKeyPress(SDL_Keycode key);
};

#endif //MUGEN_CPP_WAVEFORMMENU_H
