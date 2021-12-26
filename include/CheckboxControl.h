#ifndef MUGEN_CHECKBOXCONTROL_H
#define MUGEN_CHECKBOXCONTROL_H

#include "Texture.h"
#include "MainWindow.h"
#include "Control.h"

class CheckboxControl : public Control {

public:

    ~CheckboxControl() override {};
    InstrumentMenu* menu;
    int borderSize = 2;
    bool horizontal;
    void switchCheck();
    void render(int x, int y);

    void triggerOnCheck();

    void (InstrumentMenu::*onCheck)();

    SDL_Renderer* rend;
    SDL_Rect highlightRect, bgRect, checkboxBgRect, checkboxBorderRect, renderArea;


    Texture* labelTexture;
    Texture* checkTexture;

    void renderCheckbox(int x, int y);

    void loadTextures(Texture *labelTexture, MainWindow *window, bool horizontal = true);

private:
    void activate() override;

};


#endif //MUGEN_CHECKBOXCONTROL_H
