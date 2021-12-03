//
// Created by alberto on 11/20/21.
//

#ifndef MUGEN_CPP_WAVEFORMMENU_H
#define MUGEN_CPP_WAVEFORMMENU_H

#include "Texture.h"
#include "SelectControl.h"
#include "Direction.h"
#include "CheckboxControl.h"
#include <vector>

class MainWindow;

class WaveformMenu {
public:
    WaveformMenu(MainWindow* mainWindow);
    ~WaveformMenu();
    void init();
    void render();

    void setTextTexture(Texture* texture, std::string text);

    void selectFocusedControl();

    SDL_Renderer* renderer;
//private:
    std::string helpMessage;
    int currentOsc = 0, maxOsc = 0;
    double currentFreqMod = 1.0, currentAmplitude = 1.0;
    SDL_Color textColor;
    SelectControl oscSelector, mainWaveSelector, mainFreqSelector, mainAmpSelector,
                    lfoWaveSelector, lfoFreqSelector, lfoAmpSelector;
    CheckboxControl lfoCheckBox;
    const static int controlMatrixRows = 3, controlMatrixCols = 4;
    Control* controlArray[controlMatrixRows][controlMatrixCols] = {
            {nullptr,       nullptr,           &oscSelector,      nullptr},
            {nullptr,       &mainWaveSelector, &mainFreqSelector, &mainAmpSelector},
            {&lfoCheckBox,  &lfoWaveSelector,  &lfoFreqSelector,  &lfoAmpSelector}
    };
    std::vector<Texture*> waveImages {&sineImg, &squareImg, &triangleImg, &sawDownImg, &noiseImg};
    int focusedControlCol = 2;
    int focusedControlRow = 0;
    Texture screenTitle, oscCounter,
        arrowImg, checkImg,
        sineImg, squareImg, triangleImg,
        sawDownImg, sawUpImg, noiseImg,
        wavetypeLabel, mainWaveSign, lfoWaveSign,
        frequencyLabel, mainFreqValue, lfoFreqValue,
        amplitudeLabel, mainAmpValue, lfoAmpValue,
        lfoLabel,
        helpBar,
        undefinedLabel,
        blocksLeftLabel;
    MusicBox* musicBox;
    MainWindow* window;
    Instrument* editedInstrument;
    Oscillator* editedOsc;

    static std::string doubleToStr(double d, int precision);


    void setImageTexture(Texture *texture, std::string imagePath);
    void loadTextures();
    void handleKeyPress(SDL_Keycode key);
    void updateTextures();
    void loadControls();
    void setTextTexture(Texture *texture, std::string text, TTF_Font *font);
    void setWaveImage(SelectControl* control, WaveformType wavetype);
    void changeControlFocus(Direction direction);
    Control* getFocusedControl();

    int xByPercent(Texture *texture, double percent);

    int yByPercent(Texture *texture, double percent);

    void setLFO();

    void nextOsc(bool reverse = false);
};

#endif //MUGEN_CPP_WAVEFORMMENU_H
