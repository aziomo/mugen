#include "../include/MainWindow.h"
#include "../include/CompositionMenu.h"
#include "../include/InstrumentMenu.h"
#include "../include/GraphPainter.h"

#define INST_MENU 1
#define COMP_MENU 2
#define OPTI_MENU 3

MainWindow::MainWindow(MusicBox *musicBox) {
    this->mBox = musicBox;
    initSDL();
    loadTextures();
    compositionMenu = new CompositionMenu(this);
    graphPainter = new GraphPainter(this);
    instrumentMenu = new InstrumentMenu(this);
    compositionMenu->init();
    instrumentMenu->init();
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

void MainWindow::renderTabs() {
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
    if (openTab != INST_MENU)
        SDL_RenderFillRect(renderer, &firstTab);
    if (openTab != COMP_MENU)
        SDL_RenderFillRect(renderer, &secondTab);
    if (openTab != OPTI_MENU)
        SDL_RenderFillRect(renderer, &thirdTab);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if (openTab != INST_MENU)
        SDL_RenderDrawRect(renderer, &firstTab);
    if (openTab != COMP_MENU)
        SDL_RenderDrawRect(renderer, &secondTab);
    if (openTab != OPTI_MENU)
        SDL_RenderDrawRect(renderer, &thirdTab);

    instrumentsTab.render(xByPercent(&instrumentsTab, 0.18),
                          yByPercent(&instrumentsTab, 0.055));
    compositionTab.render(xByPercent(&compositionTab, 0.5),
                          yByPercent(&compositionTab, 0.055));
    optionsTab.render(xByPercent(&optionsTab, 0.825),
                      yByPercent(&optionsTab, 0.055));
}

void MainWindow::render() {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &windowArea);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer, &mainArea);
    renderTabs();

    if (openTab == INST_MENU)
        instrumentMenu->render();
    if (openTab == COMP_MENU) {
        compositionMenu->render();
    }
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

    if (keyState[SDL_SCANCODE_F1] && !lastKeyState[SDL_SCANCODE_F1])
        openTab = 1;
    if (keyState[SDL_SCANCODE_F2] && !lastKeyState[SDL_SCANCODE_F2])
        openTab = 2;
    if (keyState[SDL_SCANCODE_F3] && !lastKeyState[SDL_SCANCODE_F3])
        openTab = 3;

    if (keyState[SDL_SCANCODE_2] && !lastKeyState[SDL_SCANCODE_2])
        passKeyPressToMenu(openTab, SDLK_2);

    if (keyState[SDL_SCANCODE_3] && !lastKeyState[SDL_SCANCODE_3])
        passKeyPressToMenu(openTab, SDLK_3);

    if (keyState[SDL_SCANCODE_W] && !lastKeyState[SDL_SCANCODE_W])
        passKeyPressToMenu(openTab, SDLK_w);

    if (keyState[SDL_SCANCODE_I] && !lastKeyState[SDL_SCANCODE_I])
        passKeyPressToMenu(openTab, SDLK_i);


    if (keyState[SDL_SCANCODE_UP] && !lastKeyState[SDL_SCANCODE_UP])
        passKeyPressToMenu(openTab, SDLK_UP);
    if (keyState[SDL_SCANCODE_UP] && lastKeyState[SDL_SCANCODE_UP] && keyPressState[SDL_SCANCODE_UP]++ > 10)
        passKeyPressToMenu(openTab, SDLK_UP);
    if (!keyState[SDL_SCANCODE_UP] && lastKeyState[SDL_SCANCODE_UP])
        keyPressState[SDL_SCANCODE_UP] = 0;

    if (keyState[SDL_SCANCODE_DOWN] && !lastKeyState[SDL_SCANCODE_DOWN])
        passKeyPressToMenu(openTab, SDLK_DOWN);
    if (keyState[SDL_SCANCODE_DOWN] && lastKeyState[SDL_SCANCODE_DOWN] && keyPressState[SDL_SCANCODE_DOWN]++ > 10)
        passKeyPressToMenu(openTab, SDLK_DOWN);
    if (!keyState[SDL_SCANCODE_DOWN] && lastKeyState[SDL_SCANCODE_DOWN])
        keyPressState[SDL_SCANCODE_DOWN] = 0;

    if (keyState[SDL_SCANCODE_LEFT] && !lastKeyState[SDL_SCANCODE_LEFT])
        passKeyPressToMenu(openTab, SDLK_LEFT);
    if (keyState[SDL_SCANCODE_LEFT] && lastKeyState[SDL_SCANCODE_LEFT] && keyPressState[SDL_SCANCODE_LEFT]++ > 10)
        passKeyPressToMenu(openTab, SDLK_LEFT);
    if (!keyState[SDL_SCANCODE_LEFT] && lastKeyState[SDL_SCANCODE_LEFT])
        keyPressState[SDL_SCANCODE_LEFT] = 0;

    if (keyState[SDL_SCANCODE_RIGHT] && !lastKeyState[SDL_SCANCODE_RIGHT])
        passKeyPressToMenu(openTab, SDLK_RIGHT);
    if (keyState[SDL_SCANCODE_RIGHT] && lastKeyState[SDL_SCANCODE_RIGHT] && keyPressState[SDL_SCANCODE_RIGHT]++ > 10)
        passKeyPressToMenu(openTab, SDLK_RIGHT);
    if (!keyState[SDL_SCANCODE_RIGHT] && lastKeyState[SDL_SCANCODE_RIGHT])
        keyPressState[SDL_SCANCODE_RIGHT] = 0;

    if (keyState[SDL_SCANCODE_SPACE] && !lastKeyState[SDL_SCANCODE_SPACE])
        passKeyPressToMenu(openTab, SDLK_SPACE);

    if (keyState[SDL_SCANCODE_RETURN] && !lastKeyState[SDL_SCANCODE_RETURN])
        passKeyPressToMenu(openTab, SDLK_RETURN);

}

void MainWindow::loadTextures() {
    instrumentsTab.loadFromText(renderer, "INSTRUMENTS", textColor, mainFont);
    compositionTab.loadFromText(renderer, "COMPOSITION", textColor, mainFont);
    optionsTab.loadFromText(renderer, "OPTIONS", textColor, mainFont);
}

int MainWindow::xByPercent(Texture* texture, double percent, Alignment align) const {
    switch (align) {
        case TO_RIGHT:
            return w * percent;
        case TO_LEFT:
            return w * percent - texture->w;
        case CENTER:
        default:
            return w * percent - texture->w * 0.5;
    }
}
int MainWindow::xByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    switch (align) {
        case TO_RIGHT:
            return w * percent;
        case TO_LEFT:
            return w * percent - rect->w;
        case CENTER:
        default:
            return w * percent - rect->w * 0.5;
    }
}
int MainWindow::yByPercent(Texture* texture, double percent, Alignment align) const {
    switch (align) {
        case TO_TOP:
            return w * percent - texture->h;
        case TO_BOTTOM:
            return w * percent;
        case CENTER:
        default:
            return w * percent - texture->h * 0.5;
    }
}
int MainWindow::yByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    switch (align) {
        case TO_TOP:
            return w * percent - rect->h;
        case TO_BOTTOM:
            return w * percent;
        case CENTER:
        default:
            return w * percent - rect->h * 0.5;
    }
}

void MainWindow::passKeyPressToMenu(int menu, SDL_Keycode key){
    switch (menu) {
        case INST_MENU:
            instrumentMenu->handleKeyPress(key); break;
        case COMP_MENU:
            compositionMenu->handleKeyPress(key); break;
        case OPTI_MENU:
        default:
            break;
    }
}
