#ifndef MUGEN_OPTIONSMENU_H
#define MUGEN_OPTIONSMENU_H
#include "Serializer.h"
#include "Deserializer.h"
#include "MainWindow.h"
#include "ButtonControl.h"

class MainWindow;

class OptionsMenu {

public:
    SDL_Renderer* renderer;
    SDL_Color textColor;
    MainWindow* window;
    MusicBox* musicBox;

    Serializer* serializer;
    Deserializer* deserializer;

    ButtonControl saveSongBtn, saveProjectBtn, loadProjectBtn;

    Texture saveSongLabel, saveProjectLabel, loadProjectLabel;

    const static int controlMatrixRows = 3, controlMatrixCols = 1;
    int focusedControlRow = 0, focusedControlCol = 0;

    Control* controlArray[controlMatrixRows][controlMatrixCols] = {
            {&saveSongBtn},
            {&saveProjectBtn},
            {&loadProjectBtn}
    };


    void init();

    void setTextTexture(Texture *texture, const string &text, TTF_Font *font) const;
    void loadTextures();
    void loadControls();
    void render();

    void saveProject();

    int xByPercent(Texture *texture, double percent, Alignment align = CENTER) const;
    int xByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;
    int yByPercent(Texture *texture, double percent, Alignment align = CENTER) const;

    int yByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;

    OptionsMenu(MainWindow* mainWindow);

    Control *getFocusedControl();

    void handleKeyPress(SDL_Keycode key);

    void selectFocusedControl();

    void changeControlFocus(Direction direction);

    void loadProject();

    void loadInstruments(JSON projectJson);

    void loadComposition(JSON projectJson);
};




#endif //MUGEN_OPTIONSMENU_H
