#ifndef MUGEN_CPP_MAINWINDOW_H
#define MUGEN_CPP_MAINWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "MusicBox.h"
#include "Texture.h"
#include "GraphPainter.h"

#define SET_WINDOW_WIDTH   (1000)
#define SET_WINDOW_HEIGHT  (700)

class WaveformMenu;

class MainWindow {
public:
    MainWindow(MusicBox* musicBox);
    ~MainWindow();
    void initSDL();
    void render();

    int w = SET_WINDOW_WIDTH;
    int h = SET_WINDOW_HEIGHT;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* mainFont = nullptr;
    TTF_Font* smallFont = nullptr;

    WaveformMenu* waveformMenu;
    GraphPainter* graphPainter;
    MusicBox* mBox;
    int borderSize = 10;
    SDL_Rect mainArea = {borderSize, borderSize, w - borderSize * 2, h - borderSize * 2};

    void handleKeyPress(const Uint8 *keyState, bool *lastKeyState, int* keyPressState);

private:
    SDL_Rect windowArea = {0, 0, w, h};
    SDL_Window* window = nullptr;

    void quitSDL();

    void renderBorders();
};

#endif //MUGEN_CPP_MAINWINDOW_H