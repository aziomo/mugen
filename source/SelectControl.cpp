//
// Created by alberto on 11/21/21.
//

#include <sstream>
#include <iomanip>
#include "../include/SelectControl.h"
#include "../include/WaveformMenu.h"

SelectControl::~SelectControl(){
    rend = nullptr;
    for(auto texture : optionsImages){
        texture = nullptr;
    }
    arrowTexture = nullptr;
    modifiedValue = nullptr;
    modifiedOsc = nullptr;
    mainTexture = nullptr;
    font = nullptr;
}

void SelectControl::switchHighlight(){
    isHighlighted = !isHighlighted;
}

void SelectControl::switchEditing(){
    isEditing = !isEditing;
}

void SelectControl::loadTextControl(double* initialValue, Texture* textTexture, MainWindow* window) {
    isImage = false;
    this->rend = window->renderer;
    this->arrowTexture = &window->waveformMenu->arrow;
    this->textColor = window->waveformMenu->textColor;
    this->font = window->mainFont;
    modifiedValue = initialValue;
    mainTexture = textTexture;
}

void SelectControl::loadImageControl(Oscillator* osc, std::vector<Texture*> imageTextures, MainWindow* window) {
    isImage = true;
    imageIndex = 0;
    this->rend = window->renderer;
    this->arrowTexture = &window->waveformMenu->arrow;
    optionsImages = std::move(imageTextures);
    modifiedOsc = osc;
    mainTexture = optionsImages[0];
}

void SelectControl::enableEditing(){
    isEditing = true;
}

void SelectControl::render(int x, int y) {
    if (isHighlighted){
        highlightRect = {x - borderSize*2, y - borderSize*2,
                         mainTexture->width + borderSize * 4,
                         mainTexture->height + borderSize * 4};
        bgRect = {x - borderSize, y - borderSize, mainTexture->width + borderSize * 2, mainTexture->height + borderSize * 2};
        SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(rend, &highlightRect);
        SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(rend, &bgRect);
    }

    if (isEditing){
        arrowTexture->renderRotated(x - arrowTexture->width,
                             y + (mainTexture->height - arrowTexture->height) / 2, 270);
        arrowTexture->renderRotated(x + mainTexture->width,
                             y + (mainTexture->height - arrowTexture->height) / 2, 90);
    }
    mainTexture->render(x,y);
}

void SelectControl::increment() {
    if (this->isImage){
        if (++imageIndex == optionsImages.size()){
            imageIndex = 0;
        }
        nextWaveType();
        mainTexture = optionsImages[imageIndex];
    }
    else {
        *modifiedValue += 0.01;
        mainTexture->loadFromText(rend, doubleToStr(*modifiedValue, 2), textColor, font);
    }
}

void SelectControl::decrement() {
    if (this->isImage){
        if (--imageIndex < 0){
            imageIndex += optionsImages.size();
        }
        nextWaveType(false);
        mainTexture = optionsImages[imageIndex];
    }
    else {
        *modifiedValue -= 0.01;
        if (*modifiedValue < 0) {
            *modifiedValue = 0;
        }
        mainTexture->loadFromText(rend, doubleToStr(*modifiedValue, 2), textColor, font);
    }
}

std::string SelectControl::doubleToStr(double d, int precision){
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << d;
    return stream.str();
}

void SelectControl::nextWaveType(bool increment){
    switch (modifiedOsc->waveType) {
        case WaveformType::SINE:
            modifiedOsc->setWaveformType(increment ? WaveformType::SQUARE : WaveformType::NOISE); break;
        case WaveformType::SQUARE:
            modifiedOsc->setWaveformType(increment ? WaveformType::TRIANGLE : WaveformType::SINE); break;
        case WaveformType::TRIANGLE:
            modifiedOsc->setWaveformType(increment ? WaveformType::SAWTOOTH : WaveformType::SQUARE); break;
        case WaveformType::SAWTOOTH:
            modifiedOsc->setWaveformType(increment ? WaveformType::NOISE : WaveformType::TRIANGLE); break;
        case WaveformType::NOISE:
            modifiedOsc->setWaveformType(increment ? WaveformType::SINE : WaveformType::SAWTOOTH); break;
    }
}