#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <thread>
#include "MusicBox.h"
#include "MainWindow.h"

int main() {

    MusicBox* musicBox = new MusicBox(1);
    MainWindow* mainWindow = new MainWindow(musicBox);
    int quit = 0;
    SDL_Event e;

    while (!quit){
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT){
                quit = 1;
            }
            else if (e.type == SDL_KEYDOWN){
                mainWindow->handleKeyPress(e.key.keysym.sym);
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        quit = 1;
                        break;
                    case SDLK_o:
                        musicBox->isRunning ?
                            musicBox->stopPlaying() : musicBox->startPlaying();
                        break;
                    default:
                        // do nothing i guess
                        break;
                }
            }
        }
        mainWindow->render();
    }

    return 0;
}
