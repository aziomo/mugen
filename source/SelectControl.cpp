//
// Created by alberto on 11/21/21.
//

#include <sstream>
#include <iomanip>
#include "../include/SelectControl.h"
#include "../include/WaveformMenu.h"

void SelectControl::switchHighlight(){
    isHighlighted = !isHighlighted;
}

void SelectControl::switchEditing(){
    isEditing = !isEditing;
}

void SelectControl::loadControl(double initialValue, Texture* textTexture, MainWindow* window) {
    this->rend = window->renderer;
    this->arrowTexture = &window->waveformMenu->arrow;
    this->textColor = window->waveformMenu->textColor;
    this->font = window->mainFont;
    selectionValue = initialValue;
    mainTexture = textTexture;
}

void SelectControl::loadControl(std::vector<Texture*> imageTextures, MainWindow* window) {
    this->rend = window->renderer;
    this->arrowTexture = &window->waveformMenu->arrow;
    selectionValue = 0;
    optionsImages = std::move(imageTextures);
    mainTexture = optionsImages[selectionValue];
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
    if (optionsImages.size() != 0){
        if (++selectionValue == optionsImages.size()){
            selectionValue = 0;
        }
        mainTexture = optionsImages[selectionValue];
    }
    else {
        selectionValue += 0.01;
        mainTexture->loadFromText(rend, doubleToStr(selectionValue, 2), textColor, font);
    }
}

void SelectControl::decrement() {
    if (optionsImages.size() != 0){
        if (--selectionValue < 0){
            selectionValue += optionsImages.size();
        }
        mainTexture = optionsImages[selectionValue];
    }
    else {
        selectionValue -= 0.01;
        if (selectionValue < 0) {
            selectionValue = 0;
        }
        mainTexture->loadFromText(rend, doubleToStr(selectionValue, 2), textColor, font);
    }
}

std::string SelectControl::doubleToStr(double d, int precision){
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << d;
    return stream.str();
}