#include "../include/CompositionMenu.h"

using std::string;
using std::to_string;

CompositionMenu::CompositionMenu(MainWindow *mainWindow) {
    textColor = {255, 255, 255};
    musicBox = mainWindow->musicBox;
    window = mainWindow;
    renderer = mainWindow->renderer;
    timeline = new Timeline(this->renderer, window->smallFont, 21, 45, window->mainArea.h*2/5, 8);
    instrumentList = new ItemList(this->renderer, window->tinyFont, window->mainArea.w/5, window->mainArea.w/6, 6, true);
    segmentManager = new SegmentManager(timeline, window->smallFont, window->mainArea.w/10, window->mainArea.w/6, 5);
}

void CompositionMenu::init(){
    loadTextures();
    loadControls();
    loadExampleBits();
}


void CompositionMenu::render(){
    updateTextures();
    timeline->render(xByPercent(&timeline->outline, 0.5),
            window->mainArea.h/2);
    segmentManager->render(xByPercent(&segmentManager->outline, 0.04, TO_RIGHT),
                           window->mainArea.h * 1/5);
    segmentManagerLabel.render(xByPercent(&segmentManager->outline, 0.04, TO_RIGHT),
                               window->mainArea.h * 1/5 - segmentManagerLabel.h);
    instrumentList->render(xByPercent(&instrumentList->outline, 0.864),
                           window->mainArea.h * 1/5);
    instrumentListLabel.render(xByPercent(&instrumentList->outline, 0.864),
                               window->mainArea.h * 1/5 - instrumentListLabel.h);


    segmentsLabel.render(xByPercent(&segmentsLabel, 0.5, TO_LEFT),
                         yByPercent(&segmentsLabel, 0.23));
    segmentsSelector.render(xByPercent(&segmentsValue, 0.55, CENTER),
                            yByPercent(&segmentsValue, 0.23));
    colsLabel.render(xByPercent(&colsLabel, 0.5, TO_LEFT),
                     yByPercent(&colsLabel, 0.28));
    colsSelector.render(xByPercent(&colsValue, 0.55, CENTER),
                        yByPercent(&colsLabel, 0.28));
    tempoLabel.render(xByPercent(&tempoLabel, 0.5, TO_LEFT),
                         yByPercent(&tempoLabel, 0.33));
    tempoSelector.render(xByPercent(&tempoValue, 0.55, CENTER),
                      yByPercent(&tempoValue, 0.33));

}

void CompositionMenu::addSegment() {
    timeline->songSegs.push_back(timeline->allSegs.front());
}

void CompositionMenu::removeSegment(){
    if (timeline->songSegs.size() > 1){
        timeline->songSegs.pop_back();
    }
    if (timeline->focusedSegmentIndex == timeline->songSegs.size()){
        timeline->focusedSegmentIndex--;
    }
}

void CompositionMenu::addColumn(){
    for (auto& segment : timeline->allSegs)
        segment->cols.push_back(new Column());
}

void CompositionMenu::removeColumn(){
    if (timeline->segColumns() > 1)
        for (auto& segment : timeline->allSegs)
            segment->cols.pop_back();
    if (timeline->focusedColIndex == timeline->segColumns()){
        timeline->focusedColIndex--;
    }
}


void CompositionMenu::loadTextures() {
    setTextTexture(&segmentsLabel, "Segments:", window->mainFont);
    setTextTexture(&colsLabel, "Columns:", window->mainFont);
    setTextTexture(&tempoLabel, "Tempo:", window->mainFont);
    setTextTexture(&instrumentListLabel, "Current instrument", window->smallFont);
    setTextTexture(&segmentManagerLabel, "Segments", window->smallFont);
}

void CompositionMenu::loadControls(){
    tempoSelector.loadTextControl(SelectorType::INTEGER, &tempoValue, window);
    tempoSelector.setModifiedInteger(&timeline->tempo);
    segmentsSelector.loadTextControl(SelectorType::FUNCTION, &segmentsValue, window);
    segmentsSelector.setIncrementFunction(&CompositionMenu::addSegment);
    segmentsSelector.setDecrementFunction(&CompositionMenu::removeSegment);
    colsSelector.loadTextControl(SelectorType::FUNCTION, &colsValue, window);
    colsSelector.setIncrementFunction(&CompositionMenu::addColumn);
    colsSelector.setDecrementFunction(&CompositionMenu::removeColumn);

    segmentsSelector.isHighlighted = true;
}

void CompositionMenu::updateTextures() {
    setTextTexture(&segmentsValue, to_string(timeline->songSegs.size()), window->mainFont);
    setTextTexture(&colsValue, to_string(timeline->songSegs.front()->cols.size()), window->mainFont);
    setTextTexture(&tempoValue, to_string(timeline->tempo), window->mainFont);
}

void CompositionMenu::setTextTexture(Texture* texture, const string& text) const {
    texture->loadFromText(renderer, text, textColor, window->mainFont);
}
void CompositionMenu::setTextTexture(Texture* texture, const string& text, TTF_Font* font) const {
    texture->loadFromText(renderer, text, textColor, font);
}
int CompositionMenu::xByPercent(Texture* texture, double percent, Alignment align) const {
    return window->xByPercent(texture, percent, align);
}
int CompositionMenu::xByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    return window->xByPercent(rect, percent, align);
}
int CompositionMenu::yByPercent(Texture* texture, double percent, Alignment align) const {
    return window->yByPercent(texture, percent, align);
}
int CompositionMenu::yByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    return window->yByPercent(rect, percent, align);
}

void CompositionMenu::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
            if (isTimelineFocused){
                timeline->move(Direction::UP);
            }
            else {
                if (getFocusedControl()->isHighlighted || isSegmentListFocused || isInstrumentListFocused) {
                    changeControlFocus(Direction::UP);
                }
                if (getFocusedControl()->isEditing) {
                    getFocusedControl()->increment(true);
                }
            }
            break;
        case SDLK_DOWN:
            if (isTimelineFocused){
                timeline->move(Direction::DOWN);
            }
            else {
                if (getFocusedControl()->isHighlighted || isSegmentListFocused || isInstrumentListFocused) {
                    changeControlFocus(Direction::DOWN);
                }
                if (getFocusedControl()->isEditing) {
                    getFocusedControl()->decrement(true);
                }
            }
            break;
        case SDLK_LEFT:
            if (isTimelineFocused){
                timeline->move(Direction::LEFT);
            }
            else {
                if (getFocusedControl()->isHighlighted || isInstrumentListFocused) {
                    changeControlFocus(Direction::LEFT);
                }
                if (getFocusedControl()->isEditing) {
                    getFocusedControl()->decrement(false);
                }
            }
            break;
        case SDLK_RIGHT:
            if (isTimelineFocused){
                timeline->move(Direction::RIGHT);
            }
            else {
                if (getFocusedControl()->isHighlighted || isSegmentListFocused ) {
                    changeControlFocus(Direction::RIGHT);
                }
                if (getFocusedControl()->isEditing) {
                    getFocusedControl()->increment(false);
                }
            }
            break;

        case SDLK_TAB:
            if (getFocusedControl()->isEditing){
                selectFocusedControl();
            }
            switchTimelineFocus();
            break;
        case SDLK_SPACE:
            if (timeline->editingMode){
                timeline->focusedColIndex++;
            } else {
                if (!playbackOn){
                    playbackTimeline();
                } else {
                    stopPlayback();
                }
            }
            break;
        case SDLK_RETURN:
            if (isTimelineFocused){
                timeline->editingMode = !timeline->editingMode;
            } else {
                selectFocusedControl();
            }
            break;

        case SDLK_z:
        case SDLK_s:
        case SDLK_x:
        case SDLK_d:
        case SDLK_c:
        case SDLK_v:
        case SDLK_g:
        case SDLK_b:
        case SDLK_h:
        case SDLK_n:
        case SDLK_j:
        case SDLK_m:
        case SDLK_COMMA:
        case SDLK_l:
        case SDLK_PERIOD:
            if (timeline->editingMode){
                auto* bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex)->bits[timeline->focusedBitIndex];
                if (*bitPtr == nullptr){
                    *bitPtr = new Bit(musicBox->keyToNoteValue(key), musicBox->instruments.at(instrumentList->selectedIndex));
                } else {
                    (*bitPtr)->note.frequency = midiToFreq(musicBox->keyToNoteValue(key));
                }
                timeline->focusedColIndex++;
            }
            break;
        case SDLK_DELETE:
            if (timeline->editingMode){
                auto* bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex)->bits[timeline->focusedBitIndex];
                delete *bitPtr;
                *bitPtr = nullptr;
                timeline->focusedColIndex++;
            }
            break;

        case SDLK_EQUALS:
            if (isSegmentListFocused){
                segmentManager->segmentUp();
            }
            break;
        case SDLK_MINUS:
            if (isSegmentListFocused){
                segmentManager->segmentDown();
            }
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
            if (isSegmentListFocused){
                segmentManager->moveUp();
            }
            else if (isInstrumentListFocused){
                instrumentList->moveUp();
                musicBox->currentInstrument = instrumentList->selectedIndex;
            }
            else if (focusedControlRow > 0) {
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
            if (isSegmentListFocused){
                segmentManager->moveDown();
            }
            else if (isInstrumentListFocused){
                instrumentList->moveDown();
                musicBox->currentInstrument = instrumentList->selectedIndex;
            }
            else if (focusedControlRow < controlMatrixRows - 1) {
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

            if (isInstrumentListFocused){
                // exit the list
                getFocusedControl()->switchHighlight();
                isInstrumentListFocused = false;
                instrumentList->isFocused = false;
            }
            else if (!isSegmentListFocused){
                // enter the list
                getFocusedControl()->switchHighlight();
                isSegmentListFocused = true;
                segmentManager->isFocused = true;
            }
            // the rest doesn't mean much right now
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

            if (isSegmentListFocused){
                // exit the list
                getFocusedControl()->switchHighlight();
                isSegmentListFocused = false;
                segmentManager->isFocused = false;
            }
            else if (!isInstrumentListFocused){
                // enter the list
                getFocusedControl()->switchHighlight();
                isInstrumentListFocused = true;
                instrumentList->isFocused = true;
            }
            // the rest doesn't mean much right now
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
    auto* cols = &timeline->songSegs.front()->cols;
    auto* instrument = musicBox->instruments.front();

    for (int i = 0; i < cols->size(); i++){
        timeline->songSegs.front()->cols.at(i)->bits[0] = new Bit(60 - i * 3, instrument);
//        if (i % 2 == 1) {
//            auto* bitto = new Bit(80 - i * 3, instrument);
//            cols->at(i)->bits[1] = bitto;
//        }
    }
    timeline->songSegs.front()->cols.at(1)->bits[1] = new Bit(60, instrument, 3, 0);
    timeline->songSegs.front()->cols.at(2)->bits[1] = new Bit(60, instrument, 3, 1);
    timeline->songSegs.front()->cols.at(3)->bits[1] = new Bit(60, instrument, 3, 2);
    timeline->songSegs.front()->cols.at(4)->bits[1] = new Bit(60, instrument, 3, 3);

}

void CompositionMenu::stopPlayback(){
    musicBox->stopPlaying();
    while (!musicBox->blocksBuffer.empty()){
        musicBox->blocksBuffer.pop();
    }
    musicBox->blocksAvailable = 0;
    musicBox->startPlaying();
    playbackOn = false;
}

void CompositionMenu::playbackTimeline(){

    double globalTime = musicBox->globalTime;
    double beginTime = globalTime;
    double timeBetweenCols = 60.0 / timeline->tempo;
    double timeElapsed = 0.0;
    double lastColTriggerTime = -timeBetweenCols;

    vector<Bit*> heldBits;

    double songLength = timeline->songSegs.front()->cols.size() * timeBetweenCols * timeline->songSegs.size();

    while (timeElapsed < songLength){

        if (timeElapsed - lastColTriggerTime > timeBetweenCols){

            int colsElapsed = timeElapsed / timeBetweenCols;
            Column* currentCol = timeline->songSegs.front()->cols.at(colsElapsed);


            for (auto bit : heldBits){
                if (bit->holdSection == bit->holdTicks){
                    bit->note.releasedOnTime = globalTime;
                    bit->holdSection = 0;
                }
                else {
                    bit->holdSection++;
                }
            }

            for (auto bit : currentCol->bits){
                if (bit != nullptr){
                    if (bit->holdSection == 0 && bit->holdTicks > 0){
                        heldBits.push_back(bit);
                    }

                    bit->note.pressedOnTime = globalTime;
                    bit->note.isAudible = true;
                    bitsPlayed.push_back(bit);
                }
            }

            if (colsElapsed > 0){
                Column* previousCol = timeline->songSegs.front()->cols.at(colsElapsed - 1);
                for (auto bit : previousCol->bits){
                    if (bit != nullptr && bit->holdTicks == 0){
                        bit->note.releasedOnTime = globalTime;
                    }
                }
            }

            lastColTriggerTime = timeElapsed;
        }

        auto bitIterator = heldBits.begin();
        while (bitIterator != heldBits.end()){
            if (!(*bitIterator)->note.isAudible){
                bitIterator = heldBits.erase(bitIterator);
            } else {
                ++bitIterator;
            }
        }

        bitIterator = bitsPlayed.begin();
        while (bitIterator != bitsPlayed.end()){
            if (!(*bitIterator)->note.isAudible){
                bitIterator = bitsPlayed.erase(bitIterator);
            } else {
                ++bitIterator;
            }
        }

        if (musicBox->blocksAvailable < musicBox->maxBlockCount){
            musicBox->writeBitsToBuffer(&bitsPlayed);
        }

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

void CompositionMenu::switchTimelineFocus() {
    getFocusedControl()->switchHighlight();
    if (isTimelineFocused){
        isTimelineFocused = false;
        timeline->timelineFocused = false;
    } else {
        isTimelineFocused = true;
        timeline->timelineFocused = true;
    }
}
