#ifndef MUGEN_CPP_SELECTCONTROL_H
#define MUGEN_CPP_SELECTCONTROL_H

#include "Texture.h"
#include "MainWindow.h"
#include "SelectorType.h"
#include "Control.h"
#include <vector>

class SelectControl : public Control {

public:
    double* p_modifiedDouble = nullptr;
    int* p_modifiedInteger = nullptr;

    ~SelectControl() override;

    void decrement(bool largeDecrement) override;
    void increment(bool largeIncrement) override;
    void render(int x, int y);

    void activate() override;

    void switchEditing();
    void loadTextControl(SelectorType type, Texture *textTexture, MainWindow *window);
    void loadImageControl(std::vector<Texture*> imageTextures, MainWindow *window);
    void isModifyingLFO(bool isModifying);
    Texture* mainTexture;

    void setModifiedDouble(double* modifiedDouble);
    void setModifiedInteger(int *modifiedInt);
    void setIncrementFunction(void (CompositionMenu::*function)());
    void setDecrementFunction(void (CompositionMenu::*function)());

private:
    void (CompositionMenu::*p_incFunc)();
    void (CompositionMenu::*p_decFunc)();
    SDL_Renderer* p_rend;
    int borderSize = 2;
    vector<Texture*> optionsImages;
    Texture* p_arrowTexture;
    SDL_Rect highlightRect, bgRect;
    SelectorType m_type;
    InstrumentMenu* p_instrumentMenu;
    CompositionMenu* p_compositionMenu;
    bool modifyLFO = false;

    void setNextWaveType(bool increment = true);
};


#endif //MUGEN_CPP_SELECTCONTROL_H
