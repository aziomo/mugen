//
// Created by alberto on 12/27/21.
//
#include "../include/CompositionMenu.h"

using std::string;
using std::to_string;

CompositionMenu::CompositionMenu(MainWindow *mainWindow) {
    textColor = {255, 255, 255};
    musicBox = mainWindow->mBox;
    window = mainWindow;
    renderer = mainWindow->renderer;
    timeline = new Timeline(this->renderer, window->mainArea.w - window->borderSize*2, window->mainArea.h/2);
    instrumentList = new ItemList(this->renderer, window->tinyFont, window->mainArea.w / 5, window->mainArea.w /5, 8);
    instrumentList->enumerate = true;
}

void CompositionMenu::init(){
    loadTextures();
}


void CompositionMenu::render(){
    updateTextures();
    timeline->render(window->borderSize*2, window->mainArea.h/2);
    instrumentList->render(window->mainArea.w * 4/5, window->mainArea.h * 1/5);
    instrumentListLabel.render(window->mainArea.w * 4/5, window->mainArea.h * 1/5 - instrumentListLabel.h);
    segmentsLabel.render(xByPercent(&segmentsLabel, 0.45, TO_LEFT),
                         yByPercent(&segmentsLabel, 0.15));
    rowsLabel.render(xByPercent(&rowsLabel, 0.45, TO_LEFT),
                         yByPercent(&rowsLabel, 0.2));
    tempoLabel.render(xByPercent(&tempoLabel, 0.45, TO_LEFT),
                         yByPercent(&tempoLabel, 0.25));

    segmentsValue.render(xByPercent(&segmentsLabel, 0.45, TO_LEFT) + segmentsLabel.w + segmentsValue.w / 2,
                         yByPercent(&segmentsValue, 0.15));

    rowsValue.render(xByPercent(&rowsLabel, 0.45, TO_LEFT) + rowsLabel.w + rowsValue.w / 2,
                     yByPercent(&rowsLabel, 0.2));

    tempoValue.render(xByPercent(&tempoLabel, 0.45, TO_LEFT) + tempoLabel.w + tempoValue.w/2,
                      yByPercent(&tempoValue, 0.25));


}

void CompositionMenu::loadTextures() {
    setTextTexture(&segmentsLabel, "Segments:", window->smallFont);
    setTextTexture(&rowsLabel, "Rows:", window->smallFont);
    setTextTexture(&tempoLabel, "Tempo:", window->smallFont);
    setTextTexture(&instrumentListLabel, "Instrument list", window->smallFont);
}

void CompositionMenu::loadControls(){
    tempoSelector.loadTextControl(SelectorType::INTEGER, &tempoValue, window);
    tempoSelector.setModifiedInteger(&timeline->tempo);
}

void CompositionMenu::updateTextures() {
    setTextTexture(&segmentsValue, to_string(timeline->segments.size()), window->smallFont);
    setTextTexture(&rowsValue, to_string(timeline->segments.front().rows.size()), window->smallFont);
    setTextTexture(&tempoValue, to_string(timeline->tempo), window->smallFont);
}


void CompositionMenu::setTextTexture(Texture* texture, string text) const {
    texture->loadFromText(renderer, text, textColor, window->mainFont);
}

void CompositionMenu::setTextTexture(Texture* texture, string text, TTF_Font* font) const {
    texture->loadFromText(renderer, text, textColor, font);
}



int CompositionMenu::xByPercent(Texture* texture, double percent, Alignment align) const {
    switch (align) {
        case TO_RIGHT:
            return window->w * percent;
        case TO_LEFT:
            return window->w * percent - texture->w;
        case CENTER:
        default:
            return window->w * percent - texture->w * 0.5;
    }
}
int CompositionMenu::xByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    switch (align) {
        case TO_RIGHT:
            return window->w * percent;
        case TO_LEFT:
            return window->w * percent - rect->w;
        case CENTER:
        default:
            return window->w * percent - rect->w * 0.5;
    }
}
int CompositionMenu::yByPercent(Texture* texture, double percent, Alignment align) const {
    switch (align) {
        case TO_TOP:
            return window->w * percent - texture->h;
        case TO_BOTTOM:
            return window->w * percent;
        case CENTER:
        default:
            return window->w * percent - texture->h * 0.5;
    }
}
int CompositionMenu::yByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    switch (align) {
        case TO_TOP:
            return window->w * percent - rect->h;
        case TO_BOTTOM:
            return window->w * percent;
        case CENTER:
        default:
            return window->w * percent - rect->h * 0.5;
    }
}