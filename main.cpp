#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <thread>
#include "include/MusicBox.h"
#include "include/MainWindow.h"

int main() {

    MusicBox* musicBox = new MusicBox();
    MainWindow* mainWindow = new MainWindow(musicBox);
    int quit = 0;
    SDL_Event e;

    bool stillPressed = false;
    bool lastKeyState[128];
    int keyPressState[128] = {0};
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    for (int i = 0; i < 128; i++) {
        lastKeyState[i] = state[i];
    }

//    musicBox->startPlaying();z
    while (!quit){
        SDL_PumpEvents();

        int keyboardSize = 20;
        state = SDL_GetKeyboardState(&keyboardSize);

        mainWindow->handleKeyPress(state, lastKeyState, keyPressState);

        if(!state[SDL_SCANCODE_1] && stillPressed){
            stillPressed = false;
        }

        if (state[SDL_SCANCODE_1] && !stillPressed )  {
            stillPressed = true;
            musicBox->isRunning ?
                musicBox->stopPlaying() : musicBox->startPlaying();
        }

        if (state[SDL_SCANCODE_F4]){
            quit = 1;
        }

        for (int i = 0; i < 128; i++) {
            lastKeyState[i] = state[i];
        }

        mainWindow->render();
    }
    delete mainWindow;
    delete musicBox;
    return 0;
}
