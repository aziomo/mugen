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

    musicBox->startPlaying();
    while (!quit){
        // new keyboard capture system
        SDL_PumpEvents();

        const Uint8* state = SDL_GetKeyboardState(nullptr);

//        if (state[SDLv
        mainWindow->handleNewKeyPress(state);

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



        // old keyboard capture system
        /*while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT){
                quit = 1;
            }
            else if (e.type == SDL_KEYDOWN){
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
                        // do nothing i guess
                        break;
                }
            }
        }*/
        mainWindow->render();
    }
    delete mainWindow;
    delete musicBox;
    return 0;
}
