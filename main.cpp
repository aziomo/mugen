#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <thread>
#include "include/MusicBox.h"
#include "include/MainWindow.h"

int main() {

    MusicBox* musicBox = new MusicBox();
    MainWindow* mainWindow = new MainWindow(musicBox);

    bool lastKeyState[128];
    int keyPressState[128] = {0};
    const Uint8* state;
    state = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < 128; i++)
        lastKeyState[i] = state[i];

    musicBox->startPlaying();
    while (!mainWindow->quit){
        SDL_PumpEvents();
        int keyboardSize = 20;
        state = SDL_GetKeyboardState(&keyboardSize);
        mainWindow->handleKeyPress(state, lastKeyState, keyPressState);
        for (int i = 0; i < 128; i++)
            lastKeyState[i] = state[i];
        mainWindow->render();
    }
    delete mainWindow;
    delete musicBox;
    exit(0);
}
