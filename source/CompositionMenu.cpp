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
    timeline = new Timeline(this->renderer, window->smallFont, window->mainArea.w - window->borderSize*2, window->mainArea.h/3, 8);
    instrumentList = new ItemList(this->renderer, window->tinyFont, window->mainArea.w / 5, window->mainArea.w /5, 8);
    instrumentList->enumerate = true;
}

void CompositionMenu::init(){
    loadTextures();
    loadControls();
    loadExampleBits();
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

    segmentsSelector.render(xByPercent(&segmentsValue, 0.47, TO_RIGHT),
                         yByPercent(&segmentsValue, 0.15));

    rowsSelector.render(xByPercent(&rowsValue, 0.47, TO_RIGHT),
                     yByPercent(&rowsLabel, 0.2));

    tempoSelector.render(xByPercent(&tempoValue, 0.47, TO_RIGHT),
                      yByPercent(&tempoValue, 0.25));

}

void CompositionMenu::addSegment(){
    timeline->segments.emplace_back(timeline->segments.front().rows.size());
}

void CompositionMenu::removeSegment(){
    if (timeline->segments.size() > 1){
        timeline->segments.pop_back();
    }
}

void CompositionMenu::addRow(){
    for (auto& segment : timeline->segments)
        segment.rows.emplace_back();
}

void CompositionMenu::removeRow(){
    if (timeline->segments.front().rows.size() > 1)
        for (auto& segment : timeline->segments)
            segment.rows.pop_back();
}


void CompositionMenu::loadTextures() {
    setTextTexture(&segmentsLabel, "Segments:", window->mainFont);
    setTextTexture(&rowsLabel, "Rows:", window->mainFont);
    setTextTexture(&tempoLabel, "Tempo:", window->mainFont);
    setTextTexture(&instrumentListLabel, "Instrument list", window->mainFont);
}

void CompositionMenu::loadControls(){
    tempoSelector.loadTextControl(SelectorType::INTEGER, &tempoValue, window);
    tempoSelector.setModifiedInteger(&timeline->tempo);
    segmentsSelector.loadTextControl(SelectorType::FUNCTION, &segmentsValue, window);
    segmentsSelector.setIncrementFunction(&CompositionMenu::addSegment);
    segmentsSelector.setDecrementFunction(&CompositionMenu::removeSegment);
    rowsSelector.loadTextControl(SelectorType::FUNCTION, &rowsValue, window);
    rowsSelector.setIncrementFunction(&CompositionMenu::addRow);
    rowsSelector.setDecrementFunction(&CompositionMenu::removeRow);

    segmentsSelector.isHighlighted = true;

}

void CompositionMenu::updateTextures() {
    setTextTexture(&segmentsValue, to_string(timeline->segments.size()), window->mainFont);
    setTextTexture(&rowsValue, to_string(timeline->segments.front().rows.size()), window->mainFont);
    setTextTexture(&tempoValue, to_string(timeline->tempo), window->mainFont);
}


void CompositionMenu::setTextTexture(Texture* texture, string text) const {
    texture->loadFromText(renderer, text, textColor, window->mainFont);
}

void CompositionMenu::setTextTexture(Texture* texture, string text, TTF_Font* font) const {
    texture->loadFromText(renderer, text, textColor, font);
}

int CompositionMenu::xByPercent(Texture* texture, double percent, Alignment align) const {
    window->xByPercent(texture, percent, align);
}
int CompositionMenu::xByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    window->xByPercent(rect, percent, align);
}
int CompositionMenu::yByPercent(Texture* texture, double percent, Alignment align) const {
    window->yByPercent(texture, percent, align);
}
int CompositionMenu::yByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    window->yByPercent(rect, percent, align);
}

void CompositionMenu::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::UP);
            }
            if (getFocusedControl()->isEditing) {
                getFocusedControl()->increment(true);
            }
            break;
        case SDLK_DOWN:
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::DOWN);
            }
            if (getFocusedControl()->isEditing) {
                getFocusedControl()->decrement(true);
            }
            break;
        case SDLK_LEFT:

            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::LEFT);
            }
            if (getFocusedControl()->isEditing) {
                getFocusedControl()->decrement(false);
            }
            break;
        case SDLK_RIGHT:
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::RIGHT);
            }
            if (getFocusedControl()->isEditing) {
                getFocusedControl()->increment(false);
            }
            break;

        case SDLK_SPACE:
            playbackTimeline();
            break;

        case SDLK_RETURN:
            selectFocusedControl();
            break;

        default:
            break;
    }
}

Control* CompositionMenu::getFocusedControl() {
    return controlArray[focusedControlRow][focusedControlCol];
}

void CompositionMenu::changeControlFocus(Direction direction) {
    getFocusedControl()->switchHighlight();
    switch (direction) {
        case Direction::UP:
            if (focusedControlRow > 0) {
                if (controlArray[focusedControlRow - 1][focusedControlCol] != nullptr) {
                    focusedControlRow -= 1;
                } else {
                    for (int i = 0; i < controlMatrixRows; i++) {
                        if (controlArray[focusedControlRow - 1][i] != nullptr) {
                            focusedControlRow -= 1;
                            focusedControlCol = i;
                            break;
                        }
                    }
                }
            }
            break;
        case Direction::DOWN:
            if (focusedControlRow < controlMatrixRows - 1) {
                if (controlArray[focusedControlRow + 1][focusedControlCol] != nullptr) {
                    focusedControlRow += 1;
                } else {
                    for (int i = 0; i < controlMatrixCols; i++) {
                        if (controlArray[focusedControlRow + 1][i] != nullptr) {
                            focusedControlRow += 1;
                            focusedControlCol = i;
                            break;
                        }
                    }
                }
            }
            break;
        case Direction::LEFT:

            if (focusedControlCol > 0) {
                if (controlArray[focusedControlRow][focusedControlCol - 1] != nullptr) {
                    focusedControlCol -= 1;
                } else {
                    for (int i = 0; i < controlMatrixRows; i++) {
                        if (controlArray[i][focusedControlCol - 1] != nullptr) {
                            focusedControlRow = i;
                            focusedControlCol -= 1;
                            break;
                        }
                    }
                }
            }
            break;
        case Direction::RIGHT:
            if (focusedControlCol < controlMatrixCols - 1) {
                if (controlArray[focusedControlRow][focusedControlCol + 1] != nullptr) {
                    focusedControlCol += 1;
                } else {
                    for (int i = 0; i < controlMatrixRows; i++) {
                        if (controlArray[i][focusedControlCol + 1] != nullptr) {
                            focusedControlRow = i;
                            focusedControlCol += 1;
                            break;
                        }
                    }
                }
            }
            break;
    }
    getFocusedControl()->switchHighlight();
}

void CompositionMenu::selectFocusedControl() {
    getFocusedControl()->activate();
}

void CompositionMenu::loadExampleBits() {
    auto* rows = &timeline->segments.front().rows;
    auto* firstRow = &rows->front();
    auto* instrument = musicBox->instruments.front();

    for (int i = 0; i < rows->size(); i++){
        auto* bit = new Bit(60 - i * 3, instrument);
        rows->at(i).bits[0] = bit;
        if (i % 2 == 1) {
            auto* bitto = new Bit(80 - i * 3, instrument);
            rows->at(i).bits[1] = bitto;
        }
    }

}

void CompositionMenu::playbackTimeline(){

    double globalTime = musicBox->globalTime;
    double beginTime = globalTime;
    double timeBetweenRows = 0.5;
    double timeElapsed = 0.0;
    double lastRowTriggerTime = -timeBetweenRows;

    double songLength = timeline->segments.front().rows.size() * timeBetweenRows;

    while (timeElapsed < songLength){

        if (timeElapsed - lastRowTriggerTime > timeBetweenRows){

            int rowsElapsed = timeElapsed / timeBetweenRows;
            Row* currentRow = &timeline->segments.front().rows.at(rowsElapsed);

            for (auto bit : currentRow->bits){
                if (bit != nullptr){

                    bit->note.pressedOnTime = globalTime;
                    bit->note.isAudible = true;
                    bitsPlayed.push_back(bit);
                }
            }

            if (rowsElapsed > 0){
                Row* previousRow = &timeline->segments.front().rows.at(rowsElapsed-1);
                for (auto bit : previousRow->bits){
                    if (bit != nullptr){
                        bit->note.releasedOnTime = globalTime;
                    }
                }
            }

            lastRowTriggerTime = timeElapsed;
        }

        auto bit = bitsPlayed.begin();
        while (bit != bitsPlayed.end()){
            if (!(*bit)->note.isAudible){
                bit = bitsPlayed.erase(bit);
            } else {
                ++bit;
            }
        }

        musicBox->writeBitsToBuffer(&bitsPlayed);
        globalTime = musicBox->globalTime;
        timeElapsed = globalTime - beginTime;
    }

    for (auto bit : bitsPlayed){
        if (bit != nullptr){
            bit->note.releasedOnTime = globalTime;
        }
    }
    bitsPlayed.clear();

}
