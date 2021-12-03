//
// Created by alberto on 11/29/21.
//

#ifndef MUGEN_CHECKBOXCONTROL_H
#define MUGEN_CHECKBOXCONTROL_H

#include "Texture.h"
#include "MainWindow.h"
#include "Control.h"

class CheckboxControl : public Control {

public:
    ~CheckboxControl() override;
    WaveformMenu* menu;
    int borderSize = 2;
    void switchCheck();
    void render(int x, int y);
//    bool isChecked, isHighlighted;
    void loadTextures(std::string text, Texture* checkTexture, MainWindow* window);

    void triggerOnCheck();

    void (WaveformMenu::*onCheck)();
//    void onUncheck(void (*f)());

    SDL_Renderer* rend;
    SDL_Rect highlightRect, bgRect, checkBgRect, checkBorderRect;

    Texture* labelTexture;
    Texture* checkTexture;

    void renderCheck(int x, int y);

private:
    void activate() override;
};


#endif //MUGEN_CHECKBOXCONTROL_H
