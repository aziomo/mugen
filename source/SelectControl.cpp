//
// Created by alberto on 11/21/21.
//

#include <sstream>
#include <iomanip>
#include "../include/SelectControl.h"
#include "../include/WaveformMenu.h"

SelectControl::~SelectControl(){
    rend = nullptr;
    while (!optionsImages.empty()){
        optionsImages.pop_back();
    }
    arrowTexture = nullptr;
    mainTexture = nullptr;
}

void SelectControl::setModifiedOsc(Oscillator* osc){
    modifiedOsc = osc;
}

void SelectControl::switchEditing(){
    isEditing = !isEditing;
}

void SelectControl::isModifyingLFO(bool isModifying){
    modifyLFO = isModifying;
}

void SelectControl::loadTextControl(SelectorType selectorType, Texture* textTexture, MainWindow* window) {
    menu = window->waveformMenu;
    this->type = selectorType;
    this->rend = window->renderer;
    this->arrowTexture = &window->waveformMenu->arrowImg;
    mainTexture = textTexture;
}

void SelectControl::loadImageControl(std::vector<Texture*> imageTextures, MainWindow* window) {
    menu = window->waveformMenu;
    this->type = SelectorType::WAVETYPE;
    this->rend = window->renderer;
    this->arrowTexture = &window->waveformMenu->arrowImg;
    optionsImages = std::move(imageTextures);
    mainTexture = optionsImages[0];
}

void SelectControl::render(int x, int y) {
    if (isHighlighted){
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
        case SelectorType::INSTRUMENT:
            break;
        case SelectorType::OSCILLATOR:
            menu->nextOsc();
            break;
        case SelectorType::WAVETYPE:
            setNextWaveType(); break;
        case SelectorType::FREQUENCY:
            if (!modifyLFO){
                menu->editedOsc->freqModifier += largeIncrement ? 0.1 : 0.001; break;
            } else {
                menu->editedOsc->lfo->setFrequency(menu->editedOsc->lfo->currentFrequency + (largeIncrement ? 0.1 : 0.001)); break;
            }

        case SelectorType::AMPLITUDE:
            if (!modifyLFO){
                menu->editedOsc->ampModifier += largeIncrement ? 0.1 : 0.001; break;
            } else {
                menu->editedOsc->lfo->ampModifier += largeIncrement ? 0.1 : 0.001; break;
            }
    }
}

void SelectControl::decrement(bool largeDecrement) {
    switch(type){
        case SelectorType::INSTRUMENT:
            break;
        case SelectorType::OSCILLATOR:
            menu->nextOsc(true);
            break;
        case SelectorType::WAVETYPE:
            setNextWaveType(false); break;
        case SelectorType::FREQUENCY:
            if (!modifyLFO){
                menu->editedOsc->freqModifier -= largeDecrement ? 0.1 : 0.001;
                if (menu->editedOsc->freqModifier < 0) {
                    menu->editedOsc->freqModifier = 0;
                }
            } else {
                if (menu->editedOsc->lfo->currentFrequency - (largeDecrement ? 0.1 : 0.001) < 0) {
                    menu->editedOsc->lfo->setFrequency(0);
                } else {
                    menu->editedOsc->lfo->setFrequency(menu->editedOsc->lfo->currentFrequency
                                                        - (largeDecrement ? 0.1 : 0.001));
                }
//                if (menu->editedOsc->lfo->freqModifier < 0) {
//                    menu->editedOsc->lfo->freqModifier = 0;
//                }
            }
            break;
        case SelectorType::AMPLITUDE:
            if (!modifyLFO){
                menu->editedOsc->ampModifier -= largeDecrement ? 0.1 : 0.001;
                if (menu->editedOsc->ampModifier < 0) {
                    menu->editedOsc->ampModifier = 0;
                }
            } else {
                menu->editedOsc->lfo->ampModifier -= largeDecrement ? 0.1 : 0.001;
                if (menu->editedOsc->lfo->ampModifier < 0) {
                    menu->editedOsc->lfo->ampModifier = 0;
                }
            }
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
