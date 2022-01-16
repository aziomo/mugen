//
// Created by alberto on 1/15/22.
//

#ifndef MUGEN_BUTTONCONTROL_H
#define MUGEN_BUTTONCONTROL_H

#include "Texture.h"
#include "Control.h"
#include "MainWindow.h"


class ButtonControl : public Control {
public:
    ~ButtonControl() override{};
//    int borderSize = 2;

    void activate() override;

    OptionsMenu* menu;
    void render(int x, int y);


    Texture* labelTexture;

    void (OptionsMenu::*onPress)();

    SDL_Rect highlightRect, bgRect;
    SDL_Renderer* rend;


    void loadTextures(Texture *labelTexture, MainWindow *window);
};


#endif //MUGEN_BUTTONCONTROL_H
