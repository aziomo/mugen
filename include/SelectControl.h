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
    double* modifiedValue = nullptr;

    ~SelectControl() override;

    void decrement(bool largeDecrement) override;
    void increment(bool largeIncrement) override;
    void render(int x, int y);

//    bool isHighlighted = false;
//    bool isEditing = false;

    void activate() override;

    void switchEditing();
    void loadTextControl(SelectorType type, Texture *textTexture, MainWindow *window);
    void loadImageControl(std::vector<Texture *> imageTextures, MainWindow *window);
    void setModifiedOsc(Oscillator *osc);
    void isModifyingLFO(bool isModifying);
    Texture* mainTexture;
    void setModifiedValue(double* modifiedValue);

private:
    SDL_Renderer* rend;
    int borderSize = 2;
    std::vector<Texture*> optionsImages;
    Texture* arrowTexture;
    SDL_Rect highlightRect, bgRect;
    SelectorType type;
    InstrumentMenu* menu;
    Oscillator* modifiedOsc;
    bool modifyLFO = false;


    void setNextWaveType(bool increment = true);
};


#endif //MUGEN_CPP_SELECTCONTROL_H
