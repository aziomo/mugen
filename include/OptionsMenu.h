#ifndef MUGEN_OPTIONSMENU_H
#define MUGEN_OPTIONSMENU_H
#include "Serializer.h"
#include "Deserializer.h"
#include "MainWindow.h"
#include "ButtonControl.h"
#include "ItemList.h"

class MainWindow;

class OptionsMenu {

public:
    SDL_Renderer* renderer;
    SDL_Color textColor;
    MainWindow* window;
    MusicBox* musicBox;
    ItemList* itemList;
    Serializer* serializer;
    Deserializer* deserializer;
    int screenRendered = 0;
    bool shiftPressed = false,
        dialogOpen = false,
        listFocused = false;

    string inputValue = "",
        dialogTextValue = "";

    ButtonControl exportSongBtn, saveProjectBtn, loadProjectBtn, quitBtn, okBtn;

    SDL_Rect inputBox, inputBoxBorder, dialogBox, dialogBoxBorder;

    Texture exportSongLabel, saveProjectLabel, loadProjectLabel, quitLabel, okLabel,
            opDescriptionLabel, opCancelLabel, opConfirmLabel,
            inputValueLabel;

    const static int controlMatrixRows = 4, controlMatrixCols = 1;
    int focusedControlRow = 0, focusedControlCol = 0;

    Control* controlArray[controlMatrixRows][controlMatrixCols] = {
            {&exportSongBtn},
            {&saveProjectBtn},
            {&loadProjectBtn},
            {&quitBtn}
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

    void inputLetter(const Uint8 *keyState, const bool *lastKeyState);

    void openSaveProjectScreen();

    void openLoadProjectScreen();

    void openExportSongScreen();

    void quit();

    void updateControls();

    void deleteLetter();

    void registerShiftPress(bool b);

    vector<string> getDirFilenamesWithoutExtensions(const string &dirPath);
};




#endif //MUGEN_OPTIONSMENU_H
