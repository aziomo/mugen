#include "../include/MainWindow.h"
#include "../include/InstrumentMenu.h"

MainWindow::MainWindow(MusicBox *musicBox) {
    this->mBox = musicBox;
    initSDL();
    instrumentMenu = new InstrumentMenu(this);
    instrumentMenu->init();
    graphPainter = new GraphPainter(this);
}

MainWindow::~MainWindow() {
    delete instrumentMenu;
    mBox = nullptr;
    quitSDL();
}

void MainWindow::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("mugen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (window == nullptr || renderer == nullptr) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        TTF_Init();
        largeFont = TTF_OpenFont((assets_dir + "HoneyRoom.ttf").c_str(), 32);
        mainFont = TTF_OpenFont((assets_dir + "HoneyRoom.ttf").c_str(), 26);
        smallFont = TTF_OpenFont((assets_dir + "HoneyRoom.ttf").c_str(), 20);
        tinyFont = TTF_OpenFont((assets_dir + "HoneyRoom.ttf").c_str(), 16);
        if (mainFont == nullptr) {
            printf("font fucked up\n");
        }
    }
}

void MainWindow::quitSDL() {
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

void MainWindow::renderBorders() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Rect waveformMenuBorder = {0, 0, windowArea.w * 5 / 9, windowArea.h * 3 / 5};
    SDL_RenderDrawRect(renderer, &waveformMenuBorder);
}

void MainWindow::renderTabs() const{
    SDL_Rect firstTab, secondTab, thirdTab;
    firstTab = {borderSize,borderSize,
                mainArea.w / 3,
                h / 13
    };
    secondTab = {borderSize + (w -borderSize*2) / 3,
                 borderSize,
                 mainArea.w / 3,
                 h / 13
    };
    thirdTab = {borderSize + (w - borderSize*2)*2/3,
                borderSize,
                mainArea.w / 3,
                h / 13
    };
    SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0x33);
    SDL_RenderFillRect(renderer, &secondTab);
    SDL_RenderFillRect(renderer, &thirdTab);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

//    SDL_RenderDrawRect(renderer, &firstTab);
    SDL_RenderDrawRect(renderer, &secondTab);
    SDL_RenderDrawRect(renderer, &thirdTab);
}

void MainWindow::render() {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &windowArea);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer, &mainArea);
    renderTabs();
    instrumentMenu->render();
    SDL_RenderPresent(renderer);
}

void MainWindow::handleKeyPress(const Uint8 *keyState, bool *lastKeyState, int *keyPressState) {

    if (keyState[SDL_SCANCODE_Z] && !lastKeyState[SDL_SCANCODE_Z])
        mBox->pressNoteKey(0);
    if (!keyState[SDL_SCANCODE_Z] && lastKeyState[SDL_SCANCODE_Z])
        mBox->releaseNoteKey(0);

    if (keyState[SDL_SCANCODE_S] && !lastKeyState[SDL_SCANCODE_S])
        mBox->pressNoteKey(1);
    if (!keyState[SDL_SCANCODE_S] && lastKeyState[SDL_SCANCODE_S])
        mBox->releaseNoteKey(1);

    if (keyState[SDL_SCANCODE_X] && !lastKeyState[SDL_SCANCODE_X])
        mBox->pressNoteKey(2);
    if (!keyState[SDL_SCANCODE_X] && lastKeyState[SDL_SCANCODE_X])
        mBox->releaseNoteKey(2);

    if (keyState[SDL_SCANCODE_D] && !lastKeyState[SDL_SCANCODE_D])
        mBox->pressNoteKey(3);
    if (!keyState[SDL_SCANCODE_D] && lastKeyState[SDL_SCANCODE_D])
        mBox->releaseNoteKey(3);

    if (keyState[SDL_SCANCODE_C] && !lastKeyState[SDL_SCANCODE_C])
        mBox->pressNoteKey(4);
    if (!keyState[SDL_SCANCODE_C] && lastKeyState[SDL_SCANCODE_C])
        mBox->releaseNoteKey(4);

    if (keyState[SDL_SCANCODE_V] && !lastKeyState[SDL_SCANCODE_V])
        mBox->pressNoteKey(5);
    if (!keyState[SDL_SCANCODE_V] && lastKeyState[SDL_SCANCODE_V])
        mBox->releaseNoteKey(5);

    if (keyState[SDL_SCANCODE_G] && !lastKeyState[SDL_SCANCODE_G])
        mBox->pressNoteKey(6);
    if (!keyState[SDL_SCANCODE_G] && lastKeyState[SDL_SCANCODE_G])
        mBox->releaseNoteKey(6);

    if (keyState[SDL_SCANCODE_B] && !lastKeyState[SDL_SCANCODE_B])
        mBox->pressNoteKey(7);
    if (!keyState[SDL_SCANCODE_B] && lastKeyState[SDL_SCANCODE_B])
        mBox->releaseNoteKey(7);

    if (keyState[SDL_SCANCODE_H] && !lastKeyState[SDL_SCANCODE_H])
        mBox->pressNoteKey(8);
    if (!keyState[SDL_SCANCODE_H] && lastKeyState[SDL_SCANCODE_H])
        mBox->releaseNoteKey(8);

    if (keyState[SDL_SCANCODE_N] && !lastKeyState[SDL_SCANCODE_N])
        mBox->pressNoteKey(9);
    if (!keyState[SDL_SCANCODE_N] && lastKeyState[SDL_SCANCODE_N])
        mBox->releaseNoteKey(9);

    if (keyState[SDL_SCANCODE_J] && !lastKeyState[SDL_SCANCODE_J])
        mBox->pressNoteKey(10);
    if (!keyState[SDL_SCANCODE_J] && lastKeyState[SDL_SCANCODE_J])
        mBox->releaseNoteKey(10);

    if (keyState[SDL_SCANCODE_M] && !lastKeyState[SDL_SCANCODE_M])
        mBox->pressNoteKey(11);
    if (!keyState[SDL_SCANCODE_M] && lastKeyState[SDL_SCANCODE_M])
        mBox->releaseNoteKey(11);

    if (keyState[SDL_SCANCODE_COMMA] && !lastKeyState[SDL_SCANCODE_COMMA])
        mBox->pressNoteKey(12);
    if (!keyState[SDL_SCANCODE_COMMA] && lastKeyState[SDL_SCANCODE_COMMA])
        mBox->releaseNoteKey(12);

    if (keyState[SDL_SCANCODE_L] && !lastKeyState[SDL_SCANCODE_L])
        mBox->pressNoteKey(13);
    if (!keyState[SDL_SCANCODE_L] && lastKeyState[SDL_SCANCODE_L])
        mBox->releaseNoteKey(13);

    if (keyState[SDL_SCANCODE_PERIOD] && !lastKeyState[SDL_SCANCODE_PERIOD])
        mBox->pressNoteKey(14);
    if (!keyState[SDL_SCANCODE_PERIOD] && lastKeyState[SDL_SCANCODE_PERIOD])
        mBox->releaseNoteKey(14);

    if (keyState[SDL_SCANCODE_SEMICOLON] && !lastKeyState[SDL_SCANCODE_SEMICOLON])
        mBox->pressNoteKey(15);
    if (!keyState[SDL_SCANCODE_SEMICOLON] && lastKeyState[SDL_SCANCODE_SEMICOLON])
        mBox->releaseNoteKey(15);

    if (keyState[SDL_SCANCODE_SLASH] && !lastKeyState[SDL_SCANCODE_SLASH])
        mBox->pressNoteKey(16);
    if (!keyState[SDL_SCANCODE_SLASH] && lastKeyState[SDL_SCANCODE_SLASH])
        mBox->releaseNoteKey(16);


    if (keyState[SDL_SCANCODE_2] && !lastKeyState[SDL_SCANCODE_2])
        instrumentMenu->handleKeyPress(SDLK_2);
    if (keyState[SDL_SCANCODE_3] && !lastKeyState[SDL_SCANCODE_3])
        instrumentMenu->handleKeyPress(SDLK_3);

    if (keyState[SDL_SCANCODE_W] && !lastKeyState[SDL_SCANCODE_W])
        instrumentMenu->handleKeyPress(SDLK_w);

    if (keyState[SDL_SCANCODE_I] && !lastKeyState[SDL_SCANCODE_I])
        instrumentMenu->handleKeyPress(SDLK_i);


    if (keyState[SDL_SCANCODE_UP] && !lastKeyState[SDL_SCANCODE_UP])
        instrumentMenu->handleKeyPress(SDLK_UP);
    if (keyState[SDL_SCANCODE_UP] && lastKeyState[SDL_SCANCODE_UP] && keyPressState[SDL_SCANCODE_UP]++ > 10)
        instrumentMenu->handleKeyPress(SDLK_UP);
    if (!keyState[SDL_SCANCODE_UP] && lastKeyState[SDL_SCANCODE_UP])
        keyPressState[SDL_SCANCODE_UP] = 0;

    if (keyState[SDL_SCANCODE_DOWN] && !lastKeyState[SDL_SCANCODE_DOWN])
        instrumentMenu->handleKeyPress(SDLK_DOWN);
    if (keyState[SDL_SCANCODE_DOWN] && lastKeyState[SDL_SCANCODE_DOWN] && keyPressState[SDL_SCANCODE_DOWN]++ > 10)
        instrumentMenu->handleKeyPress(SDLK_DOWN);
    if (!keyState[SDL_SCANCODE_DOWN] && lastKeyState[SDL_SCANCODE_DOWN])
        keyPressState[SDL_SCANCODE_DOWN] = 0;

    if (keyState[SDL_SCANCODE_LEFT] && !lastKeyState[SDL_SCANCODE_LEFT])
        instrumentMenu->handleKeyPress(SDLK_LEFT);
    if (keyState[SDL_SCANCODE_LEFT] && lastKeyState[SDL_SCANCODE_LEFT] && keyPressState[SDL_SCANCODE_LEFT]++ > 10)
        instrumentMenu->handleKeyPress(SDLK_LEFT);
    if (!keyState[SDL_SCANCODE_LEFT] && lastKeyState[SDL_SCANCODE_LEFT])
        keyPressState[SDL_SCANCODE_LEFT] = 0;

    if (keyState[SDL_SCANCODE_RIGHT] && !lastKeyState[SDL_SCANCODE_RIGHT])
        instrumentMenu->handleKeyPress(SDLK_RIGHT);
    if (keyState[SDL_SCANCODE_RIGHT] && lastKeyState[SDL_SCANCODE_RIGHT] && keyPressState[SDL_SCANCODE_RIGHT]++ > 10)
        instrumentMenu->handleKeyPress(SDLK_RIGHT);
    if (!keyState[SDL_SCANCODE_RIGHT] && lastKeyState[SDL_SCANCODE_RIGHT])
        keyPressState[SDL_SCANCODE_RIGHT] = 0;

    if (keyState[SDL_SCANCODE_RETURN] && !lastKeyState[SDL_SCANCODE_RETURN])
        instrumentMenu->handleKeyPress(SDLK_RETURN);

}
