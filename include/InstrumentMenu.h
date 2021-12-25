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

class InstrumentMenu {
public:
    bool showDebug = false;
    InstrumentMenu(MainWindow* mainWindow);
    ~InstrumentMenu();
    void init();
    void render();

    void setTextTexture(Texture* texture, std::string text);

    void selectFocusedControl();

    SDL_Renderer* renderer;
//private:
    std::string helpMessage;
    int currentOsc = 0, maxOsc = 0;

    SDL_Color textColor;
    SelectControl oscSelector,
        mainWaveSelector, mainFreqSelector, mainAmpSelector,
        lfoWaveSelector, lfoFreqSelector, lfoAmpSelector,
        envStartingSelector, envAttackSelector, envDecaySelector, envSustainSelector, envReleaseSelector;
    CheckboxControl lfoCheckBox;
    const static int controlMatrixRows = 6, controlMatrixCols = 5;
    Control* controlArray[controlMatrixRows][controlMatrixCols] = {
            {nullptr,       nullptr,           &oscSelector,      nullptr,          nullptr},
            {nullptr,       &mainWaveSelector, &mainFreqSelector, &mainAmpSelector, &envStartingSelector},
            {&lfoCheckBox,  &lfoWaveSelector,  &lfoFreqSelector,  &lfoAmpSelector,  &envSustainSelector},
            {nullptr,       nullptr,           nullptr,           nullptr,          &envAttackSelector},
            {nullptr,       nullptr,           nullptr,           nullptr,          &envDecaySelector},
            {nullptr,       nullptr,           nullptr,           nullptr,          &envReleaseSelector}
    };
    std::vector<Texture*> waveImages {&sineImg, &squareImg, &triangleImg, &sawDownImg, &noiseImg};
    int focusedControlCol = 2;
    int focusedControlRow = 0;
    Texture screenTitle,
    // waveform creator textures
        oscCounter,
        sineImg, squareImg, triangleImg,
        sawDownImg, sawUpImg, noiseImg,
        wavetypeLabel, mainWaveSign, lfoWaveSign,
        frequencyLabel, mainFreqValue, lfoFreqValue,
        amplitudeLabel, mainAmpValue, lfoAmpValue,
        lfoLabel,
    // envelope creator textures
        envelopeLabel,
        durationsLabel, levelsLabel,
        startValue, attackValue, decayValue, sustainValue, releaseValue,
        startLabel, attackLabel, decayLabel, sustainLabel, releaseLabel,
    // common textures
        arrowImg, checkImg,
        helpBar,
        undefinedLabel,
    // debug textures
        debugBlocksLeftLabel, debugMaxSampleLabel, debugCurrentFrequencyLabel, debugEnvelopeMomentLabel;
    MusicBox* musicBox;
    MainWindow* window;
    Instrument* editedInstrument;
    Oscillator* editedOsc;

    static std::string doubleToStr(double d, int precision);

    bool drawIntegral = false;
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
