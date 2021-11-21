//
// Created by alberto on 11/21/21.
//

#ifndef MUGEN_CPP_SELECTCONTROL_H
#define MUGEN_CPP_SELECTCONTROL_H


#include "Texture.h"


class SelectControl {

public:

    void loadControl(Texture* renderedItem, SDL_Renderer *rend);
    void highlightOn();
    void highlightOff();
    void changePrev();
    void changeNext();
    void render(int x, int y);

    bool isHighlighted = false;

    void switchHighlight();

private:
    SDL_Renderer* rend;
    int borderSize = 2;
//    T value;
    bool isEdited;
    Texture *mainTexture, *arrowTexture;
    SDL_Rect highlightRect, bgRect;

};


#endif //MUGEN_CPP_SELECTCONTROL_H
