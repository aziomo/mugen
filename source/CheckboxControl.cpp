#include "../include/CheckboxControl.h"
#include "../include/WaveformMenu.h"


void CheckboxControl::switchCheck() {
    isChecked = !isChecked;
}

void CheckboxControl::activate() {
    switchCheck();
    triggerOnCheck();
}

void CheckboxControl::render(int x, int y) {
    if (isHighlighted){
        highlightRect = { x - borderSize*2, y - borderSize*2,
                          labelTexture->w + borderSize*4,
                          (labelTexture->h + checkTexture->h + borderSize*4) + borderSize*2};
        bgRect = {x - borderSize, y - borderSize,
                  labelTexture->w + borderSize * 2,
                  labelTexture->h + checkTexture->h + borderSize * 4};
        SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(rend, &highlightRect);
        SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(rend, &bgRect);
    }
    labelTexture->render(x, y);
    renderCheck(x + labelTexture->w * 0.5 - checkTexture->w * 0.5, y + labelTexture->h);
}

void CheckboxControl::renderCheck(int x, int y){
    checkBorderRect = { x - borderSize*2, y - borderSize*2,
                        checkTexture->w + borderSize*4,
                        checkTexture->h + borderSize*4};
    checkBgRect = {x - borderSize, y - borderSize,
                   checkTexture->w + borderSize*2, checkTexture->h + borderSize*2};
    SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(rend, &checkBorderRect);
    SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(rend, &checkBgRect);
    if (isChecked) {
        checkTexture->render(x, y);
    }
}

void CheckboxControl::loadTextures(std::string text, Texture* labelTexture, MainWindow* window){
    menu = window->waveformMenu;
    this->rend = window->renderer;
    this->checkTexture = &window->waveformMenu->checkImg;
    this->labelTexture = labelTexture;
//    labelTexture->loadFromText(window->renderer, text, window->waveformMenu->textColor, window->smallFont);
}

CheckboxControl::~CheckboxControl() {

}

void CheckboxControl::triggerOnCheck() {
    (menu->*onCheck)();
}
