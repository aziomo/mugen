//
// Created by alberto on 11/20/21.
//

#include "../include/MainWindow.h"
#include "../include/WaveformMenu.h"

MainWindow::MainWindow(MusicBox* musicBox){
    this->mBox = musicBox;
    init();
}

void MainWindow::init(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        window = SDL_CreateWindow( "mugen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN );
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if( window == nullptr || renderer == nullptr ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }

        TTF_Init();

        mainFont = TTF_OpenFont("HoneyRoom.ttf", 32);
//        mainFont = TTF_OpenFont("Anonymous_Pro.ttf", 24);
        if (mainFont == nullptr)
            printf("font fucked up\n");
        waveformMenu = new WaveformMenu(this);
        waveformMenu->init();
    }
}

void MainWindow::render(){
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &windowArea);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer, &mainArea);
    waveformMenu->render();
    SDL_RenderPresent(renderer);
}

void MainWindow::handleKeyPress(SDL_Keycode key){
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
        case SDLK_i:
            break;
        case SDLK_UP:
            break;
        case SDLK_DOWN:
            break;
        case SDLK_LEFT:
            waveformMenu->handleKeyPress(SDLK_LEFT); break;
        case SDLK_RIGHT:
            waveformMenu->handleKeyPress(SDLK_RIGHT); break;
        case SDLK_RETURN:
            waveformMenu->handleKeyPress(SDLK_RETURN); break;
        default:
            break;
    }
}