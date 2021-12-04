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
    void initSDL();
    void render();
    void handleKeyPress(SDL_Keycode key);

    int w = SET_WINDOW_WIDTH;
    int h = SET_WINDOW_HEIGHT;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* mainFont = nullptr;
    TTF_Font* smallFont = nullptr;

    WaveformMenu* waveformMenu;
    MusicBox* mBox;
    SDL_Rect mainArea = {borderSize, borderSize, w - borderSize * 2, h - borderSize * 2};
    int borderSize = 10;

    void handleNewKeyPress(const Uint8 *keys);

    void handleKeyPress(SDL_Event *event);

private:
    SDL_Rect windowArea = {0, 0, w, h};
    SDL_Window* window = nullptr;

    void quitSDL();

};

#endif //MUGEN_CPP_MAINWINDOW_H