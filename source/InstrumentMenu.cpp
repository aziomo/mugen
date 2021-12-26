#include "../include/InstrumentMenu.h"
#include <string>
#include <iomanip>
#include <sstream>

using std::string;
using std::to_string;

InstrumentMenu::InstrumentMenu(MainWindow *mainWindow) {
    window = mainWindow;
    renderer = mainWindow->renderer;
    musicBox = mainWindow->mBox;
    itemList = new ItemList(this->renderer, window->smallFont, window->mainArea.w / 5, window->mainArea.h * 1 / 2, 4);
}

InstrumentMenu::~InstrumentMenu() {
    renderer = nullptr;
    musicBox = nullptr;
    window = nullptr;
    editedInstrument = nullptr;
    editedOsc = nullptr;
}

void InstrumentMenu::init() {
    textColor = {255, 255, 255};
    editedInstrument = musicBox->instruments.front();
    editedOsc = editedInstrument->oscillators.front();

    itemList->addItem("Instrument 1");
    itemList->addItem("+ New instrument");
    helpMessage = "[ENTER] SELECT | [2] ADD OSC | [3] DEL OSC | [F4] QUIT";
    loadTextures();
    loadControls();
}

void InstrumentMenu::loadControls() {
    oscSelector.loadTextControl(SelectorType::OSCILLATOR, &oscCounter, window);

    mainWaveSelector.loadImageControl(waveImages, window);
    mainFreqSelector.loadTextControl(SelectorType::FINEDOUBLE, &mainFreqValue, window);
    mainAmpSelector.loadTextControl(SelectorType::FINEDOUBLE, &mainAmpValue, window);

    lfoWaveSelector.loadImageControl(waveImages, window);
    lfoWaveSelector.isModifyingLFO(true);
    lfoFreqSelector.loadTextControl(SelectorType::FINEDOUBLE, &lfoFreqValue, window);
    lfoAmpSelector.loadTextControl(SelectorType::FINEDOUBLE, &lfoAmpValue, window);

    envInitialSelector.loadTextControl(SelectorType::FINEDOUBLE, &initialValue, window);
    envSustainSelector.loadTextControl(SelectorType::FINEDOUBLE, &sustainValue, window);
    envAttackSelector.loadTextControl(SelectorType::FINEDOUBLE, &attackValue, window);
    envDecaySelector.loadTextControl(SelectorType::FINEDOUBLE, &decayValue, window);
    envReleaseSelector.loadTextControl(SelectorType::FINEDOUBLE, &releaseValue, window);

    oscSelector.isHighlighted = true;
    lfoCheckBox.onCheck = &InstrumentMenu::setLFO;
    updateSelectorValues();
}

void InstrumentMenu::updateSelectorValues() {
    mainFreqSelector.setModifiedValue(&editedOsc->freqModifier);
    mainAmpSelector.setModifiedValue(&editedOsc->ampModifier);
    if (editedOsc->lfo != nullptr) {
        lfoFreqSelector.setModifiedValue(&editedOsc->lfo->currentFrequency);
        lfoAmpSelector.setModifiedValue(&editedOsc->lfo->ampModifier);
    }
    envInitialSelector.setModifiedValue(&editedInstrument->env.initialAmplitude);
    envSustainSelector.setModifiedValue(&editedInstrument->env.sustainAmplitude);
    envAttackSelector.setModifiedValue(&editedInstrument->env.attackDuration);
    envDecaySelector.setModifiedValue(&editedInstrument->env.decayDuration);
    envReleaseSelector.setModifiedValue(&editedInstrument->env.releaseDuration);
}

void InstrumentMenu::loadTextures() {
    setTextTexture(&undefinedLabel, "-");
    setTextTexture(&instrumentsTab, "INSTRUMENTS");
    setTextTexture(&compositionTab, "COMPOSITION");
    setTextTexture(&optionsTab, "OPTIONS");

    setTextTexture(&instrumentName, "Instrument 1", window->largeFont);

    setTextTexture(&oscLabel, "OSCILLATOR");
    setTextTexture(&wavetypeLabel, "WAVE");
    setTextTexture(&frequencyLabel, "FREQ");
    setTextTexture(&amplitudeLabel, "AMP");

    setTextTexture(&helpBar, helpMessage, window->tinyFont);
    setImageTexture(&arrowImg, assets_dir + "arrow.png");
    setImageTexture(&sineImg, assets_dir + "sine.png");
    setImageTexture(&squareImg, assets_dir + "square.png");
    setImageTexture(&triangleImg, assets_dir + "triangle.png");
    setImageTexture(&sawDownImg, assets_dir + "sawtoothdown.png");
    setImageTexture(&sawUpImg, assets_dir + "sawtoothup.png");
    setImageTexture(&noiseImg, assets_dir + "noise.png");

    setImageTexture(&checkImg, assets_dir + "check.png");
    setTextTexture(&lfoLabel, "LFO");
    lfoCheckBox.loadTextures(&lfoLabel, window, true);

    setTextTexture(&envelopeLabel, "ENVELOPE");
    setTextTexture(&durationsLabel, "DURATIONS");
    setTextTexture(&attackLabel, "Attack");
    setTextTexture(&decayLabel, "Decay");
    setTextTexture(&releaseLabel, "Release");
    setTextTexture(&levelsLabel, "LEVELS");
    setTextTexture(&initialLabel, "Initial");
    setTextTexture(&sustainLabel, "Sustain");
}

void InstrumentMenu::updateTextures() {
    // waveform textures
    setWaveImage(&mainWaveSelector, editedOsc->waveType);
    setTextTexture(&oscCounter, to_string(currentOsc + 1) + " of " +
                                to_string(editedInstrument->oscillators.size()));
    setTextTexture(&mainFreqValue, "x"+doubleToStr(editedOsc->freqModifier, 2));
    setTextTexture(&mainAmpValue, "x"+doubleToStr(editedOsc->ampModifier, 2));

    if (editedOsc->lfo != nullptr) {
        setWaveImage(&lfoWaveSelector, editedOsc->lfo->waveType);
        setTextTexture(&lfoFreqValue, doubleToStr(editedOsc->lfo->currentFrequency, 2) + "Hz");
        setTextTexture(&lfoAmpValue, "x"+doubleToStr(editedOsc->lfo->ampModifier, 2));
        lfoCheckBox.isChecked = true;
    } else {
        setTextTexture(&lfoFreqValue, "-");
        setTextTexture(&lfoAmpValue, "-");
        lfoWaveSelector.loadTextControl(SelectorType::WAVETYPE, &undefinedLabel, window);
        lfoCheckBox.isChecked = false;
    }

    // envelope textures
    setTextTexture(&initialValue, doubleToStr(editedInstrument->env.initialAmplitude, 2));
    setTextTexture(&attackValue, doubleToStr(editedInstrument->env.attackDuration, 2));
    setTextTexture(&decayValue, doubleToStr(editedInstrument->env.decayDuration, 2));
    setTextTexture(&sustainValue, doubleToStr(editedInstrument->env.sustainAmplitude, 2));
    setTextTexture(&releaseValue, doubleToStr(editedInstrument->env.releaseDuration, 2));

    // debug controls
    setTextTexture(&debugBlocksLeftLabel, to_string(musicBox->blocksAvailable));
    setTextTexture(&debugMaxSampleLabel, to_string(musicBox->maxSample));
    if (editedOsc->lfo != nullptr)
        setTextTexture(&debugCurrentFrequencyLabel,
                       to_string(sin(editedOsc->getLfoInterpolatedSample(musicBox->globalTime))));
    setTextTexture(&debugEnvelopeMomentLabel, to_string(musicBox->instruments.front()->envelopeMoment));
}

int InstrumentMenu::xByPercent(Texture* texture, double percent) const {
    return window->w * percent - texture->w * 0.5;
}

int InstrumentMenu::xByPercent(SDL_Rect* rect, double percent) const {
    return window->w * percent - rect->w * 0.5;
}

int InstrumentMenu::yByPercent(Texture* texture, double percent) const {
    return window->h * percent - texture->h * 0.5;
}

int InstrumentMenu::yByPercent(SDL_Rect* rect, double percent) const {
    return window->h * percent - rect->h * 0.5;
}

void InstrumentMenu::render() {
    updateTextures();
    double waveMenuOffsetX = 0.4;
    double waveMenuOffsetY = 0.175;
    double envMenuOffsetX = 0.8;
    double envMenuOffsetY = 0.8;

    if (showDebug) {
        debugBlocksLeftLabel.render(xByPercent(&debugBlocksLeftLabel, 0.75),
                                    yByPercent(&debugBlocksLeftLabel, 0.75));
        debugMaxSampleLabel.render(xByPercent(&debugMaxSampleLabel, 0.75),
                                   yByPercent(&debugMaxSampleLabel, 0.80));
        debugCurrentFrequencyLabel.render(xByPercent(&debugCurrentFrequencyLabel, 0.75),
                                          yByPercent(&debugCurrentFrequencyLabel, 0.85));
        debugEnvelopeMomentLabel.render(xByPercent(&debugCurrentFrequencyLabel, 0.75),
                                        yByPercent(&debugCurrentFrequencyLabel, 0.90));
    }

    //render tab outlines somehow

    // MISC
    instrumentsTab.render(xByPercent(&instrumentsTab, 0.18),
                          yByPercent(&instrumentsTab, 0.055));
    compositionTab.render(xByPercent(&compositionTab, 0.5),
                          yByPercent(&compositionTab, 0.055));
    optionsTab.render(xByPercent(&optionsTab, 0.825),
                      yByPercent(&optionsTab, 0.055));
    helpBar.render(window->borderSize * 2, window->mainArea.h - helpBar.h);

//    instrumentName.render(xByPercent(&instrumentName, 0.5),
//                          yByPercent(&instrumentName, 0.18));

    // MAIN OSCILLATOR LABELS
    oscLabel.render(xByPercent(&oscLabel, waveMenuOffsetX + 0.075),
                    yByPercent(&oscLabel, waveMenuOffsetY + 0.025));
    wavetypeLabel.render(xByPercent(&wavetypeLabel, waveMenuOffsetX),
                         yByPercent(&wavetypeLabel, waveMenuOffsetY + 0.17));
    frequencyLabel.render(xByPercent(&frequencyLabel, waveMenuOffsetX + 0.1),
                          yByPercent(&frequencyLabel, waveMenuOffsetY + 0.17));
    amplitudeLabel.render(xByPercent(&amplitudeLabel, waveMenuOffsetX + 0.1 * 2),
                          yByPercent(&amplitudeLabel, waveMenuOffsetY + 0.17));

    // MAIN OSCILLATOR SELECTORS
    oscSelector.render(xByPercent(&oscCounter, waveMenuOffsetX + 0.075),
                       yByPercent(&oscCounter, waveMenuOffsetY + 0.075));
    mainWaveSelector.render(xByPercent(mainWaveSelector.mainTexture, waveMenuOffsetX),
                            yByPercent(mainWaveSelector.mainTexture, waveMenuOffsetY + 0.25));
    mainFreqSelector.render(xByPercent(&mainFreqValue, waveMenuOffsetX + 0.1),
                            yByPercent(&mainFreqValue, waveMenuOffsetY + 0.25));
    mainAmpSelector.render(xByPercent(&mainAmpValue, waveMenuOffsetX + 0.1 * 2),
                           yByPercent(&mainFreqValue, waveMenuOffsetY + 0.25));

    // LFO LABELS
    lfoCheckBox.render(xByPercent(&lfoCheckBox.renderArea, waveMenuOffsetX - 0.085),
                       yByPercent(&lfoCheckBox.renderArea, waveMenuOffsetY + 0.35));

    // LFO SELECTORS
    lfoWaveSelector.render(xByPercent(lfoWaveSelector.mainTexture, waveMenuOffsetX),
                           yByPercent(lfoWaveSelector.mainTexture, waveMenuOffsetY + 0.35));
    lfoFreqSelector.render(xByPercent(&lfoFreqValue, waveMenuOffsetX + 0.1),
                           yByPercent(&lfoFreqValue, waveMenuOffsetY + 0.35));
    lfoAmpSelector.render(xByPercent(&lfoAmpValue, waveMenuOffsetX + 0.1 * 2),
                          yByPercent(&lfoAmpValue, waveMenuOffsetY + 0.35));


    // ENVELOPE LABELS
    envelopeLabel.render(xByPercent(&envelopeLabel, envMenuOffsetX + 0.05),
                         yByPercent(&envelopeLabel, 0.2));


    levelsLabel.render(xByPercent(&levelsLabel, envMenuOffsetX + 0.05),
                       yByPercent(&levelsLabel, 0.3));
    initialLabel.render(xByPercent(&initialLabel, envMenuOffsetX),
                        yByPercent(&initialLabel, 0.35));
    sustainLabel.render(xByPercent(&sustainLabel, envMenuOffsetX),
                        yByPercent(&sustainLabel, 0.4));

    durationsLabel.render(xByPercent(&durationsLabel, envMenuOffsetX + 0.05),
                          yByPercent(&durationsLabel, 0.5));
    attackLabel.render(xByPercent(&attackLabel, envMenuOffsetX),
                       yByPercent(&attackLabel, 0.55));
    decayLabel.render(xByPercent(&decayLabel, envMenuOffsetX),
                      yByPercent(&decayLabel, 0.6));
    releaseLabel.render(xByPercent(&releaseLabel, envMenuOffsetX),
                        yByPercent(&releaseLabel, 0.65));

    // ENVELOPE SELECTORS
    envInitialSelector.render(xByPercent(envInitialSelector.mainTexture, envMenuOffsetX + 0.12),
                              yByPercent(envInitialSelector.mainTexture, 0.35));
    envSustainSelector.render(xByPercent(envSustainSelector.mainTexture, envMenuOffsetX + 0.12),
                              yByPercent(envSustainSelector.mainTexture, 0.4));

    envAttackSelector.render(xByPercent(envAttackSelector.mainTexture, envMenuOffsetX + 0.12),
                             yByPercent(envAttackSelector.mainTexture, 0.55));
    envDecaySelector.render(xByPercent(envDecaySelector.mainTexture, envMenuOffsetX + 0.12),
                            yByPercent(envDecaySelector.mainTexture, 0.6));
    envReleaseSelector.render(xByPercent(envReleaseSelector.mainTexture, envMenuOffsetX + 0.12),
                              yByPercent(envReleaseSelector.mainTexture, 0.65));

    itemList->render(window->borderSize, window->mainArea.h/4);
}


void InstrumentMenu::setTextTexture(Texture* texture, string text) const {
    texture->loadFromText(renderer, text, textColor, window->mainFont);
}

void InstrumentMenu::setTextTexture(Texture* texture, string text, TTF_Font* font) const {
    texture->loadFromText(renderer, text, textColor, font);
}

void InstrumentMenu::setImageTexture(Texture *texture, string imagePath) const {
    texture->loadFromFile(renderer, imagePath);
}

string InstrumentMenu::doubleToStr(double d, int precision) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << d;
    return stream.str();
}

void InstrumentMenu::selectFocusedControl() {
    getFocusedControl()->activate();
}

Control* InstrumentMenu::getFocusedControl() {
    return controlArray[focusedControlRow][focusedControlCol];
}

void InstrumentMenu::changeControlFocus(Direction direction) {
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
            if (listFocused){
                break;
            }
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
            } else {
                listFocused = true;
                getFocusedControl()->switchHighlight();
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


void InstrumentMenu::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_2:
            if (getFocusedControl()->isEditing) {
                selectFocusedControl();
            }
            editedInstrument->addOscillator();
            while (currentOsc != editedInstrument->oscillators.size()-1)
                selectNextOsc();
            break;
        case SDLK_3:
            if (editedInstrument->oscillators.size() > 1) {
                if (getFocusedControl()->isEditing) {
                    selectFocusedControl();
                }
                editedInstrument->removeOscillator();
                if (currentOsc == editedInstrument->oscillators.size()) {
                    selectPrevOsc();
                }
            }
            break;
        case SDLK_i:
            drawIntegral = !drawIntegral;
            break;
        case SDLK_UP:
            if (listFocused){
                int prevIndex = itemList->selectedIndex;
                itemList->moveUp();
                if (prevIndex > itemList->selectedIndex && prevIndex != itemList->items.size()-1){
                    editedInstrument = musicBox->instruments.at(itemList->selectedIndex);
                    editedOsc = editedInstrument->oscillators.front();
                    musicBox->currentInstrument = itemList->selectedIndex;
                    updateSelectorValues();
                }
                break;
            }
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::UP);
            }
            if (getFocusedControl()->isEditing) {
                getFocusedControl()->increment(true);
            }
            break;
        case SDLK_DOWN:
            if (listFocused){
                int prevIndex = itemList->selectedIndex;
                itemList->moveDown();
                if (prevIndex < itemList->selectedIndex && itemList->selectedIndex != itemList->items.size()-1){
                    editedInstrument = musicBox->instruments.at(itemList->selectedIndex);
                    editedOsc = editedInstrument->oscillators.front();
                    musicBox->currentInstrument = itemList->selectedIndex;
                    updateSelectorValues();
                }
                break;
            }
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
            if (listFocused){
                if (itemList->selectedIndex == itemList->items.size()-1)
                    itemList->moveUp();
                listFocused = false;
                getFocusedControl()->switchHighlight();
                break;
            }
            if (getFocusedControl()->isHighlighted) {
                changeControlFocus(Direction::RIGHT);
            }
            if (getFocusedControl()->isEditing) {
                getFocusedControl()->increment(false);
            }
            break;

        case SDLK_RETURN:
            if (listFocused){
                selectItemFromList(itemList->selectedIndex);
                break;
            }
            selectFocusedControl();
            break;

        case SDLK_w:
            musicBox->outputFile == nullptr ? musicBox->openFile() : musicBox->closeFile();
            break;

        default:
            break;
    }
}

void InstrumentMenu::setLFO() {
    if (editedOsc->lfo == nullptr) {
        editedOsc->setLFO(WaveformType::SINE);
        editedOsc->lfo->currentFrequency = 5.0;
        editedOsc->lfo->ampModifier = 0.05;
        updateSelectorValues();
    } else {
        editedOsc->unsetLFO();
    }
}

void InstrumentMenu::setWaveImage(SelectControl *control, WaveformType wavetype) {
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

void InstrumentMenu::selectNextOsc() {
    if (currentOsc < editedInstrument->oscillators.size() - 1) {
        currentOsc++;
        editedOsc = editedInstrument->oscillators.at(currentOsc);
        updateSelectorValues();
    }
    lfoCheckBox.isChecked = editedOsc->lfo != nullptr;
}

void InstrumentMenu::selectPrevOsc() {
    if (currentOsc > 0) {
        currentOsc--;
        editedOsc = editedInstrument->oscillators.at(currentOsc);
        updateSelectorValues();
    }
    lfoCheckBox.isChecked = editedOsc->lfo != nullptr;
}

void InstrumentMenu::selectItemFromList(int index) {

    if (index == itemList->items.size() - 1){
        musicBox->instruments.push_back(new Instrument(musicBox->blockSize));
        itemList->removeItem(itemList->items.size() - 1);
        itemList->addItem("Instrument " + to_string(itemList->items.size()+1));
        itemList->addItem("+ New instrument");
    }
}

