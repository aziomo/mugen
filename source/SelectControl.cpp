//
// Created by alberto on 11/21/21.
//

#include "../include/SelectControl.h"


void SelectControl::switchHighlight(){
    isHighlighted = !isHighlighted;
}

void SelectControl::loadControl(Texture* renderedItem, SDL_Renderer* rend) {
    this->rend = rend;
//    arrowTexture->loadFromFile(rend, assets_dir + "arrow.png");
    mainTexture = renderedItem;
}


void SelectControl::render(int x, int y) {
    if (isHighlighted){
        highlightRect = {x - borderSize*2, y - borderSize*2,
                         mainTexture->width + borderSize * 4,
                         mainTexture->height + borderSize * 4};
        bgRect = {x - borderSize, y - borderSize, mainTexture->width + borderSize * 2, mainTexture->height + borderSize * 2};
        SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(rend, &highlightRect);
    }
    else {
        bgRect = {x - borderSize, y - borderSize,
                 mainTexture->width + borderSize * 2,
                 mainTexture->height + borderSize * 2};
    }
    SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(rend, &bgRect);
    mainTexture->render(x,y);
}
