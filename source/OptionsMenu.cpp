#include <fstream>
#include "../include/OptionsMenu.h"
#include "../include/CompositionMenu.h"
#include "../include/InstrumentMenu.h"
#include <filesystem>
namespace fs = std::filesystem;

#define MAIN 0
#define EXPORT 1
#define SAVE 2
#define LOAD 3

OptionsMenu::OptionsMenu(MainWindow* mainWindow) {
    textColor = {255, 255, 255};
    window = mainWindow;
    musicBox = mainWindow->musicBox;
    renderer = mainWindow->renderer;
    serializer = new Serializer();
    deserializer = new Deserializer();
    itemList = nullptr;
    inputBox = {0,0, static_cast<int>(window->w*0.5)-4, static_cast<int>(window->h*0.1)-4};
    inputBoxBorder = {0,0, static_cast<int>(window->w*0.5), static_cast<int>(window->h*0.1)};
    dialogBox = {0,0,
                 static_cast<int>(window->w*0.5)-4, static_cast<int>(window->h*0.5)-4};
    dialogBox.x = window->w/2 - dialogBox.w/2;
    dialogBox.y = window->h/2 - dialogBox.h/2;
    dialogBoxBorder = {dialogBox.x - 2,dialogBox.y - 2,
                       static_cast<int>(window->w*0.5), static_cast<int>(window->h*0.5)};
    inputValue = "";
    dialogTextValue = "sample text";
}

void OptionsMenu::init(){
    loadTextures();
    loadControls();
}

void OptionsMenu::loadTextures(){
    setTextTexture(&exportSongLabel, "EKSPORTUJ KOMPOZYCJĘ", window->mainFont);
    setTextTexture(&saveProjectLabel, "ZAPISZ PROJEKT", window->mainFont);
    setTextTexture(&loadProjectLabel, "WCZYTAJ PROJEKT", window->mainFont);
    setTextTexture(&quitLabel, "WYJDŹ Z PROGRAMU", window->mainFont);
    setTextTexture(&okLabel, "OK", window->mainFont);
    setTextTexture(&opCancelLabel, "[Esc] Anuluj", window->mainFont);
    setTextTexture(&opConfirmLabel, "[Enter] Zatwierdź", window->mainFont);
}

void OptionsMenu::updateControls(){
    if (inputValue.length() > 0) setTextTexture(&inputValueLabel, inputValue, window->largeFont);
    setTextTexture(&dialogTextLabel, dialogTextValue, window->mainFont);
}

void OptionsMenu::loadControls(){
    exportSongBtn.loadTextures(&exportSongLabel, window);
    saveProjectBtn.loadTextures(&saveProjectLabel, window);
    loadProjectBtn.loadTextures(&loadProjectLabel, window);
    quitBtn.loadTextures(&quitLabel, window);
    okBtn.loadTextures(&okLabel, window);
    okBtn.isHighlighted = true;

    exportSongBtn.onPress = &OptionsMenu::openExportSongScreen;
    saveProjectBtn.onPress = &OptionsMenu::openSaveProjectScreen;
    loadProjectBtn.onPress = &OptionsMenu::openLoadProjectScreen;
    quitBtn.onPress = &OptionsMenu::quit;

    exportSongBtn.isHighlighted = true;
}

void OptionsMenu::quit(){
    exit(0);
}

void OptionsMenu::saveProject(){
    inputValue.pop_back();
    std::ofstream file;
    file.open("projects/" + inputValue + ".json");
    file << nlohmann::to_string(serializer->extractProject(window->musicBox, window->compositionMenu->timeline));
    file.close();
}

void OptionsMenu::loadProject(){
    std::ifstream file;
    auto filePath = fs::current_path().string() + "/projects/" +itemList->getSelectedItem()+ ".json";
    file.open( filePath);
    JSON projectJson = JSON::parse(file);
    loadInstruments(projectJson);
    loadComposition(projectJson);
}

void OptionsMenu::loadInstruments(JSON projectJson){
    for (auto* instrument : musicBox->instruments){
        delete instrument;
    }
    musicBox->instruments.clear();
    window->compositionMenu->instrumentList->items.clear();
    window->instrumentMenu->instrumentList->items.clear();
    window->instrumentMenu->instrumentList->addItem("+ New instrument");

    for (const auto& instrumentJson : projectJson["instruments"]){
        auto* instrument = new Instrument(musicBox->blockSize);
        deserializer->deserializeInstrument(instrumentJson, instrument);
        window->instrumentMenu->addInstrument(instrument, instrument->index);
    }
}

void OptionsMenu::loadComposition(JSON projectJson){
    auto& timeline = window->compositionMenu->timeline;
    JSON compositionJson = projectJson["composition"];
    timeline->tempo = compositionJson["tempo"];

    while (timeline->segColumns() > 1)
        window->compositionMenu->removeColumn();
    for (auto& segment : timeline->allSegs)
        segment->cols.pop_back();
    while (timeline->segColumns() != compositionJson["columns"])
        window->compositionMenu->addColumn();

    for (int i = 0; i < compositionJson["all_segments"].size(); i++){
        auto segmentJson = compositionJson["all_segments"].at(i);
        auto& segment = timeline->allSegs.at(i);

        if (segmentJson == nullptr) continue;

        for (int j = 0; j < segmentJson["cols"].size(); j++){
            auto colJson = segmentJson["cols"].at(j);
            auto& col = segment->cols.at(j);

            if (colJson == nullptr) continue;

            for (int k = 0; k < colJson.size(); k++){
                auto bitJson = colJson.at(k);
                auto& bit = col->bits[k];
                if (bitJson == nullptr) continue;
                bit = new Bit(bitJson["note_value"],
                              musicBox->instruments.at(bitJson["instrument"]),
                              bitJson["hold_duration"],
                              bitJson["hold_section"]);
            }
        }
    }
    timeline->songSegs.clear();

    for (const auto& segIndex : compositionJson["song_segments"]){
        timeline->songSegs.push_back(timeline->allSegs.at(segIndex));
    }
}

void OptionsMenu::setTextTexture(Texture* texture, const string& text, TTF_Font* font) const {
    texture->loadFromText(renderer, text, textColor, font);
}

void OptionsMenu::render(){
    updateControls();
    switch (screenRendered) {
        case MAIN:
            exportSongBtn.render(xByPercent(&exportSongLabel, 0.5),
                                 yByPercent(&exportSongLabel, 0.3));
            saveProjectBtn.render(xByPercent(&saveProjectLabel, 0.5),
                                  yByPercent(&saveProjectLabel, 0.45));
            loadProjectBtn.render(xByPercent(&loadProjectLabel, 0.5),
                                  yByPercent(&loadProjectLabel, 0.60));
            quitBtn.render(xByPercent(&quitLabel, 0.5),
                    yByPercent(&quitLabel   , 0.75));
            break;
        case EXPORT:
            itemList->render(window->w*0.8 - itemList->width/2,
                             window->h/2 - itemList->height/2);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderDrawRect(renderer, &inputBoxBorder);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderDrawRect(renderer, &inputBox);

            if (inputValue.length() > 0) inputValueLabel.render(inputBox.x,inputBox.y + inputBox.h/2 - inputValueLabel.h/2);


            opDescriptionLabel.render(xByPercent(&opDescriptionLabel, 0.5),
                                      yByPercent(&opDescriptionLabel, 0.275));
            opCancelLabel.render(xByPercent(&opCancelLabel, 0.33),
                                 yByPercent(&opCancelLabel, 0.75));
            opConfirmLabel.render(xByPercent(&opConfirmLabel, 0.66),
                                  yByPercent(&opConfirmLabel, 0.75));
            break;
        case SAVE:
            opDescriptionLabel.render(xByPercent(&opDescriptionLabel, 0.5),
                                      yByPercent(&opDescriptionLabel, 0.35));
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderDrawRect(renderer, &inputBoxBorder);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderDrawRect(renderer, &inputBox);

            if (inputValue.length() > 0) inputValueLabel.render(inputBox.x,inputBox.y + inputBox.h/2 - inputValueLabel.h/2);

            opCancelLabel.render(xByPercent(&opCancelLabel, 0.33),
                                 yByPercent(&opCancelLabel, 0.7));
            opConfirmLabel.render(xByPercent(&opConfirmLabel, 0.66),
                                 yByPercent(&opConfirmLabel, 0.7));
            break;
        case LOAD:
            itemList->render(window->w/2 - itemList->width/2,
                             window->h/2 - itemList->height/2);
            opDescriptionLabel.render(xByPercent(&opDescriptionLabel, 0.5),
                                      yByPercent(&opDescriptionLabel, 0.25));
            opCancelLabel.render(xByPercent(&opCancelLabel, 0.33),
                                 yByPercent(&opCancelLabel, 0.75));
            opConfirmLabel.render(xByPercent(&opConfirmLabel, 0.66),
                                  yByPercent(&opConfirmLabel, 0.75));
            break;
    }
    if (dialogOpen){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &dialogBoxBorder);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(renderer, &dialogBox);
        okBtn.render(xByPercent(&okLabel, 0.5),
                              yByPercent(&okLabel, 0.65));
        dialogTextLabel.render(xByPercent(&dialogTextLabel, 0.5),
                               yByPercent(&dialogTextLabel, 0.4));
    }

}

int OptionsMenu::xByPercent(Texture* texture, double percent, Alignment align) const {
    return window->xByPercent(texture, percent, align);
}
int OptionsMenu::xByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    return window->xByPercent(rect, percent, align);
}
int OptionsMenu::yByPercent(Texture* texture, double percent, Alignment align) const {
    return window->yByPercent(texture, percent, align);
}
int OptionsMenu::yByPercent(SDL_Rect* rect, double percent, Alignment align) const {
    return window->yByPercent(rect, percent, align);
}

Control* OptionsMenu::getFocusedControl() {
    return controlArray[focusedControlRow][focusedControlCol];
}

void OptionsMenu::handleKeyPress(SDL_Keycode key){
    switch (key) {
        case SDLK_UP:
            if (listFocused){
                itemList->moveUp();
            } else
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::UP);
            }
            break;
        case SDLK_DOWN:
            if (listFocused){
                itemList->moveDown();
            } else
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::DOWN);
            }
            break;
        case SDLK_LEFT:
            if (screenRendered == EXPORT)
            {
                if (!window->typing){
                    openTextInput();
                    window->typing = true;
                    focusList(false);
                }

            }

            break;
        case SDLK_RIGHT:
            if (screenRendered == EXPORT)
            {
                if (window->typing){
                    closeTextInput();
                    listFocused = true;
                    focusList(true);
                }
            }

            break;
        case SDLK_RETURN:

            switch (screenRendered) {
                case EXPORT:
                    if (!dialogOpen){
                        if (window->typing){
                            closeTextInput();
                        }

                        exportCompositionToWav(inputValue);

                        auto extension = itemList->getSelectedItem();
                        if (extension != ".wav"){
                            auto convertCommand = "ffmpeg -i compositions/" + inputValue + ".wav compositions/" + inputValue + extension
                                                  + " && rm compositions/" + inputValue + ".wav";
                            system(convertCommand.c_str());
                        }
                        dialogTextValue = "Eksport zakończony pomyślnie!";
                        dialogOpen = true;
                    } else {
                        musicBox->closeFile();
                        cleanupTextInput();
                        dialogOpen = false;
                        screenRendered = MAIN;
                    }
                    break;
                case LOAD:
                    if (!dialogOpen){
                        if (!itemList->items.empty()){
                            loadProject();
                            dialogTextValue = "Projekt wczytany pomyślnie!";
                            dialogOpen = true;
                        }
                    } else {

                        dialogOpen = false;
                        focusList(false);
                        screenRendered = MAIN;
                        window->openTab = COMP_MENU;
                    }
                    break;

                case SAVE:
                    window->typing = false;
                    if (!dialogOpen){
                        saveProject();
                        dialogTextValue = "Projekt zapisany pomyślnie!";
                        dialogOpen = true;
                    } else {
                        cleanupTextInput();
                        dialogOpen = false;
                        screenRendered = MAIN;
                    }
                    break;
                case MAIN:
                    selectFocusedControl();
                    break;
                default:
                    break;
            }
            break;
        case SDLK_ESCAPE:
            if (listFocused){
                focusList(false);
            }
            cleanupTextInput();
            screenRendered = MAIN;
        case SDLK_BACKSPACE:
            deleteLetter();
        default:
            break;
    }
}

void OptionsMenu::exportCompositionToWav(const string& filename){
    musicBox->openFile("compositions/"+ filename+".wav");
    musicBox->stopPlaying();
    musicBox->playbackKeys = false;
    musicBox->startPlaying();
    window->compositionMenu->playbackTimeline();
    musicBox->stopPlaying();
    musicBox->playbackKeys = true;
    musicBox->startPlaying();
}

void OptionsMenu::changeControlFocus(Direction direction) {
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
        case Direction::RIGHT:
            break;
    }
    getFocusedControl()->switchHighlight();
}

void OptionsMenu::openLoadProjectScreen()
{
    setTextTexture(&opDescriptionLabel, "Otwórz projekt:", window->largeFont);
    delete itemList;
    itemList = new ItemList(renderer, window->mainFont, 400,200,5);
    std::string projectsPath = fs::current_path().string() + "/projects";
    auto filenames = getDirFilenamesWithoutExtensions(projectsPath);
    for (const auto & filename : filenames)
        itemList->addItem(filename);
    if (!itemList->items.empty())
        itemList->setSelectedIndex(0);
    listFocused = true;
    itemList->isFocused = true;
    screenRendered = LOAD;
}

vector<string> OptionsMenu::getDirFilenamesWithoutExtensions(const string& dirPath){
    vector<string> filenames;
    for (const auto & entry : fs::directory_iterator(dirPath)){
        string filePath = entry.path();

        auto slashpos = filePath.find_last_of('/');
        auto filename = filePath.substr(slashpos+1);
        auto dotpos = filename.find_last_of('.');
        filename = filename.substr(0, dotpos);

        filenames.push_back(filename);
//        itemList->addItem(filename);
    }
    return filenames;
}

void OptionsMenu::openSaveProjectScreen()
{
    screenRendered = SAVE;
    updateInputBoxPosition();
    openTextInput();
    setTextTexture(&opDescriptionLabel, "Zapisz projekt jako:", window->largeFont);
}

void OptionsMenu::openExportSongScreen()
{
    screenRendered = EXPORT;
    updateInputBoxPosition();
    setTextTexture(&opDescriptionLabel, "Eksportuj kompozycję jako:", window->largeFont);
    openTextInput();
    delete itemList;
    itemList = new ItemList(renderer, window->mainFont, 100,200,5);
    for (const auto& format : getAudioFormats())
        itemList->addItem("." + format);

}

void OptionsMenu::updateInputBoxPosition(){
    switch (screenRendered){
        case EXPORT:
            inputBox.x = xByPercent(&inputBox, 0.45);
            inputBox.y = yByPercent(&inputBox, 0.5);
            break;
        case SAVE:
            inputBox.x = xByPercent(&inputBox, 0.5);
            inputBox.y = yByPercent(&inputBox, 0.5);
            break;
        default:
            break;
    }
    inputBoxBorder.x = inputBox.x-2;
    inputBoxBorder.y = inputBox.y-2;
}

vector<string> OptionsMenu::getAudioFormats()
{
    return vector<string> {"wav", "mp3", "ogg", "flac", "aac"};
}

void OptionsMenu::selectFocusedControl() {
    getFocusedControl()->activate();
}

void OptionsMenu::deleteLetter(){
    if (window->typing && inputValue.length() > 1){
        inputValue.pop_back();
        inputValue.pop_back();
        inputValue += '_';
    }
}

void OptionsMenu::openTextInput()
{
    if (!window->typing) {
        inputValue += '_';
        window->typing = true;
    }
}

void OptionsMenu::closeTextInput()
{
    if (window->typing){
        inputValue.pop_back();
        window->typing = false;
    }
}

void OptionsMenu::cleanupTextInput()
{
    inputValue = "";
    window->typing = false;
}

void OptionsMenu::focusList(bool focus)
{
    listFocused = focus;
    itemList->isFocused = focus;
}



void OptionsMenu::inputLetter(const Uint8 *keyState, const bool *lastKeyState) {
    if (inputValue.length() > 20) return;
    char c = '\0';
    if (keyState[SDL_SCANCODE_A] && !lastKeyState[SDL_SCANCODE_A]) c = shiftPressed ? 'A' : 'a';
    if (keyState[SDL_SCANCODE_B] && !lastKeyState[SDL_SCANCODE_B]) c = shiftPressed ? 'B' : 'b';
    if (keyState[SDL_SCANCODE_C] && !lastKeyState[SDL_SCANCODE_C]) c = shiftPressed ? 'C' : 'c';
    if (keyState[SDL_SCANCODE_D] && !lastKeyState[SDL_SCANCODE_D]) c = shiftPressed ? 'D' : 'd';
    if (keyState[SDL_SCANCODE_E] && !lastKeyState[SDL_SCANCODE_E]) c = shiftPressed ? 'E' : 'e';
    if (keyState[SDL_SCANCODE_F] && !lastKeyState[SDL_SCANCODE_F]) c = shiftPressed ? 'F' : 'f';
    if (keyState[SDL_SCANCODE_G] && !lastKeyState[SDL_SCANCODE_G]) c = shiftPressed ? 'G' : 'g';
    if (keyState[SDL_SCANCODE_H] && !lastKeyState[SDL_SCANCODE_H]) c = shiftPressed ? 'H' : 'h';
    if (keyState[SDL_SCANCODE_I] && !lastKeyState[SDL_SCANCODE_I]) c = shiftPressed ? 'I' : 'i';
    if (keyState[SDL_SCANCODE_J] && !lastKeyState[SDL_SCANCODE_J]) c = shiftPressed ? 'J' : 'j';
    if (keyState[SDL_SCANCODE_K] && !lastKeyState[SDL_SCANCODE_K]) c = shiftPressed ? 'K' : 'k';
    if (keyState[SDL_SCANCODE_L] && !lastKeyState[SDL_SCANCODE_L]) c = shiftPressed ? 'L' : 'l';
    if (keyState[SDL_SCANCODE_M] && !lastKeyState[SDL_SCANCODE_M]) c = shiftPressed ? 'M' : 'm';
    if (keyState[SDL_SCANCODE_N] && !lastKeyState[SDL_SCANCODE_N]) c = shiftPressed ? 'N' : 'n';
    if (keyState[SDL_SCANCODE_O] && !lastKeyState[SDL_SCANCODE_O]) c = shiftPressed ? 'O' : 'o';
    if (keyState[SDL_SCANCODE_P] && !lastKeyState[SDL_SCANCODE_P]) c = shiftPressed ? 'P' : 'p';
    if (keyState[SDL_SCANCODE_Q] && !lastKeyState[SDL_SCANCODE_Q]) c = shiftPressed ? 'Q' : 'q';
    if (keyState[SDL_SCANCODE_R] && !lastKeyState[SDL_SCANCODE_R]) c = shiftPressed ? 'R' : 'r';
    if (keyState[SDL_SCANCODE_S] && !lastKeyState[SDL_SCANCODE_S]) c = shiftPressed ? 'S' : 's';
    if (keyState[SDL_SCANCODE_T] && !lastKeyState[SDL_SCANCODE_T]) c = shiftPressed ? 'T' : 't';
    if (keyState[SDL_SCANCODE_U] && !lastKeyState[SDL_SCANCODE_U]) c = shiftPressed ? 'U' : 'u';
    if (keyState[SDL_SCANCODE_V] && !lastKeyState[SDL_SCANCODE_V]) c = shiftPressed ? 'V' : 'v';
    if (keyState[SDL_SCANCODE_W] && !lastKeyState[SDL_SCANCODE_W]) c = shiftPressed ? 'W' : 'w';
    if (keyState[SDL_SCANCODE_X] && !lastKeyState[SDL_SCANCODE_X]) c = shiftPressed ? 'X' : 'x';
    if (keyState[SDL_SCANCODE_Y] && !lastKeyState[SDL_SCANCODE_Y]) c = shiftPressed ? 'Y' : 'y';
    if (keyState[SDL_SCANCODE_Z] && !lastKeyState[SDL_SCANCODE_Z]) c = shiftPressed ? 'Z' : 'z';
    if (keyState[SDL_SCANCODE_SPACE] && !lastKeyState[SDL_SCANCODE_SPACE]) c = '_';
    if (keyState[SDL_SCANCODE_MINUS] && !lastKeyState[SDL_SCANCODE_MINUS]) c = shiftPressed ? '_' : '-';
    if (c != '\0'){
        inputValue.pop_back();
        inputValue += c;
        inputValue += '_';
    }
}

void OptionsMenu::registerShiftPress(bool shiftPressed) {
    this->shiftPressed = shiftPressed;
}

