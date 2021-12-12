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

void MainWindow::handleNewKeyPress(const Uint8 *keyState, bool* lastKeyState, int* keyPressState) {


    if (keyState[SDL_SCANCODE_Z] && !lastKeyState[SDL_SCANCODE_Z])
        mBox->pressedKeys[0] = true;
    if (!keyState[SDL_SCANCODE_Z] && lastKeyState[SDL_SCANCODE_Z])
        mBox->pressedKeys[0] = false;

    if (keyState[SDL_SCANCODE_S] && !lastKeyState[SDL_SCANCODE_S])
        mBox->pressedKeys[1] = true;
    if (!keyState[SDL_SCANCODE_S] && lastKeyState[SDL_SCANCODE_S])
        mBox->pressedKeys[1] = false;

    if (keyState[SDL_SCANCODE_X] && !lastKeyState[SDL_SCANCODE_X])
        mBox->pressedKeys[2] = true;
    if (!keyState[SDL_SCANCODE_X] && lastKeyState[SDL_SCANCODE_X])
        mBox->pressedKeys[2] = false;

    if (keyState[SDL_SCANCODE_D] && !lastKeyState[SDL_SCANCODE_D])
        mBox->pressedKeys[3] = true;
    if (!keyState[SDL_SCANCODE_D] && lastKeyState[SDL_SCANCODE_D])
        mBox->pressedKeys[3] = false;

    if (keyState[SDL_SCANCODE_C] && !lastKeyState[SDL_SCANCODE_C])
        mBox->pressedKeys[4] = true;
    if (!keyState[SDL_SCANCODE_C] && lastKeyState[SDL_SCANCODE_C])
        mBox->pressedKeys[4] = false;//

    if (keyState[SDL_SCANCODE_V] && !lastKeyState[SDL_SCANCODE_V])
        mBox->pressedKeys[5] = true;
    if (!keyState[SDL_SCANCODE_V] && lastKeyState[SDL_SCANCODE_V])
        mBox->pressedKeys[5] = false;

    if (keyState[SDL_SCANCODE_G] && !lastKeyState[SDL_SCANCODE_G])
        mBox->pressedKeys[6] = true;
    if (!keyState[SDL_SCANCODE_G] && lastKeyState[SDL_SCANCODE_G])
        mBox->pressedKeys[6] = false;

    if (keyState[SDL_SCANCODE_B] && !lastKeyState[SDL_SCANCODE_B])
        mBox->pressedKeys[7] = true;
    if (!keyState[SDL_SCANCODE_B] && lastKeyState[SDL_SCANCODE_B])
        mBox->pressedKeys[7] = false;

    if (keyState[SDL_SCANCODE_H] && !lastKeyState[SDL_SCANCODE_H])
        mBox->pressedKeys[8] = true;
    if (!keyState[SDL_SCANCODE_H] && lastKeyState[SDL_SCANCODE_H])
        mBox->pressedKeys[8] = false;

    if (keyState[SDL_SCANCODE_N] && !lastKeyState[SDL_SCANCODE_N])
        mBox->pressedKeys[9] = true;
    if (!keyState[SDL_SCANCODE_N] && lastKeyState[SDL_SCANCODE_N])
        mBox->pressedKeys[9] = false;

    if (keyState[SDL_SCANCODE_J] && !lastKeyState[SDL_SCANCODE_J])
        mBox->pressedKeys[10] = true;
    if (!keyState[SDL_SCANCODE_J] && lastKeyState[SDL_SCANCODE_J])
        mBox->pressedKeys[10] = false;

    if (keyState[SDL_SCANCODE_M] && !lastKeyState[SDL_SCANCODE_M])
        mBox->pressedKeys[11] = true;
    if (!keyState[SDL_SCANCODE_M] && lastKeyState[SDL_SCANCODE_M])
        mBox->pressedKeys[11] = false;

    if (keyState[SDL_SCANCODE_COMMA] && !lastKeyState[SDL_SCANCODE_COMMA])
        mBox->pressedKeys[12] = true;
    if (!keyState[SDL_SCANCODE_COMMA] && lastKeyState[SDL_SCANCODE_COMMA])
        mBox->pressedKeys[12] = false;

    if (keyState[SDL_SCANCODE_L] && !lastKeyState[SDL_SCANCODE_L])
        mBox->pressedKeys[13] = true;
    if (!keyState[SDL_SCANCODE_L] && lastKeyState[SDL_SCANCODE_L])
        mBox->pressedKeys[13] = false;

    if (keyState[SDL_SCANCODE_PERIOD] && !lastKeyState[SDL_SCANCODE_PERIOD])
        mBox->pressedKeys[14] = true;
    if (!keyState[SDL_SCANCODE_PERIOD] && lastKeyState[SDL_SCANCODE_PERIOD])
        mBox->pressedKeys[14] = false;

    if (keyState[SDL_SCANCODE_SEMICOLON] && !lastKeyState[SDL_SCANCODE_SEMICOLON])
        mBox->pressedKeys[15] = true;
    if (!keyState[SDL_SCANCODE_SEMICOLON] && lastKeyState[SDL_SCANCODE_SEMICOLON])
        mBox->pressedKeys[15] = false;

    if (keyState[SDL_SCANCODE_SLASH] && !lastKeyState[SDL_SCANCODE_SLASH])
        mBox->pressedKeys[16] = true;
    if (!keyState[SDL_SCANCODE_SLASH] && lastKeyState[SDL_SCANCODE_SLASH])
        mBox->pressedKeys[16] = false;


    if (keyState[SDL_SCANCODE_2] && !lastKeyState[SDL_SCANCODE_2])
        waveformMenu->handleKeyPress(SDLK_2);
    if (keyState[SDL_SCANCODE_3] && !lastKeyState[SDL_SCANCODE_3])
        waveformMenu->handleKeyPress(SDLK_3);

    if (keyState[SDL_SCANCODE_W] && !lastKeyState[SDL_SCANCODE_W])
        waveformMenu->handleKeyPress(SDLK_w);


    if (keyState[SDL_SCANCODE_UP] && !lastKeyState[SDL_SCANCODE_UP])
        waveformMenu->handleKeyPress(SDLK_UP);
    if (keyState[SDL_SCANCODE_UP] && lastKeyState[SDL_SCANCODE_UP] && keyPressState[SDL_SCANCODE_UP]++ > 10)
        waveformMenu->handleKeyPress(SDLK_UP);
    if (!keyState[SDL_SCANCODE_UP] && lastKeyState[SDL_SCANCODE_UP])
        keyPressState[SDL_SCANCODE_UP] = 0;

    if (keyState[SDL_SCANCODE_DOWN] && !lastKeyState[SDL_SCANCODE_DOWN])
        waveformMenu->handleKeyPress(SDLK_DOWN);
    if (keyState[SDL_SCANCODE_DOWN] && lastKeyState[SDL_SCANCODE_DOWN] && keyPressState[SDL_SCANCODE_DOWN]++ > 10)
        waveformMenu->handleKeyPress(SDLK_DOWN);
    if (!keyState[SDL_SCANCODE_DOWN] && lastKeyState[SDL_SCANCODE_DOWN])
        keyPressState[SDL_SCANCODE_DOWN] = 0;

    if (keyState[SDL_SCANCODE_LEFT] && !lastKeyState[SDL_SCANCODE_LEFT])
        waveformMenu->handleKeyPress(SDLK_LEFT);
    if (keyState[SDL_SCANCODE_LEFT] && lastKeyState[SDL_SCANCODE_LEFT] && keyPressState[SDL_SCANCODE_LEFT]++ > 10)
        waveformMenu->handleKeyPress(SDLK_LEFT);
    if (!keyState[SDL_SCANCODE_LEFT] && lastKeyState[SDL_SCANCODE_LEFT])
        keyPressState[SDL_SCANCODE_LEFT] = 0;

    if (keyState[SDL_SCANCODE_RIGHT] && !lastKeyState[SDL_SCANCODE_RIGHT])
        waveformMenu->handleKeyPress(SDLK_RIGHT);
    if (keyState[SDL_SCANCODE_RIGHT] && lastKeyState[SDL_SCANCODE_RIGHT] && keyPressState[SDL_SCANCODE_RIGHT]++ > 10)
        waveformMenu->handleKeyPress(SDLK_RIGHT);
    if (!keyState[SDL_SCANCODE_RIGHT] && lastKeyState[SDL_SCANCODE_RIGHT])
        keyPressState[SDL_SCANCODE_RIGHT] = 0;

    if (keyState[SDL_SCANCODE_RETURN] && !lastKeyState[SDL_SCANCODE_RETURN])
        waveformMenu->handleKeyPress(SDLK_RETURN);

}
