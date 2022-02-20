#include "../include/CompositionMenu.h"

using std::string;
using std::to_string;

CompositionMenu::CompositionMenu(MainWindow *mainWindow) {
    textColor = {255, 255, 255};
    musicBox = mainWindow->musicBox;
    window = mainWindow;
    renderer = mainWindow->renderer;
    timeline = new Timeline(this->renderer, window->smallFont, 21, 45, window->mainArea.h*1/2, 8);
    instrumentList = new ItemList(this->renderer, window->tinyFont, window->mainArea.w/5, window->mainArea.w/6, 6, true);
    segmentManager = new SegmentManager(timeline, window->smallFont, window->mainArea.w/10, window->mainArea.w/6, 5);
}

void CompositionMenu::init(){
    loadTextures();
    loadControls();
    updateHelpBar();
}


void CompositionMenu::render(){
    updateTextures();
    timeline->render(xByPercent(&timeline->outline, 0.5),
            window->mainArea.h * 0.45);
    segmentManager->render(xByPercent(&segmentManager->outline, 0.04, TO_RIGHT) - 3,
                           window->mainArea.h * 0.15);
    segmentManagerLabel.render(xByPercent(&segmentManager->outline, 0.04, TO_RIGHT) - 3,
                               window->mainArea.h * 0.15 - segmentManagerLabel.h);
    instrumentList->render(xByPercent(&instrumentList->outline, 0.864),
                           window->mainArea.h * 0.15);
    instrumentListLabel.render(xByPercent(&instrumentList->outline, 0.864),
                               window->mainArea.h * 0.15 - instrumentListLabel.h);


    segmentsLabel.render(xByPercent(&segmentsLabel, 0.5, TO_LEFT),
                         yByPercent(&segmentsLabel, 0.18));
    segmentsSelector.render(xByPercent(&segmentsValue, 0.55, CENTER),
                            yByPercent(&segmentsValue, 0.18));
    colsLabel.render(xByPercent(&colsLabel, 0.5, TO_LEFT),
                     yByPercent(&colsLabel, 0.26));
    colsSelector.render(xByPercent(&colsValue, 0.55, CENTER),
                        yByPercent(&colsLabel, 0.26));
    tempoLabel.render(xByPercent(&tempoLabel, 0.5, TO_LEFT),
                         yByPercent(&tempoLabel, 0.34));
    tempoSelector.render(xByPercent(&tempoValue, 0.55, CENTER),
                      yByPercent(&tempoValue, 0.34));


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
    setTextTexture(&segmentsLabel, "Segmenty:", window->mainFont);
    setTextTexture(&colsLabel, "Kolumny:", window->mainFont);
    setTextTexture(&tempoLabel, "Tempo:", window->mainFont);
    setTextTexture(&instrumentListLabel, "Bieżący instrument", window->smallFont);
    setTextTexture(&segmentManagerLabel, "Segmenty", window->smallFont);
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

void CompositionMenu::registerShiftPress(bool shiftPressed){
    this->shiftPressed = shiftPressed;
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
                    startPlayback();
                } else {
                    stopPlayback();
                }
            }
            break;
        case SDLK_RETURN:
            if (isTimelineFocused){
                timeline->editingMode = !timeline->editingMode;
            } else if (!isSegmentListFocused && !isInstrumentListFocused){
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
        case SDLK_SEMICOLON:
        case SDLK_SLASH:
            if (timeline->editingMode){
                auto* bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex)->bits[timeline->focusedBitIndex];
                if (*bitPtr != nullptr){

                    if ((*bitPtr)->holdDuration > 0){

                        int holdDuration = (*bitPtr)->holdDuration;
                        int holdSection = (*bitPtr)->holdSection;

                        for (int i = 0; i <= holdDuration - holdSection; i++){
                            auto* _bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex+i)->bits[timeline->focusedBitIndex];
                            delete *_bitPtr;
                            *_bitPtr = nullptr;
                        }
                        for (int i = holdSection; i > 0; i--){
                            auto* _bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex-i)->bits[timeline->focusedBitIndex];
                            (*_bitPtr)->holdDuration = holdSection-1;
                        }


                        if (shiftPressed && timeline->focusedColIndex > 0
                            && timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex-1)->bits[timeline->focusedBitIndex]->note.frequency == midiToFreq(musicBox->keyToNoteValue(key)))
                        {
                            for (int i = holdSection; i > 0; i--){
                                auto* _bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex-i)->bits[timeline->focusedBitIndex];
                                (*_bitPtr)->holdDuration++;
                            }

                            delete *bitPtr;
                            *bitPtr = new Bit(musicBox->keyToNoteValue(key), musicBox->instruments.at(instrumentList->selectedIndex), holdSection, holdSection);
                        }
                        else
                        {
                            delete *bitPtr;
                            *bitPtr = new Bit(musicBox->keyToNoteValue(key), musicBox->instruments.at(instrumentList->selectedIndex));
                        }

                    } else {
                        if (!shiftPressed){
                            (*bitPtr)->note.frequency = midiToFreq(musicBox->keyToNoteValue(key));
                        } else {
                            int holdDuration = 0;

                            if (timeline->focusedColIndex > 0){
                                auto* comparedBit = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex - 1)->bits[timeline->focusedBitIndex];
                                if ((*comparedBit) != nullptr && (*comparedBit)->note.frequency == midiToFreq(musicBox->keyToNoteValue(key))){

                                    holdDuration = (*comparedBit)->holdDuration;
                                    int holdSection = (*comparedBit)->holdSection;

                                    for (int i = holdSection+1; i > 0; i--){
                                        auto* _bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex-i)->bits[timeline->focusedBitIndex];
                                        (*_bitPtr)->holdDuration++;
                                    }
                                    holdDuration++;

                                }
                            }

                            delete *bitPtr;
                            *bitPtr = new Bit(musicBox->keyToNoteValue(key), musicBox->instruments.at(instrumentList->selectedIndex), holdDuration, holdDuration);
                        }

                    }

                } else { // bitPtr == nullptr
                    if (!shiftPressed){
                        *bitPtr = new Bit(musicBox->keyToNoteValue(key), musicBox->instruments.at(instrumentList->selectedIndex));
                    } else {
                        int holdDuration = 0;

                        if (timeline->focusedColIndex > 0){
                            auto* comparedBit = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex - 1)->bits[timeline->focusedBitIndex];
                            if ((*comparedBit) != nullptr && (*comparedBit)->note.frequency == midiToFreq(musicBox->keyToNoteValue(key))){

                                holdDuration = (*comparedBit)->holdDuration;
                                int holdSection = (*comparedBit)->holdSection;

                                for (int i = holdSection+1; i > 0; i--){
                                    auto* _bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex-i)->bits[timeline->focusedBitIndex];
                                    (*_bitPtr)->holdDuration++;
                                }
                                holdDuration++;
                            }
                        }

                        *bitPtr = new Bit(musicBox->keyToNoteValue(key), musicBox->instruments.at(instrumentList->selectedIndex), holdDuration, holdDuration);
                    }
                }
                timeline->focusedColIndex++;
                if (timeline->focusedColIndex == timeline->segColumns()){
                    if (timeline->focusedSegmentIndex < timeline->songSegs.size()-1){
                        timeline->focusedSegmentIndex++;
                        timeline->focusedColIndex = 0;
                    } else {
                        timeline->focusedColIndex--;
                    }
                }

            }
            break;



        case SDLK_DELETE:
            if (timeline->editingMode){
                auto* bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex)->bits[timeline->focusedBitIndex];
                if (*bitPtr != nullptr){
                    if ((*bitPtr)->holdDuration > 0){

                        int holdDuration = (*bitPtr)->holdDuration;
                        int holdSection = (*bitPtr)->holdSection;

                        for (int i = 0; i <= holdDuration - holdSection; i++){
                            auto* _bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex+i)->bits[timeline->focusedBitIndex];
                            delete *_bitPtr;
                            *_bitPtr = nullptr;
                        }

                        for (int i = holdSection; i > 0; i--){
                            auto* _bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex-i)->bits[timeline->focusedBitIndex];
                            (*_bitPtr)->holdDuration = holdSection-1;
                        }
                    }
                    delete *bitPtr;
                    *bitPtr = nullptr;
                }
            }
            break;
        case SDLK_BACKSPACE:
            if (timeline->editingMode){

                timeline->focusedColIndex--;
                if (timeline->focusedColIndex < 0){
                    if (timeline->focusedSegmentIndex > 0){
                        timeline->focusedSegmentIndex--;
                        timeline->focusedColIndex = timeline->segColumns()-1;
                    } else {
                        timeline->focusedColIndex++;
                    }
                }

                auto* bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex)->bits[timeline->focusedBitIndex];
                if (*bitPtr != nullptr){
                    if ((*bitPtr)->holdDuration > 0){

                        int holdDuration = (*bitPtr)->holdDuration;
                        int holdSection = (*bitPtr)->holdSection;

                        for (int i = 0; i <= holdDuration - holdSection; i++){
                            auto* _bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex+i)->bits[timeline->focusedBitIndex];
                            delete *_bitPtr;
                            *_bitPtr = nullptr;
                        }

                        for (int i = holdSection; i > 0; i--){
                            auto* _bitPtr = &timeline->songSegs.at(timeline->focusedSegmentIndex)->cols.at(timeline->focusedColIndex-i)->bits[timeline->focusedBitIndex];
                            (*_bitPtr)->holdDuration = holdSection-1;
                        }
                    }
                    delete *bitPtr;
                    *bitPtr = nullptr;
                }
            }
            break;

        case SDLK_EQUALS:
            if (isSegmentListFocused || isTimelineFocused){
                segmentManager->segmentUp();
            }
            break;
        case SDLK_MINUS:
            if (isSegmentListFocused || isTimelineFocused){
                segmentManager->segmentDown();
            }
            break;

        default:
            break;
    }
    updateHelpBar();
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
    }
    timeline->songSegs.front()->cols.at(1)->bits[1] = new Bit(60, instrument, 4, 0);
    timeline->songSegs.front()->cols.at(2)->bits[1] = new Bit(60, instrument, 4, 1);
    timeline->songSegs.front()->cols.at(3)->bits[1] = new Bit(60, instrument, 4, 2);
    timeline->songSegs.front()->cols.at(4)->bits[1] = new Bit(60, instrument, 4, 3);
    timeline->songSegs.front()->cols.at(5)->bits[1] = new Bit(60, instrument, 4, 4);

}

void CompositionMenu::startPlayback(){
    musicBox->stopPlaying();
    musicBox->playbackKeys = false;
    musicBox->startPlaying();
    musicBox->writeThread = std::thread(&CompositionMenu::playbackTimeline, this);
}

void CompositionMenu::stopPlayback(){
    musicBox->stopPlaying();
    while (!musicBox->blocksBuffer.empty()){
        musicBox->blocksBuffer.pop();
    }
    musicBox->blocksReadyToOutput = 0;
    musicBox->playbackKeys = true;
    musicBox->startPlaying();
    playbackOn = false;
}

void CompositionMenu::playbackTimeline(){

    playbackOn = true;
    double globalTime = musicBox->globalTime;
    double beginTime = globalTime;
    double timeBetweenCols = 60.0 / timeline->tempo;
    double timeElapsed = 0.0;
    double lastColTriggerTime = -timeBetweenCols;

    vector<Bit*> heldBits;

    double songLength = timeline->songSegs.front()->cols.size() * timeBetweenCols * timeline->songSegs.size();

    while (timeElapsed < songLength){

        std::unique_lock<std::mutex> mu(musicBox->mu_blocksReadyToRead);
        if (musicBox->blocksReadyToOutput == musicBox->maxBlockCount){
            musicBox->cv_blocksReadyToWrite.wait(mu);
        }
        if (!musicBox->isRunning) break;

        if (timeElapsed - lastColTriggerTime > timeBetweenCols){


            int colsElapsed = (int)(timeElapsed / timeBetweenCols) % timeline->segColumns();
            int segsElapsed = timeElapsed / timeBetweenCols / timeline->segColumns();
            Column* currentCol = timeline->songSegs.at(segsElapsed)->cols.at(colsElapsed);


            for (auto bit : currentCol->bits){
                if (bit != nullptr && bit->holdSection == 0){
                    bit->note.pressedOnTime = globalTime;
                    bit->note.isAudible = true;
                    bitsPlayed.push_back(bit);
                }
            }

            if (colsElapsed > 0){
                Column* previousCol = timeline->songSegs.at(segsElapsed)->cols.at(colsElapsed - 1);
                for (int i = 0; i < 5; i++){
                    auto bit = previousCol->bits[i];
                    if (bit != nullptr && bit->holdSection == bit->holdDuration){
                        auto beginBit = timeline->songSegs.at(segsElapsed)->cols.at(colsElapsed - bit->holdDuration-1)->bits[i];
                        beginBit->note.releasedOnTime = globalTime;
                    }
                }
            }
            else if (segsElapsed > 0){
                Column* previousCol = timeline->songSegs.at(segsElapsed-1)->cols.at(timeline->segColumns()-1);
                for (int i = 0; i < 5; i++){
                    auto bit = previousCol->bits[i];
                    if (bit != nullptr && bit->holdSection == bit->holdDuration){
                        auto beginBit = timeline->songSegs.at(segsElapsed-1)->cols.at(timeline->segColumns() - bit->holdDuration-1)->bits[i];
                        beginBit->note.releasedOnTime = globalTime;
                    }
                }
            }

            lastColTriggerTime = timeElapsed;
        }


        auto bitIterator = bitsPlayed.begin();
        while (bitIterator != bitsPlayed.end()){
            if (!(*bitIterator)->note.isAudible){
                bitIterator = bitsPlayed.erase(bitIterator);
            } else {
                ++bitIterator;
            }
        }
        if (musicBox->blocksReadyToOutput < musicBox->maxBlockCount){
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

    playbackOn = false;
}

void CompositionMenu::switchTimelineFocus() {
    if (!isSegmentListFocused && !isInstrumentListFocused){
        getFocusedControl()->switchHighlight();
    }


    if (isTimelineFocused){
        if (timeline->editingMode){
            timeline->editingMode = false;
        }
        isTimelineFocused = false;
        timeline->timelineFocused = false;
        if (isInstrumentListFocused){
            instrumentList->isFocused = true;
        }
        if (isSegmentListFocused){
            segmentManager->isFocused = true;
        }
    } else {
        isTimelineFocused = true;
        timeline->timelineFocused = true;
        if (isInstrumentListFocused){
            instrumentList->isFocused = false;
        }
        if (isSegmentListFocused){
            segmentManager->isFocused = false;
        }
    }
}

void CompositionMenu::updateHelpBar()
{
    if (isTimelineFocused){

        if (timeline->editingMode){
            window->setHelpBarText("[~] KLAWISZE | [KLAWISZ] UMIEŚĆ ([+ SHIFT] PRZEDŁUŻ) | [← ↓ ↑ →] NAWIGUJ | [BACKSPACE] USUŃ | [ENTER] ZATWIERDŹ");
        } else {
            window->setHelpBarText("[~] KLAWISZE | [← ↓ ↑ →] NAWIGUJ | [SPACJA] ODTWÓRZ | [TAB] USTAWIENIA | [ENTER] ZATWIERDŹ | [-/+] ZMIEŃ SEGMENT");
        }

    } else {
        if (isSegmentListFocused){
            window->setHelpBarText("[~] KLAWISZE | [← ↓ ↑ →] NAWIGUJ | [-] POPRZEDNI | [+] NASTĘPNY");
        } else if (isInstrumentListFocused) {
            window->setHelpBarText("[~] KLAWISZE | [← ↓ ↑ →] NAWIGUJ");
        } else if (getFocusedControl()->isEditing){
            window->setHelpBarText("[~] KLAWISZE | [← ↓] ZMNIEJSZ | [↑ →] ZWIĘKSZ | [ENTER] ZATWIERDŹ");
        } else {
            window->setHelpBarText("[~] KLAWISZE | [← ↓ ↑ →] NAWIGUJ | [TAB] OŚ CZASU | [ENTER] EDYTUJ | [SPACJA] ODTWÓRZ");
        }
    }
}