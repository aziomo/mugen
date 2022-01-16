//
// Created by alberto on 1/15/22.
//

#include "../include/ButtonControl.h"
#include "../include/OptionsMenu.h"

void ButtonControl::activate() {
    (menu->*onPress)();
}

void ButtonControl::render(int x, int y) {
    if (isHighlighted){
        highlightRect = { x - borderSize*2, y - borderSize*2,
                          labelTexture->w,
                          labelTexture->h};
        bgRect = {x - borderSize, y - borderSize,
                  labelTexture->w - borderSize * 2,
                  labelTexture->h - borderSize * 2};
        SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(rend, &highlightRect);
        SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(rend, &bgRect);
    }
    labelTexture->render(x, y);
}

void ButtonControl::loadTextures(Texture* labelTexture, MainWindow* window){
    menu = window->optionsMenu;
    this->rend = window->renderer;
    this->labelTexture = labelTexture;

}