#ifndef MUGEN_CPP_MAINWINDOW_H
#define MUGEN_CPP_MAINWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "MusicBox.h"
#include "Texture.h"

#define SET_WINDOW_WIDTH   (1000)
#define SET_WINDOW_HEIGHT  (600)

#define INST_MENU 1
#define COMP_MENU 2
#define OPTI_MENU 3

class InstrumentMenu;
class CompositionMenu;
class OptionsMenu;

class MainWindow {
public:
    MainWindow(MusicBox* musicBox);
    ~MainWindow();
    void initSDL();
    void render();

    int w = SET_WINDOW_WIDTH;
    int h = SET_WINDOW_HEIGHT;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* largeFont = nullptr;
    TTF_Font* mainFont = nullptr;
    TTF_Font* smallFont = nullptr;
    TTF_Font* tinyFont = nullptr;
    bool typing = false;
    bool showPiano = false;

    InstrumentMenu* instrumentMenu;
    CompositionMenu* compositionMenu;
    OptionsMenu* optionsMenu;
    MusicBox* musicBox;
    Texture helpBar, piano, octaveLabel, octaveDescriptionLabel, octaveHelpLabel;
    int borderSize = 10;
    SDL_Rect mainArea = {borderSize, borderSize, w - borderSize * 2, h - borderSize * 2};

    void handleKeyPress(const Uint8 *keyState, bool *lastKeyState, int* keyPressState);

    int xByPercent(Texture *texture, double percent, Alignment align = CENTER) const;
    int xByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;
    int yByPercent(Texture *texture, double percent, Alignment align = CENTER) const;
    int yByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;

    void setHelpBarText(const string &text);

    int openTab = 2;
private:
    Texture instrumentsTab, compositionTab, optionsTab,
            f1Label, f2Label, f3Label;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect windowArea = {0, 0, w, h};

    SDL_Window* window = nullptr;

    void quitSDL();
    void loadTextures();

    void renderTabs();

    void passKeyPressToMenu(int menu, SDL_Keycode);

    void registerShiftPress(int menu, bool shiftPressed);

};

#endif //MUGEN_CPP_MAINWINDOW_H