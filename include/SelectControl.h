//
// Created by alberto on 11/21/21.
//

#ifndef MUGEN_CPP_SELECTCONTROL_H
#define MUGEN_CPP_SELECTCONTROL_H


#include "Texture.h"
#include "MainWindow.h"
#include <vector>


class SelectControl {

public:

    void decrement();
    void increment();
    void render(int x, int y);

    bool isHighlighted = false;
    bool isEditing = false;

    void switchEditing();
    void switchHighlight();
    void loadControl(double initialValue, Texture *textTexture, MainWindow *window);
    void loadControl(std::vector<Texture *> imageTextures, MainWindow *window);
private:
    SDL_Renderer* rend;
    int borderSize = 2;
    double selectionValue;
    Texture* mainTexture;
    std::vector<Texture*> optionsImages;
    Texture* arrowTexture;
    SDL_Rect highlightRect, bgRect;
    SDL_Color textColor = {255,255,255};
    TTF_Font* font;

    void enableEditing();

    static std::string doubleToStr(double d, int precision);
};


#endif //MUGEN_CPP_SELECTCONTROL_H
