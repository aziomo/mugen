//
// Created by alberto on 11/21/21.
//

#include <sstream>
#include "../include/SelectControl.h"
#include "../include/InstrumentMenu.h"

SelectControl::~SelectControl(){
    rend = nullptr;
    while (!optionsImages.empty()){
        optionsImages.pop_back();
    }
    arrowTexture = nullptr;
    mainTexture = nullptr;
}

void SelectControl::setModifiedDouble(double* modifiedDouble){
    this->modifiedDouble = modifiedDouble;
}

void SelectControl::setModifiedInteger(int* modifiedInt){
    this->modifiedInteger = modifiedInt;
}

void SelectControl::switchEditing(){
    isEditing = !isEditing;
}

void SelectControl::isModifyingLFO(bool isModifying){
    modifyLFO = isModifying;
}

void SelectControl::loadTextControl(SelectorType selectorType, Texture* textTexture, MainWindow* window) {
    menu = window->instrumentMenu;
    this->type = selectorType;
    this->rend = window->renderer;
    this->arrowTexture = &window->instrumentMenu->arrowImg;
    mainTexture = textTexture;
}

void SelectControl::loadImageControl(vector<Texture*> imageTextures, MainWindow* window) {
    menu = window->instrumentMenu;
    this->type = SelectorType::WAVETYPE;
    this->rend = window->renderer;
    this->arrowTexture = &window->instrumentMenu->arrowImg;
    optionsImages = std::move(imageTextures);
    mainTexture = optionsImages[0];
}

void SelectControl::render(int x, int y) {
    if (isHighlighted){
        if (type == SelectorType::DOUBLE)
            bool retard = true;
        highlightRect = {x - borderSize*2, y - borderSize*2,
                         mainTexture->w + borderSize * 4,
                         mainTexture->h + borderSize * 4};
        bgRect = {x - borderSize, y - borderSize, mainTexture->w + borderSize * 2, mainTexture->h + borderSize * 2};
        SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(rend, &highlightRect);
        SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(rend, &bgRect);
    }

    if (isEditing){
        arrowTexture->renderRotated(x - arrowTexture->w,
                             y + (mainTexture->h - arrowTexture->h) / 2, 270);
        arrowTexture->renderRotated(x + mainTexture->w,
                             y + (mainTexture->h - arrowTexture->h) / 2, 90);
    }
    mainTexture->render(x,y);
}

void SelectControl::increment(bool largeIncrement) {
    switch(type){
        case SelectorType::FUNCTION:
            incrementFunction();
            break;
        case SelectorType::OSCILLATOR:
            menu->selectNextOsc();
            break;
        case SelectorType::WAVETYPE:
            setNextWaveType(); break;
        case SelectorType::INTEGER:
            *modifiedInteger += 1;
            break;
        case SelectorType::FINEDOUBLE:
            *modifiedDouble += largeIncrement ? 0.1 : 0.01; break;
        case SelectorType::DOUBLE:
            *modifiedDouble += 0.1; break;
    }
}

void SelectControl::decrement(bool largeDecrement) {
    switch(type){
        case SelectorType::FUNCTION:
            decrementFunction();
            break;
        case SelectorType::OSCILLATOR:
            menu->selectPrevOsc();
            break;
        case SelectorType::WAVETYPE:
            setNextWaveType(false); break;
        case SelectorType::FINEDOUBLE:
            *modifiedDouble -= largeDecrement ? 0.1 : 0.01;
            if (*modifiedDouble < 0)
                *modifiedDouble = 0;
            break;
        case SelectorType::DOUBLE:
            *modifiedDouble -= 0.1;
            if (*modifiedDouble < 0)
                *modifiedDouble = 0;
            break;
        case SelectorType::INTEGER:
            if (*modifiedDouble > 0)
                *modifiedInteger -= 1;
            break;
    }
}

void SelectControl::setNextWaveType(bool increment){
    WaveformType previousWave = modifyLFO ? menu->editedOsc->lfo->waveType : menu->editedOsc->waveType;
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
        menu->editedOsc->lfo->setWaveformType(nextWave);
        menu->editedOsc->setupLfoLookup(menu->editedOsc->lfo->waveType);
    } else {
        menu->editedOsc->setWaveformType(nextWave);
    }
}

void SelectControl::activate() {
    if (!modifyLFO || menu->lfoCheckBox.isChecked){
        switchHighlight();
        switchEditing();
    }
}
