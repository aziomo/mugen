#ifndef MUGEN_COMPOSITIONMENU_H
#define MUGEN_COMPOSITIONMENU_H

#include "Direction.h"
#include "ItemList.h"
#include "Timeline.h"
#include <vector>
#include "SelectControl.h"
#include "SegmentManager.h"

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
    SegmentManager* segmentManager;

    SelectControl tempoSelector, segmentsSelector, colsSelector;

    const static int controlMatrixRows = 3, controlMatrixCols = 1;

    int focusedControlRow = 0, focusedControlCol = 0;
    Control* controlArray[controlMatrixRows][controlMatrixCols] = {
            {&segmentsSelector},
            {&colsSelector},
            {&tempoSelector}
    };

    bool isTimelineFocused = false;
    bool isSegmentListFocused = false;
    bool isInstrumentListFocused = false;

    bool shiftPressed = false;

    bool playbackOn = false;

    vector<Bit*> bitsPlayed;

    Texture instrumentListLabel, segmentManagerLabel,
            segmentsLabel, colsLabel, tempoLabel,
            segmentsValue, colsValue, tempoValue;

    void render();
    void setTextTexture(Texture *texture, const string& text) const;
    void setTextTexture(Texture *texture, const string& text, TTF_Font *font) const;
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
    void addColumn();
    void removeColumn();
    void handleKeyPress(SDL_Keycode key);
    Control* getFocusedControl();
    void changeControlFocus(Direction direction);
    void selectFocusedControl();
    void playbackTimeline();
    void switchTimelineFocus();
    void startPlayback();
    void stopPlayback();
    void registerShiftPress(bool shiftPressed);
    void updateHelpBar();
};

#endif //MUGEN_COMPOSITIONMENU_H
