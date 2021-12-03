#include "../include/WaveformMenu.h"
#include "../include/Direction.h"
#include <string>
#include <iomanip>
#include <sstream>

WaveformMenu::WaveformMenu(MainWindow* mainWindow) {
    window = mainWindow;
    renderer = mainWindow->renderer;
    musicBox = mainWindow->mBox;
}

WaveformMenu::~WaveformMenu(){
    renderer = nullptr;
    musicBox = nullptr;
    window = nullptr;
    editedInstrument = nullptr;
    editedOsc = nullptr;
}

void WaveformMenu::init(){
    textColor = {255,255,255};
    editedInstrument = musicBox->instruments.front();
    editedOsc = editedInstrument->oscillators.front();
    helpMessage = "[ENTER] SELECT | [1] SIGNAL | [2] ADD OSC | [3] DEL OSC | [F4] QUIT";
    loadTextures();
    loadControls();
}

void WaveformMenu::loadControls(){
    oscSelector.loadTextControl(SelectorType::OSCILLATOR, &oscCounter, window);
    mainWaveSelector.loadImageControl(waveImages, window);
    lfoWaveSelector.loadImageControl(waveImages, window);
    lfoWaveSelector.isModifyingLFO(true);
    mainFreqSelector.loadTextControl(SelectorType::FREQUENCY, &mainFreqValue, window);
    lfoFreqSelector.loadTextControl(SelectorType::FREQUENCY, &lfoFreqValue, window);
    lfoFreqSelector.isModifyingLFO(true);
    mainAmpSelector.loadTextControl(SelectorType::AMPLITUDE, &mainAmpValue, window);
    lfoAmpSelector.loadTextControl(SelectorType::AMPLITUDE, &lfoAmpValue, window);
    lfoAmpSelector.isModifyingLFO(true);
    oscSelector.isHighlighted = true;
    lfoCheckBox.onCheck = &WaveformMenu::setLFO;
}

void WaveformMenu::loadTextures(){
    setTextTexture(&undefinedLabel, "-");
    setTextTexture(&screenTitle, "INSTRUMENT CREATOR");
    setTextTexture(&oscCounter, "oscillator " + std::to_string(currentOsc+1) + " of " + std::to_string(musicBox->instruments.size()));
    setTextTexture(&wavetypeLabel, "WAVE");
    setTextTexture(&mainWaveSign, "~");
    setTextTexture(&frequencyLabel, "FREQ");
    setTextTexture(&amplitudeLabel, "AMP");
    setTextTexture(&mainFreqValue, doubleToStr(editedOsc->freqModifier, 2));
    setTextTexture(&mainAmpValue, doubleToStr(editedOsc->ampModifier, 2));

    setTextTexture(&lfoFreqValue, "-");
    setTextTexture(&lfoAmpValue, "-");
    lfoWaveSelector.loadTextControl(SelectorType::WAVETYPE, &undefinedLabel, window);

    setTextTexture(&blocksLeftLabel, "0");
    setTextTexture(&helpBar, helpMessage, window->smallFont);
    setImageTexture(&arrowImg, assets_dir + "arrow.png");
    setImageTexture(&sineImg, assets_dir + "sine.png");
    setImageTexture(&squareImg, assets_dir + "square.png");
    setImageTexture(&triangleImg, assets_dir + "triangle.png");
    setImageTexture(&sawDownImg, assets_dir + "sawtoothdown.png");
    setImageTexture(&sawUpImg, assets_dir + "sawtoothup.png");
    setImageTexture(&noiseImg, assets_dir + "noise.png");

    setTextTexture(&lfoLabel, "LFO");

    setImageTexture(&checkImg, assets_dir + "check.png");
    lfoCheckBox.loadTextures("LFO", &lfoLabel, window);
}

void WaveformMenu::updateTextures(){
    setWaveImage(&mainWaveSelector, editedOsc->waveType);
    setTextTexture(&oscCounter, "oscillator " + std::to_string(currentOsc+1) + " of " + std::to_string(editedInstrument->oscillators.size()));
    setTextTexture(&mainFreqValue, doubleToStr(editedOsc->freqModifier, 2));
    setTextTexture(&mainAmpValue, doubleToStr(editedOsc->ampModifier, 2));

    setTextTexture(&blocksLeftLabel, std::to_string(musicBox->blocksLeft));

    if (editedOsc->lfo != nullptr) {
        setWaveImage(&lfoWaveSelector, editedOsc->lfo->waveType);
        setTextTexture(&lfoFreqValue, doubleToStr(editedOsc->lfo->currentFrequency, 2)+"Hz");
        setTextTexture(&lfoAmpValue, doubleToStr(editedOsc->lfo->ampModifier, 2) );
    }
    else {
        setTextTexture(&lfoFreqValue, "-");
        setTextTexture(&lfoAmpValue, "-");
        lfoWaveSelector.loadTextControl(SelectorType::WAVETYPE, &undefinedLabel, window);
    }
}

int WaveformMenu::xByPercent(Texture* texture, double percent){
    return window->w * percent - texture->w * 0.5;
}

int WaveformMenu::yByPercent(Texture* texture, double percent){
    return window->h * percent - texture->h * 0.5;
}

void WaveformMenu::render(){
    updateTextures();

    blocksLeftLabel.render(xByPercent(&blocksLeftLabel, 0.75),
                           yByPercent(&blocksLeftLabel, 0.75));

    // MISC
    screenTitle.render(xByPercent(&screenTitle, 0.3),
                       yByPercent(&screenTitle, 0.1));
    helpBar.render(window->borderSize, window->mainArea.h - helpBar.h);

    // MAIN OSCILLATOR LABELS
    wavetypeLabel.render(xByPercent(&wavetypeLabel, 0.15),
                         yByPercent(&wavetypeLabel, 0.3));
    frequencyLabel.render(xByPercent(&frequencyLabel, 0.3),
                          yByPercent(&frequencyLabel, 0.3));
    amplitudeLabel.render(xByPercent(&amplitudeLabel, 0.45),
                          yByPercent(&amplitudeLabel, 0.3));

    // MAIN OSCILLATOR SELECT CONTROLS
    oscSelector.render(xByPercent(&oscCounter, 0.3),
                          yByPercent(&oscCounter, 0.2));
    mainWaveSelector.render(xByPercent(mainWaveSelector.mainTexture, 0.15),
                            yByPercent(mainWaveSelector.mainTexture, 0.4));
    mainFreqSelector.render(xByPercent(&mainFreqValue, 0.3),
                       yByPercent(&mainFreqValue, 0.4));
    mainAmpSelector.render(xByPercent(&mainAmpValue, 0.45),
                            yByPercent(&mainFreqValue, 0.4));

    // LFO LABELS
//    lfoLabel.render(window->w / 8, window->h * 3 / 4);
    lfoCheckBox.render(xByPercent(lfoCheckBox.labelTexture, 0.05),
                       yByPercent(lfoCheckBox.labelTexture, 0.5));

    // LFO SELECT CONTROLS
    lfoWaveSelector.render(xByPercent(lfoWaveSelector.mainTexture, 0.15),
                           yByPercent(lfoWaveSelector.mainTexture, 0.5));
    lfoFreqSelector.render(xByPercent(&lfoFreqValue, 0.3),
                           yByPercent(&lfoFreqValue, 0.5));
    lfoAmpSelector.render(xByPercent(&lfoAmpValue, 0.45),
                          yByPercent(&lfoAmpValue, 0.5));
}


void WaveformMenu::setTextTexture(Texture* texture, std::string text){
    texture->loadFromText(renderer, text, textColor, window->mainFont);
}

void WaveformMenu::setTextTexture(Texture* texture, std::string text, TTF_Font* font){
    texture->loadFromText(renderer, text, textColor, font);
}

void WaveformMenu::setImageTexture(Texture* texture, std::string imagePath){
    texture->loadFromFile(renderer, imagePath);
}

std::string WaveformMenu::doubleToStr(double d, int precision){
    std::stringstream stream;
    stream << std::fixed << std::setprecision(3) << d;
    return stream.str();
}

void WaveformMenu::selectFocusedControl() {
    getFocusedControl()->activate();
}

Control* WaveformMenu::getFocusedControl(){
    return controlArray[focusedControlRow][focusedControlCol];
}

void WaveformMenu::changeControlFocus(Direction direction){
    getFocusedControl()->switchHighlight();
    switch (direction) {
        case Direction::UP:
            if (focusedControlRow > 0){
                if (controlArray[focusedControlRow-1][focusedControlCol] != nullptr) {
                    focusedControlRow -= 1;
                }
                else {
                    for (int i = 0; i < controlMatrixRows; i++) {
                        if (controlArray[focusedControlRow-1][i] != nullptr){
                            focusedControlRow -= 1;
                            focusedControlCol = i;
                            break;
                        }
                    }
                }
            }
            break;
        case Direction::DOWN:
            if (focusedControlRow < controlMatrixRows - 1){
                if (controlArray[focusedControlRow+1][focusedControlCol] != nullptr) {
                    focusedControlRow += 1;
                }
                else {
                    for (int i = 0; i < controlMatrixCols; i++) {
                        if (controlArray[focusedControlRow+1][i] != nullptr){
                            focusedControlRow += 1;
                            focusedControlCol = i;
                            break;
                        }
                    }
                }
            }
            break;
        case Direction::LEFT:
            if (focusedControlCol > 0){
                if (controlArray[focusedControlRow][focusedControlCol-1] != nullptr) {
                    focusedControlCol -= 1;
                }
                else {
                    for (int i = 0; i < controlMatrixRows; i++) {
                        if (controlArray[i][focusedControlCol-1] != nullptr){
                            focusedControlRow = i;
                            focusedControlCol -= 1;
                            break;
                        }
                    }
                }
            }
            break;
        case Direction::RIGHT:
            if (focusedControlCol < controlMatrixCols - 1){
                if (controlArray[focusedControlRow][focusedControlCol+1] != nullptr) {
                    focusedControlCol += 1;
                }
                else {
                    for (int i = 0; i < controlMatrixRows; i++) {
                        if (controlArray[i][focusedControlCol+1] != nullptr){
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


void WaveformMenu::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_2:
            editedInstrument->addOscillator();
            currentOsc = editedInstrument->oscillators.size() - 1;
            editedOsc = editedInstrument->oscillators.at(currentOsc);
            break;
        case SDLK_3:
            if (editedInstrument->oscillators.size() > 1) {
                editedInstrument->removeOscillator();
                if (currentOsc == editedInstrument->oscillators.size()){
                    oscSelector.decrement(false);
                }
            }
            break;
        case SDLK_9:
            setLFO();
        case SDLK_UP:
            if (getFocusedControl()->isHighlighted){
                changeControlFocus(Direction::UP);
            }
            break;
        case SDLK_DOWN:
            if (getFocusedControl()->isHighlighted){
                changeControlFocus(Direction::DOWN);
            }
            break;
        case SDLK_LEFT:
            if (getFocusedControl()->isHighlighted){
                changeControlFocus(Direction::LEFT);
            }
            if (getFocusedControl()->isEditing){
                getFocusedControl()->decrement(false);
            }
             break;
        case SDLK_RIGHT:
            if (getFocusedControl()->isHighlighted){
                changeControlFocus(Direction::RIGHT);
            }
            if (getFocusedControl()->isEditing){
                getFocusedControl()->increment(false);
            }
            break;
        case SDLK_MINUS:
            if (getFocusedControl()->isEditing){
                getFocusedControl()->decrement(true);
            }
            break;
        case SDLK_EQUALS:
            if (getFocusedControl()->isEditing){
                getFocusedControl()->increment(true);
            }
            break;
        case SDLK_RETURN:
            selectFocusedControl(); break;
        default:
            break;
    }
}

void WaveformMenu::setLFO(){
    if (editedOsc->lfo == nullptr){
        editedOsc->setLFO(44100, WaveformType::SINE);
        editedOsc->lfo->setFrequency(5);
        editedOsc->lfo->setAmpMod(0.5);
    }
    else {
        editedOsc->unsetLFO();
    }
}

void WaveformMenu::setWaveImage(SelectControl* control, WaveformType wavetype){
    switch (wavetype) {
        case WaveformType::SINE:
            control->mainTexture = &sineImg; break;
        case WaveformType::SQUARE:
            control->mainTexture = &squareImg; break;
        case WaveformType::TRIANGLE:
            control->mainTexture = &triangleImg; break;
        case WaveformType::SAWTOOTHDOWN:
            control->mainTexture = &sawDownImg; break;
        case WaveformType::SAWTOOTHUP:
            control->mainTexture = &sawUpImg; break;
        case WaveformType::NOISE:
            control->mainTexture = &noiseImg; break;
    }
}

void WaveformMenu::nextOsc(bool reverse){
    if (reverse) {
        if (currentOsc > 0){
            currentOsc--;
            editedOsc = editedInstrument->oscillators.at(currentOsc);
        }
    }
    else if (currentOsc < editedInstrument->oscillators.size()-1){
        currentOsc++;
        editedOsc = editedInstrument->oscillators.at(currentOsc);
    }
    lfoCheckBox.isChecked = editedOsc->lfo != nullptr;

}

