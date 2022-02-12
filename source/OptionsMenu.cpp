#include <fstream>
#include "../include/OptionsMenu.h"
#include "../include/CompositionMenu.h"
#include "../include/InstrumentMenu.h"


OptionsMenu::OptionsMenu(MainWindow* mainWindow) {
    textColor = {255, 255, 255};
    window = mainWindow;
    musicBox = mainWindow->musicBox;
    renderer = mainWindow->renderer;
    serializer = new Serializer();
    deserializer = new Deserializer();
}

void OptionsMenu::init(){
    loadTextures();
    loadControls();
}

void OptionsMenu::loadTextures(){
    setTextTexture(&saveSongLabel, "EKSPORTUJ KOMPOZYCJE", window->mainFont);
    setTextTexture(&saveProjectLabel, "ZAPISZ PROJEKT", window->mainFont);
    setTextTexture(&loadProjectLabel, "WCZYTAJ PROJEKT", window->mainFont);
}

void OptionsMenu::loadControls(){
    saveSongBtn.loadTextures(&saveSongLabel, window);
    saveProjectBtn.loadTextures(&saveProjectLabel, window);
    loadProjectBtn.loadTextures(&loadProjectLabel, window);
    saveSongBtn.onPress = &OptionsMenu::saveProject;
    saveProjectBtn.onPress = &OptionsMenu::saveProject;
    loadProjectBtn.onPress = &OptionsMenu::loadProject;

    saveSongBtn.isHighlighted = true;
}

void OptionsMenu::saveProject(){
    std::ofstream file;
    file.open("project.txt");
    file << nlohmann::to_string(serializer->extractProject(window->musicBox, window->compositionMenu->timeline));
    file.close();
}

void OptionsMenu::loadProject(){
    std::ifstream file;
    file.open("project.txt");
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
    saveSongBtn.render(xByPercent(&saveSongLabel, 0.5),
                          yByPercent(&saveSongLabel, 0.3));
    saveProjectBtn.render(xByPercent(&saveProjectLabel, 0.5),
                          yByPercent(&saveProjectLabel, 0.5));
    loadProjectBtn.render(xByPercent(&loadProjectLabel, 0.5),
                       yByPercent(&loadProjectLabel, 0.7));

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
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::UP);
            }
            break;
        case SDLK_DOWN:
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::DOWN);
            }
            break;
        case SDLK_LEFT:
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::LEFT);
            }
            break;
        case SDLK_RIGHT:
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::RIGHT);
            }
            break;
        case SDLK_RETURN:
            selectFocusedControl();
            break;
        default:
            break;
    }
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

void OptionsMenu::selectFocusedControl() {
    getFocusedControl()->activate();
}