//
// Created by alberto on 12/27/21.
//

#ifndef MUGEN_COMPOSITIONMENU_H
#define MUGEN_COMPOSITIONMENU_H


#include "Texture.h"
#include "Direction.h"
#include "ItemList.h"
#include "Timeline.h"
#include <vector>
#include "MainWindow.h"
#include "SelectControl.h"

class MainWindow;

class CompositionMenu {

public:
    CompositionMenu(MainWindow* mainWindow);

    SDL_Renderer* renderer;
    SDL_Color textColor;
    MainWindow* window;
    MusicBox* musicBox;
    ItemList* instrumentList;
    Timeline* timeline;
    SelectControl tempoSelector, segmentsSelector, rowsSelector;

    const static int controlMatrixRows = 3, controlMatrixCols = 1;

    int focusedControlRow = 0, focusedControlCol = 0;
    Control* controlArray[controlMatrixRows][controlMatrixCols] = {
            {&segmentsSelector},
            {&rowsSelector},
            {&tempoSelector}
    };

    bool isTimelineFocused = false;
    int focusedSegment = 0;
    int focusedRow = 0;
    int focusedBit = 0;

    vector<Bit*> bitsPlayed;




    Texture instrumentListLabel,
            segmentsLabel, rowsLabel, tempoLabel,
            segmentsValue, rowsValue, tempoValue;




    void render();


    void setTextTexture(Texture *texture, string text) const;

    void setTextTexture(Texture *texture, string text, TTF_Font *font) const;

    void loadTextures();

    void updateTextures();

    void init();

    int xByPercent(Texture *texture, double percent, Alignment align = CENTER) const;
    int xByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;
    int yByPercent(Texture *texture, double percent, Alignment align = CENTER) const;
    int yByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;

    void loadControls();

    void addSegment();

    void removeSegment();

    void addRow();

    void removeRow();

    void handleKeyPress(SDL_Keycode key);

    Control* getFocusedControl();

    void changeControlFocus(Direction direction);

    void selectFocusedControl();

    void loadExampleBits();

    void playbackTimeline();
};


#endif //MUGEN_COMPOSITIONMENU_H
