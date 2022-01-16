//
// Created by alberto on 11/21/21.
//

#ifndef MUGEN_CPP_SELECTCONTROL_H
#define MUGEN_CPP_SELECTCONTROL_H


#include "Texture.h"
#include "MainWindow.h"
#include "SelectorType.h"
#include "Control.h"
#include <vector>


class SelectControl : public Control {

public:
    double* modifiedDouble = nullptr;
    int* modifiedInteger = nullptr;

    ~SelectControl() override;

    void decrement(bool largeDecrement) override;
    void increment(bool largeIncrement) override;
    void render(int x, int y);

    void activate() override;

    void switchEditing();
    void loadTextControl(SelectorType type, Texture *textTexture, MainWindow *window);
    void loadImageControl(std::vector<Texture *> imageTextures, MainWindow *window);
    void isModifyingLFO(bool isModifying);
    Texture* mainTexture;

    void setModifiedDouble(double* modifiedDouble);
    void setModifiedInteger(int *modifiedInt);
    void setIncrementFunction(void (CompositionMenu::*function)());
    void setDecrementFunction(void (CompositionMenu::*function)());

private:
    void (CompositionMenu::*incrementFunction)();
    void (CompositionMenu::*decrementFunction)();
    SDL_Renderer* rend;
    int borderSize = 2;
    vector<Texture*> optionsImages;
    Texture* arrowTexture;
    SDL_Rect highlightRect, bgRect;
    SelectorType type;
    InstrumentMenu* instrumentMenu;
    CompositionMenu* compositionMenu;
    bool modifyLFO = false;


    void setNextWaveType(bool increment = true);

};


#endif //MUGEN_CPP_SELECTCONTROL_H
