#include "../include/MainWindow.h"
#include "../include/WaveformMenu.h"

MainWindow::MainWindow(MusicBox* musicBox){
    this->mBox = musicBox;
    initSDL();
    waveformMenu = new WaveformMenu(this);
    waveformMenu->init();
}

MainWindow::~MainWindow(){
    delete waveformMenu;
    mBox = nullptr;
    quitSDL();
}

void MainWindow::initSDL(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        window = SDL_CreateWindow("mugen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN );
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if( window == nullptr || renderer == nullptr ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        TTF_Init();
        mainFont = TTF_OpenFont((assets_dir + "HoneyRoom.ttf").c_str(), 26);
        smallFont = TTF_OpenFont((assets_dir + "HoneyRoom.ttf").c_str(), 16);
        if (mainFont == nullptr){
            printf("font fucked up\n");
        }
    }
}

void MainWindow::quitSDL(){
    TTF_CloseFont(mainFont);
    mainFont = nullptr;
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;
    renderer = nullptr;
    IMG_Quit();
    SDL_Quit();
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

void MainWindow::handleKeyPress(SDL_Event* event){
    if (event->type == SDL_KEYDOWN) {

    } else if (event->type == SDL_KEYUP){

    }
    switch (event->key.keysym.sym) {
        case SDLK_z:
            mBox->playMidiNote(); break; //play C note  octave * currentOctave - 3
        case SDLK_s:
            mBox->playMidiNote(1); break; // C#
        case SDLK_x:
            mBox->pressedKeys[2] = true;
//            mBox->playMidiNote(2); break; // D
            mBox->putMidiNoteInQueue(2); break;
        case SDLK_d:
//            mBox->playMidiNote(3); break; // D#
            mBox->putMidiNoteInQueue(3); break;
        case SDLK_c:
//            mBox->playMidiNote(4); break; // E
            mBox->putMidiNoteInQueue(4); break;
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
        case SDLK_MINUS:
            waveformMenu->handleKeyPress(SDLK_MINUS); break;
        case SDLK_EQUALS:
            waveformMenu->handleKeyPress(SDLK_EQUALS); break;
        case SDLK_2:
            waveformMenu->handleKeyPress(SDLK_2); break;
        case SDLK_3:
            waveformMenu->handleKeyPress(SDLK_3); break;
        case SDLK_9:
            waveformMenu->handleKeyPress(SDLK_9); break;

        case SDLK_UP:
            waveformMenu->handleKeyPress(SDLK_UP); break;
        case SDLK_DOWN:
            waveformMenu->handleKeyPress(SDLK_DOWN); break;
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

void MainWindow::handleNewKeyPress(const Uint8 *keys) {

    if (keys[SDL_SCANCODE_X] && !mBox->pressedKeys[2]){
        mBox->pressedKeys[2] = true;
    }
    if (!keys[SDL_SCANCODE_X] && mBox->pressedKeys[2]){
        mBox->pressedKeys[2] = false;
    }
    if (keys[SDL_SCANCODE_D] && !mBox->pressedKeys[3]){
        mBox->pressedKeys[3] = true;
    }
    if (!keys[SDL_SCANCODE_D] && mBox->pressedKeys[3]){
        mBox->pressedKeys[3] = false;
    }
    if (keys[SDL_SCANCODE_C] && !mBox->pressedKeys[4]){
        mBox->pressedKeys[4] = true;
    }
    if (!keys[SDL_SCANCODE_C] && mBox->pressedKeys[4]){
        mBox->pressedKeys[4] = false;
    }


    /*if (keys[SDL_SCANCODE_D]) {
//        mBox->putMidiNoteInQueue(3);
        mBox->putMidiNoteInMainBuffer(3);
        mBox->putMidiNoteInMainBuffer(3);
    }
    if (keys[SDL_SCANCODE_C]) {
//        mBox->putMidiNoteInQueue(4);
//        mBox->putMidiNoteInMainBuffer(4);
        mBox->putMidiNoteInMainBuffer(4);
    }*/
}
