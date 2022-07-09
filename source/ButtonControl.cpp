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
        highlightRect = { x-borderSize*4, y-borderSize*4,
                          labelTexture->w + borderSize*8,
                          labelTexture->h + borderSize*8};
        bgRect = {x - borderSize*2, y - borderSize*2,
                  labelTexture->w + borderSize * 4,
                  labelTexture->h + borderSize * 4};
        SetRenderDrawColor(rend, menu->window->colors.foreground);
        SDL_RenderFillRect(rend, &highlightRect);
        SetRenderDrawColor(rend, menu->window->colors.background);
        SDL_RenderFillRect(rend, &bgRect);
    }
    labelTexture->render(x, y);
}

void ButtonControl::loadTextures(Texture* labelTexture, MainWindow* window){
    menu = window->optionsMenu;
    this->rend = window->renderer;
    this->labelTexture = labelTexture;
}