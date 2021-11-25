#ifndef MUGEN_CPP_MAINWINDOW_H
#define MUGEN_CPP_MAINWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "MusicBox.h"
#include "Texture.h"

#define SET_WINDOW_WIDTH   (800)
#define SET_WINDOW_HEIGHT  (600)

class WaveformMenu;

class MainWindow {
public:
    MainWindow(MusicBox* musicBox);
    ~MainWindow();
    void init();
    void render();
    void handleKeyPress(SDL_Keycode key);

    int windowWidth = SET_WINDOW_WIDTH;
    int windowHeight = SET_WINDOW_HEIGHT;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* mainFont = nullptr;

    WaveformMenu* waveformMenu;
    MusicBox* mBox;
private:
    int borderSize = 10;
    SDL_Rect windowArea = {0, 0, windowWidth, windowHeight};
    SDL_Window* window = nullptr;
    SDL_Rect mainArea = {borderSize, borderSize, windowWidth - borderSize*2, windowHeight - borderSize*2};
};

#endif //MUGEN_CPP_MAINWINDOW_H