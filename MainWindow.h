//
// Created by alberto on 11/18/21.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef MUGEN_CPP_MAINWINDOW_H
#define MUGEN_CPP_MAINWINDOW_H

#endif //MUGEN_CPP_MAINWINDOW_H

#define SET_SCREEN_WIDTH   (800)
#define SET_SCREEN_HEIGHT  (600)

const int SCREEN_WIDTH = SET_SCREEN_WIDTH;
const int SCREEN_HEIGHT = SET_SCREEN_HEIGHT;

class MainWindow {
public:
    MainWindow(MusicBox* musicBox){
        this->mBox = musicBox;
        init();
    }

    void init(){
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        } else {
            window = SDL_CreateWindow( "mugen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if( window == nullptr || renderer == nullptr ) {
                printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            }
        }
    }

    void render(){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &windowArea);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &mainArea);
        SDL_RenderPresent(renderer);
    }

    void handleKeyPress(SDL_Keycode key){
        switch (key) {
            case SDLK_z:
                mBox->playMidiNote(); break; //play C note  octave * currentOctave - 3
            case SDLK_s:
                mBox->playMidiNote(1); break; // C#
            case SDLK_x:
                mBox->playMidiNote(2); break; // D
            case SDLK_d:
                mBox->playMidiNote(3); break; // D#
            case SDLK_c:
                mBox->playMidiNote(4); break; // E
            case SDLK_v:
                mBox->playMidiNote(5); break; // F
            case SDLK_g:
                mBox->playMidiNote(6); break; // F#
            case SDLK_b:
                mBox->playMidiNote(7); break; // G
            case SDLK_h:
                mBox->playMidiNote(8); break; // G#
            case SDLK_n:
                mBox->playMidiNote(9); break; // A
            case SDLK_j:
                mBox->playMidiNote(10); break; // A#
            case SDLK_m:
                mBox->playMidiNote(11); break; // B
            case SDLK_COMMA:
                mBox->playMidiNote(12); break; // C
            case SDLK_l:
                mBox->playMidiNote(13); break; // C#
            case SDLK_PERIOD:
                mBox->playMidiNote(14); break; // D
            case SDLK_SEMICOLON:
                mBox->playMidiNote(15); break; // D#
            case SDLK_SLASH:
                mBox->playMidiNote(16); break; // E
            case SDLK_9:
                mBox->addOscillator(WaveformType::SINE); break;
            case SDLK_0:
                mBox->addOscillator(WaveformType::SQUARE); break;
            case SDLK_MINUS:
                mBox->popOscillator(); break;
        }
    }

private:
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Rect windowArea = {0,0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect mainArea = {25, 25, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50};
    MusicBox* mBox;
};
