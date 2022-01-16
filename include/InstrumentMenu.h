//
// Created by alberto on 11/20/21.
//

#ifndef MUGEN_CPP_WAVEFORMMENU_H
#define MUGEN_CPP_WAVEFORMMENU_H

#include "Texture.h"
#include "SelectControl.h"
#include "Direction.h"
#include "CheckboxControl.h"
#include "ItemList.h"
#include <vector>

class MainWindow;

class InstrumentMenu {
public:
    bool showDebug = false;
    InstrumentMenu(MainWindow* mainWindow);
    ~InstrumentMenu();
    void init();
    void render();

    void setTextTexture(Texture* texture, const std::string& text) const;

    void selectFocusedControl();

    SDL_Renderer* renderer;
//private:
    std::string helpMessage;
    int currentOsc = 0;

    SDL_Color textColor;

    ItemList* instrumentList;
    bool isInstrumentListFocused = false;

    SelectControl oscSelector,
        mainWaveSelector, mainFreqSelector, mainAmpSelector,
        lfoWaveSelector, lfoFreqSelector, lfoAmpSelector,
        envInitialSelector, envAttackSelector, envDecaySelector, envSustainSelector, envReleaseSelector;
    CheckboxControl lfoCheckBox;
    const static int controlMatrixRows = 6, controlMatrixCols = 5;
    Control* controlArray[controlMatrixRows][controlMatrixCols] = {
            {nullptr,       nullptr,           &oscSelector,      nullptr,          nullptr},
            {nullptr,       &mainWaveSelector, &mainFreqSelector, &mainAmpSelector, &envInitialSelector},
            {&lfoCheckBox,  &lfoWaveSelector,  &lfoFreqSelector,  &lfoAmpSelector,  &envSustainSelector},
            {nullptr,       nullptr,           nullptr,           nullptr,          &envAttackSelector},
            {nullptr,       nullptr,           nullptr,           nullptr,          &envDecaySelector},
            {nullptr,       nullptr,           nullptr,           nullptr,          &envReleaseSelector}
    };
    vector<Texture*> waveImages {&sineImg, &squareImg, &triangleImg, &sawDownImg, &noiseImg};
    int focusedControlCol = 2;
    int focusedControlRow = 0;
    Texture instrumentName,
    // waveform creator textures
        oscLabel, oscCounter,
        wavetypeLabel,
        sineImg, squareImg, triangleImg,
        sawDownImg, sawUpImg, noiseImg,
        frequencyLabel, mainFreqValue, lfoFreqValue,
        amplitudeLabel, mainAmpValue, lfoAmpValue,
        lfoLabel,
    // envelope creator textures
        envelopeLabel,
        durationsLabel, levelsLabel,
        initialValue, attackValue, decayValue, sustainValue, releaseValue,
        initialLabel, attackLabel, decayLabel, sustainLabel, releaseLabel,
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
    void setImageTexture(Texture *texture, std::string imagePath) const;
    void loadTextures();
    void handleKeyPress(SDL_Keycode key);
    void updateTextures();
    void loadControls();
    void setTextTexture(Texture *texture, const std::string& text, TTF_Font *font) const;
    void setWaveImage(SelectControl* control, WaveformType wavetype);
    void changeControlFocus(Direction direction);
    Control* getFocusedControl();

    void setLFO();


    void selectPrevOsc();
    void selectNextOsc();

    void updateSelectorValues();

    int xByPercent(Texture *texture, double percent, Alignment align = CENTER) const;
    int xByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;
    int yByPercent(Texture *texture, double percent, Alignment align = CENTER) const;
    int yByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;

    void selectItemFromList(int index);

    void addInstrument(Instrument *instrument, int index);
};

#endif //MUGEN_CPP_WAVEFORMMENU_H
