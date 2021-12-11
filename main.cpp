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

    //SDL_GetKeyboardState(nullptr);

    musicBox->startPlaying();
    while (!quit){
        // new keyboard capture system
        SDL_PumpEvents();

        int keyboardSize = 20;
        state = SDL_GetKeyboardState(&keyboardSize);

        mainWindow->handleNewKeyPress(state, lastKeyState, keyPressState);

        //  0 + 0 -> 0
        //  0 + 1 -> 1
        //  1 + 0 -> 0
        //  1 + 1 -> 0

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



        // old keyboard capture system
/*
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
                mainWindow->handleKeyPress(&e);
                switch (e.key.keysym.sym) {
                    case SDLK_F4:
                        quit = 1;
                        break;
                    case SDLK_1:
                        musicBox->isRunning ?
                            musicBox->stopPlaying() : musicBox->startPlaying();
                        break;
                    default:
                        break;
                }
            }
        }
*/
        mainWindow->render();
    }
    delete mainWindow;
    delete musicBox;
    return 0;
}
