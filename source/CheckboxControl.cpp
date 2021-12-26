#include "../include/CheckboxControl.h"
#include "../include/InstrumentMenu.h"

using std::max;

void CheckboxControl::switchCheck() {
    isChecked = !isChecked;
}

void CheckboxControl::activate() {
    switchCheck();
    triggerOnCheck();
}

void CheckboxControl::render(int x, int y) {
    if (isHighlighted){
        highlightRect = { x - borderSize*2, y - borderSize*2,
                          renderArea.w,
                          renderArea.h};
        bgRect = {x - borderSize, y - borderSize,
                  renderArea.w - borderSize * 2,
                  renderArea.h - borderSize * 2};
        SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(rend, &highlightRect);
        SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(rend, &bgRect);
    }
    labelTexture->render(x, y);
    if (horizontal){
        renderCheckbox(x + labelTexture->w + borderSize*4, y + borderSize*2);
    } else {
        renderCheckbox(x + labelTexture->w * 0.5 - checkTexture->w * 0.5, y + labelTexture->h);
    }
}

void CheckboxControl::renderCheckbox(int x, int y){
    checkboxBorderRect.x = x - borderSize * 2;
    checkboxBorderRect.y = y - borderSize * 2;
    checkboxBgRect.x = x - borderSize;
    checkboxBgRect.y = y - borderSize;

    SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(rend, &checkboxBorderRect);
    SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(rend, &checkboxBgRect);
    if (isChecked) {
        checkTexture->render(x, y);
    }
}

void CheckboxControl::loadTextures(Texture* labelTexture, MainWindow* window, bool horizontal){
    menu = window->instrumentMenu;
    this->rend = window->renderer;
    this->checkTexture = &window->instrumentMenu->checkImg;
    this->labelTexture = labelTexture;
    this->horizontal = horizontal;

    checkboxBorderRect.w = checkTexture->w + borderSize*4;
    checkboxBorderRect.h = checkTexture->h + borderSize*4;
    checkboxBgRect.w = checkTexture->w + borderSize*2;
    checkboxBgRect.h = checkTexture->h + borderSize*2;

    if (horizontal){
        renderArea.w = labelTexture->w + checkboxBorderRect.w + borderSize * 6;
        renderArea.h = checkboxBorderRect.h + borderSize * 4;
    }
    else {
        renderArea.w = labelTexture->w + borderSize*4,
        renderArea.h = labelTexture->h + checkboxBorderRect.w + borderSize*4;
    }
//    labelTexture->loadFromText(window->renderer, text, window->instrumentMenu->textColor, window->tinyFont);
}


void CheckboxControl::triggerOnCheck() {
    (menu->*onCheck)();
}
