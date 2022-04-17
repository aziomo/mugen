#include "../include/SelectControl.h"
#include "../include/InstrumentMenu.h"
#include "../include/CompositionMenu.h"

SelectControl::~SelectControl(){
    p_rend = nullptr;
    while (!optionsImages.empty()){
        optionsImages.pop_back();
    }
    p_arrowTexture = nullptr;
    mainTexture = nullptr;
}

void SelectControl::setModifiedDouble(double* modifiedDouble){
    p_modifiedDouble = modifiedDouble;
}

void SelectControl::setModifiedInteger(int* modifiedInt){
    p_modifiedInteger = modifiedInt;
}

void SelectControl::switchEditing(){
    isEditing = !isEditing;
}

void SelectControl::isModifyingLFO(bool isModifying){
    modifyLFO = isModifying;
}

void SelectControl::loadTextControl(SelectorType selectorType, Texture* textTexture, MainWindow* window) {

    p_instrumentMenu = window->instrumentMenu;
    p_compositionMenu = window->compositionMenu;
    m_type = selectorType;
    p_rend = window->renderer;
    p_arrowTexture = &window->instrumentMenu->arrowImg;
    mainTexture = textTexture;
}

void SelectControl::loadImageControl(vector<Texture*> imageTextures, MainWindow* window) {
    p_instrumentMenu = window->instrumentMenu;
    m_type = SelectorType::WAVETYPE;
    p_rend = window->renderer;
    this->p_arrowTexture = &window->instrumentMenu->arrowImg;
    optionsImages = std::move(imageTextures);
    mainTexture = optionsImages[0];
}

void SelectControl::render(int x, int y) {
    if (isHighlighted){
        highlightRect = {x - borderSize*2, y - borderSize*2,
                         mainTexture->w + borderSize * 4,
                         mainTexture->h + borderSize * 4};
        bgRect = {x - borderSize, y - borderSize, mainTexture->w + borderSize * 2, mainTexture->h + borderSize * 2};
        SDL_SetRenderDrawColor(p_rend, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(p_rend, &highlightRect);
        SDL_SetRenderDrawColor(p_rend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(p_rend, &bgRect);
    }

    if (isEditing){
        p_arrowTexture->renderRotated(x - p_arrowTexture->w,
                             y + (mainTexture->h - p_arrowTexture->h) / 2, 270);
        p_arrowTexture->renderRotated(x + mainTexture->w,
                             y + (mainTexture->h - p_arrowTexture->h) / 2, 90);
    }
    mainTexture->render(x,y);
}

void SelectControl::increment(bool largeIncrement) {
    switch(m_type){
        case SelectorType::FUNCTION:
            (p_compositionMenu->*p_incFunc)();
            break;
        case SelectorType::OSCILLATOR:
            p_instrumentMenu->selectNextOsc();
            break;
        case SelectorType::WAVETYPE:
            setNextWaveType(); break;
        case SelectorType::INTEGER:
            *p_modifiedInteger += 1;
            break;
        case SelectorType::FINEDOUBLE:
            *p_modifiedDouble += largeIncrement ? 0.1 : 0.01; break;
        case SelectorType::DOUBLE:
            *p_modifiedDouble += 0.1; break;
    }
}

void SelectControl::decrement(bool largeDecrement) {
    switch(m_type){
        case SelectorType::FUNCTION:
            (p_compositionMenu->*p_decFunc)();
            break;
        case SelectorType::OSCILLATOR:
            p_instrumentMenu->selectPrevOsc();
            break;
        case SelectorType::WAVETYPE:
            setNextWaveType(false); break;
        case SelectorType::FINEDOUBLE:
            *p_modifiedDouble -= largeDecrement ? 0.1 : 0.01;
            if (*p_modifiedDouble < 0)
                *p_modifiedDouble = 0;
            break;
        case SelectorType::DOUBLE:
            *p_modifiedDouble -= 0.1;
            if (*p_modifiedDouble < 0)
                *p_modifiedDouble = 0;
            break;
        case SelectorType::INTEGER:
            if (*p_modifiedInteger > 0)
                *p_modifiedInteger -= 1;
            break;
    }
}

void SelectControl::setNextWaveType(bool increment){
    WaveformType previousWave = modifyLFO ? p_instrumentMenu->editedOsc->lfo->waveType : p_instrumentMenu->editedOsc->waveType;
    WaveformType nextWave;

    switch (previousWave) {
        case WaveformType::SINE:
            nextWave = increment ? WaveformType::SQUARE : WaveformType::NOISE; break;
        case WaveformType::SQUARE:
            nextWave = increment ? WaveformType::TRIANGLE : WaveformType::SINE; break;
        case WaveformType::TRIANGLE:
            nextWave = increment ? WaveformType::SAWTOOTHDOWN : WaveformType::SQUARE; break;
        case WaveformType::SAWTOOTHDOWN:
            nextWave = increment ? WaveformType::SAWTOOTHUP : WaveformType::TRIANGLE; break;
        case WaveformType::SAWTOOTHUP:
            nextWave = increment ? WaveformType::NOISE : WaveformType::SAWTOOTHDOWN; break;
        case WaveformType::NOISE:
            nextWave = increment ? WaveformType::SINE : WaveformType::SAWTOOTHUP; break;
    }

    if (modifyLFO){
        p_instrumentMenu->editedOsc->lfo->setWaveformType(nextWave);
        p_instrumentMenu->editedOsc->setupLfoLookup(p_instrumentMenu->editedOsc->lfo->waveType);
    } else {
        p_instrumentMenu->editedOsc->setWaveformType(nextWave);
    }
}

void SelectControl::activate() {
    if (!modifyLFO || p_instrumentMenu->lfoCheckBox.isChecked){
        switchHighlight();
        switchEditing();
    }
}

void SelectControl::setIncrementFunction(void (CompositionMenu::*function)()) {
    p_incFunc = function;
}

void SelectControl::setDecrementFunction(void (CompositionMenu::*function)()) {
    p_decFunc = function;
}