#include "../include/MainWindow.h"
#include "../include/CompositionMenu.h"
#include "../include/InstrumentMenu.h"
#include "../include/OptionsMenu.h"

#define INST_MENU 1
#define COMP_MENU 2
#define OPTI_MENU 3

MainWindow::MainWindow(MusicBox *musicBox) {
    this->musicBox = musicBox;
    initSDL();
    loadTextures();
    compositionMenu = new CompositionMenu(this);
    instrumentMenu = new InstrumentMenu(this);
    optionsMenu = new OptionsMenu(this);
    compositionMenu->init();
    instrumentMenu->init();
    optionsMenu->init();
}

MainWindow::~MainWindow() {
    delete instrumentMenu;
    musicBox = nullptr;
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
            printf("Failed to load the font\n");
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
    helpBar.render(borderSize * 2, mainArea.h - helpBar.h);

    if (openTab == INST_MENU)
        instrumentMenu->render();
    if (openTab == COMP_MENU)
        compositionMenu->render();
    if (openTab == OPTI_MENU)
        optionsMenu->render();

    SDL_RenderPresent(renderer);
}

void MainWindow::handleKeyPress(const Uint8 *keyState, bool *lastKeyState, int *keyPressState) {

    if (keyState[SDL_SCANCODE_Z] && !lastKeyState[SDL_SCANCODE_Z]){
        passKeyPressToMenu(openTab, SDLK_z);
        musicBox->pressNoteKey(0);
    }
    if (!keyState[SDL_SCANCODE_Z] && lastKeyState[SDL_SCANCODE_Z])
        musicBox->releaseNoteKey(0);

    if (keyState[SDL_SCANCODE_S] && !lastKeyState[SDL_SCANCODE_S]){
        passKeyPressToMenu(openTab, SDLK_s);
        musicBox->pressNoteKey(1);
    }
    if (!keyState[SDL_SCANCODE_S] && lastKeyState[SDL_SCANCODE_S])
        musicBox->releaseNoteKey(1);

    if (keyState[SDL_SCANCODE_X] && !lastKeyState[SDL_SCANCODE_X]){
        passKeyPressToMenu(openTab, SDLK_x);
        musicBox->pressNoteKey(2);
    }
    if (!keyState[SDL_SCANCODE_X] && lastKeyState[SDL_SCANCODE_X])
        musicBox->releaseNoteKey(2);

    if (keyState[SDL_SCANCODE_D] && !lastKeyState[SDL_SCANCODE_D]){
        passKeyPressToMenu(openTab, SDLK_d);
        musicBox->pressNoteKey(3);
    }
    if (!keyState[SDL_SCANCODE_D] && lastKeyState[SDL_SCANCODE_D])
        musicBox->releaseNoteKey(3);

    if (keyState[SDL_SCANCODE_C] && !lastKeyState[SDL_SCANCODE_C]){
        passKeyPressToMenu(openTab, SDLK_c);
        musicBox->pressNoteKey(4);
    }
    if (!keyState[SDL_SCANCODE_C] && lastKeyState[SDL_SCANCODE_C])
        musicBox->releaseNoteKey(4);

    if (keyState[SDL_SCANCODE_V] && !lastKeyState[SDL_SCANCODE_V]){
        passKeyPressToMenu(openTab, SDLK_v);
        musicBox->pressNoteKey(5);
    }
    if (!keyState[SDL_SCANCODE_V] && lastKeyState[SDL_SCANCODE_V])
        musicBox->releaseNoteKey(5);

    if (keyState[SDL_SCANCODE_G] && !lastKeyState[SDL_SCANCODE_G]){
        passKeyPressToMenu(openTab, SDLK_v);
        musicBox->pressNoteKey(6);
    }
    if (!keyState[SDL_SCANCODE_G] && lastKeyState[SDL_SCANCODE_G])
        musicBox->releaseNoteKey(6);

    if (keyState[SDL_SCANCODE_B] && !lastKeyState[SDL_SCANCODE_B]){
        passKeyPressToMenu(openTab, SDLK_b);
        musicBox->pressNoteKey(7);
    }
    if (!keyState[SDL_SCANCODE_B] && lastKeyState[SDL_SCANCODE_B])
        musicBox->releaseNoteKey(7);

    if (keyState[SDL_SCANCODE_H] && !lastKeyState[SDL_SCANCODE_H]){
        passKeyPressToMenu(openTab, SDLK_h);
        musicBox->pressNoteKey(8);
    }
    if (!keyState[SDL_SCANCODE_H] && lastKeyState[SDL_SCANCODE_H])
        musicBox->releaseNoteKey(8);

    if (keyState[SDL_SCANCODE_N] && !lastKeyState[SDL_SCANCODE_N]){
        passKeyPressToMenu(openTab, SDLK_n);
        musicBox->pressNoteKey(9);
    }
    if (!keyState[SDL_SCANCODE_N] && lastKeyState[SDL_SCANCODE_N])
        musicBox->releaseNoteKey(9);

    if (keyState[SDL_SCANCODE_J] && !lastKeyState[SDL_SCANCODE_J]){
        passKeyPressToMenu(openTab, SDLK_j);
        musicBox->pressNoteKey(10);
    }
    if (!keyState[SDL_SCANCODE_J] && lastKeyState[SDL_SCANCODE_J])
        musicBox->releaseNoteKey(10);

    if (keyState[SDL_SCANCODE_M] && !lastKeyState[SDL_SCANCODE_M]){
        passKeyPressToMenu(openTab, SDLK_m);
        musicBox->pressNoteKey(11);
    }
    if (!keyState[SDL_SCANCODE_M] && lastKeyState[SDL_SCANCODE_M])
        musicBox->releaseNoteKey(11);

    if (keyState[SDL_SCANCODE_COMMA] && !lastKeyState[SDL_SCANCODE_COMMA]){
        passKeyPressToMenu(openTab, SDLK_COMMA);
        musicBox->pressNoteKey(12);
    }
    if (!keyState[SDL_SCANCODE_COMMA] && lastKeyState[SDL_SCANCODE_COMMA])
        musicBox->releaseNoteKey(12);

    if (keyState[SDL_SCANCODE_L] && !lastKeyState[SDL_SCANCODE_L]){
        passKeyPressToMenu(openTab, SDLK_l);
        musicBox->pressNoteKey(13);
    }
    if (!keyState[SDL_SCANCODE_L] && lastKeyState[SDL_SCANCODE_L])
        musicBox->releaseNoteKey(13);

    if (keyState[SDL_SCANCODE_PERIOD] && !lastKeyState[SDL_SCANCODE_PERIOD]){
        passKeyPressToMenu(openTab, SDLK_PERIOD);
        musicBox->pressNoteKey(14);
    }
    if (!keyState[SDL_SCANCODE_PERIOD] && lastKeyState[SDL_SCANCODE_PERIOD])
        musicBox->releaseNoteKey(14);

    if (keyState[SDL_SCANCODE_SEMICOLON] && !lastKeyState[SDL_SCANCODE_SEMICOLON]){
        passKeyPressToMenu(openTab, SDLK_SEMICOLON);
        musicBox->pressNoteKey(15);
    }
    if (!keyState[SDL_SCANCODE_SEMICOLON] && lastKeyState[SDL_SCANCODE_SEMICOLON])
        musicBox->releaseNoteKey(15);

    if (keyState[SDL_SCANCODE_SLASH] && !lastKeyState[SDL_SCANCODE_SLASH]){
        passKeyPressToMenu(openTab, SDLK_SLASH);
        musicBox->pressNoteKey(16);
    }
    if (!keyState[SDL_SCANCODE_SLASH] && lastKeyState[SDL_SCANCODE_SLASH])
        musicBox->releaseNoteKey(16);

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

    if (keyState[SDL_SCANCODE_EQUALS] && !lastKeyState[SDL_SCANCODE_EQUALS])
        passKeyPressToMenu(openTab, SDLK_EQUALS);
    if (keyState[SDL_SCANCODE_EQUALS] && lastKeyState[SDL_SCANCODE_EQUALS] && keyPressState[SDL_SCANCODE_EQUALS]++ > 10)
        passKeyPressToMenu(openTab, SDLK_EQUALS);
    if (!keyState[SDL_SCANCODE_EQUALS] && lastKeyState[SDL_SCANCODE_EQUALS])
        keyPressState[SDL_SCANCODE_EQUALS] = 0;

    if (keyState[SDL_SCANCODE_MINUS] && !lastKeyState[SDL_SCANCODE_MINUS])
        passKeyPressToMenu(openTab, SDLK_MINUS);
    if (keyState[SDL_SCANCODE_MINUS] && lastKeyState[SDL_SCANCODE_MINUS] && keyPressState[SDL_SCANCODE_MINUS]++ > 10)
        passKeyPressToMenu(openTab, SDLK_MINUS);
    if (!keyState[SDL_SCANCODE_MINUS] && lastKeyState[SDL_SCANCODE_MINUS])
        keyPressState[SDL_SCANCODE_MINUS] = 0;

    if (keyState[SDL_SCANCODE_LSHIFT] && !lastKeyState[SDL_SCANCODE_LSHIFT])
        registerShiftPress(openTab, true);
    if (!keyState[SDL_SCANCODE_LSHIFT])
        registerShiftPress(openTab, false);


    if (keyState[SDL_SCANCODE_PAGEUP] && !lastKeyState[SDL_SCANCODE_PAGEUP])
        musicBox->octaveUp();

    if (keyState[SDL_SCANCODE_PAGEDOWN] && !lastKeyState[SDL_SCANCODE_PAGEDOWN])
        musicBox->octaveDown();

    if (keyState[SDL_SCANCODE_TAB] && !lastKeyState[SDL_SCANCODE_TAB])
        passKeyPressToMenu(openTab, SDLK_TAB);

    if (keyState[SDL_SCANCODE_SPACE] && !lastKeyState[SDL_SCANCODE_SPACE])
        passKeyPressToMenu(openTab, SDLK_SPACE);

    if (keyState[SDL_SCANCODE_RETURN] && !lastKeyState[SDL_SCANCODE_RETURN])
        passKeyPressToMenu(openTab, SDLK_RETURN);

    if (keyState[SDL_SCANCODE_DELETE] && !lastKeyState[SDL_SCANCODE_DELETE])
        passKeyPressToMenu(openTab, SDLK_DELETE);

    if (keyState[SDL_SCANCODE_BACKSPACE] && !lastKeyState[SDL_SCANCODE_BACKSPACE])
        passKeyPressToMenu(openTab, SDLK_BACKSPACE);


}

void MainWindow::loadTextures() {
    instrumentsTab.loadFromText(renderer, "INSTRUMENTY", textColor, mainFont);
    compositionTab.loadFromText(renderer, "KOMPOZYCJA", textColor, mainFont);
    optionsTab.loadFromText(renderer, "OPCJE", textColor, mainFont);
}

void MainWindow::setHelpBarText(const std::string& text){
    helpBar.loadFromText(renderer, text, textColor, tinyFont);
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
            return h * percent - texture->h;
        case TO_BOTTOM:
            return h * percent;
        case CENTER:
        default:
            return h * percent - texture->h * 0.5;
    }
}
int MainWindow::yByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    switch (align) {
        case TO_TOP:
            return h * percent - rect->h;
        case TO_BOTTOM:
            return h * percent;
        case CENTER:
        default:
            return h * percent - rect->h * 0.5;
    }
}

void MainWindow::registerShiftPress(int menu, bool shiftPressed)
{
    switch (menu) {
        case COMP_MENU:
            compositionMenu->registerShiftPress(shiftPressed); break;
        default:
            break;
    }
}

void MainWindow::passKeyPressToMenu(int menu, SDL_Keycode key){
    switch (menu) {
        case INST_MENU:
            instrumentMenu->handleKeyPress(key); break;
        case COMP_MENU:
            compositionMenu->handleKeyPress(key); break;
        case OPTI_MENU:
            optionsMenu->handleKeyPress(key); break;
        default:
            break;
    }
}


