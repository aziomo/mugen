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

void MainWindow::renderBorders(){
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Rect waveformMenuBorder = {0,0, windowArea.w*5/9, windowArea.h*3/5};
    SDL_RenderDrawRect(renderer, &waveformMenuBorder);
}

void MainWindow::render(){
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &windowArea);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer, &mainArea);
    waveformMenu->render();
    waveformMenu->renderGraph();
//    renderBorders();
    SDL_RenderPresent(renderer);
}

void MainWindow::handleKeyPress(const Uint8 *keyState, bool* lastKeyState, int* keyPressState) {


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
        mBox->pressedKeys[4] = false;

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

    if (keyState[SDL_SCANCODE_I] && !lastKeyState[SDL_SCANCODE_I])
        waveformMenu->handleKeyPress(SDLK_i);


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
