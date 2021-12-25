#include "../include/InstrumentMenu.h"
#include <string>
#include <iomanip>
#include <sstream>

InstrumentMenu::InstrumentMenu(MainWindow* mainWindow) {
    window = mainWindow;
    renderer = mainWindow->renderer;
    musicBox = mainWindow->mBox;
}

InstrumentMenu::~InstrumentMenu(){
    renderer = nullptr;
    musicBox = nullptr;
    window = nullptr;
    editedInstrument = nullptr;
    editedOsc = nullptr;
}

void InstrumentMenu::init(){
    textColor = {255,255,255};
    editedInstrument = musicBox->instruments.front();
    editedOsc = editedInstrument->oscillators.front();
    helpMessage = "[ENTER] SELECT | [1] SIGNAL | [2] ADD OSC | [3] DEL OSC | [F4] QUIT";
    loadTextures();
    loadControls();
}

void InstrumentMenu::loadControls(){
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
    lfoCheckBox.onCheck = &InstrumentMenu::setLFO;

    envStartingSelector.loadTextControl(SelectorType::ENVELOPE, &startValue, window);
    envStartingSelector.setModifiedValue(&editedInstrument->env.startingAmplitude);

    envSustainSelector.loadTextControl(SelectorType::ENVELOPE, &sustainValue, window);
    envSustainSelector.setModifiedValue(&editedInstrument->env.sustainAmplitude);

    envAttackSelector.loadTextControl(SelectorType::ENVELOPE, &attackValue, window);
    envAttackSelector.setModifiedValue(&editedInstrument->env.attackDuration);

    envDecaySelector.loadTextControl(SelectorType::ENVELOPE, &decayValue, window);
    envDecaySelector.setModifiedValue(&editedInstrument->env.decayDuration);

    envReleaseSelector.loadTextControl(SelectorType::ENVELOPE, &releaseValue, window);
    envReleaseSelector.setModifiedValue(&editedInstrument->env.releaseDuration);

}

void InstrumentMenu::loadTextures()
{
    setTextTexture(&undefinedLabel, "-");
    setTextTexture(&screenTitle, "INSTRUMENT CREATOR");
    setTextTexture(&oscCounter, "OSCILLATOR " + std::to_string(currentOsc+1) + " of " + std::to_string(musicBox->instruments.size()));
    setTextTexture(&wavetypeLabel, "WAVE");
    setTextTexture(&mainWaveSign, "~");
    setTextTexture(&frequencyLabel, "FREQ");
    setTextTexture(&amplitudeLabel, "AMP");
    setTextTexture(&mainFreqValue, doubleToStr(editedOsc->freqModifier, 3));
    setTextTexture(&mainAmpValue, doubleToStr(editedOsc->ampModifier, 3));

    setTextTexture(&lfoFreqValue, "-");
    setTextTexture(&lfoAmpValue, "-");
    lfoWaveSelector.loadTextControl(SelectorType::WAVETYPE, &undefinedLabel, window);


    setTextTexture(&envelopeLabel, "ENVELOPE");

    setTextTexture(&durationsLabel, "DURATIONS");
    setTextTexture(&attackLabel, "Attack");
    setTextTexture(&decayLabel, "Decay");
    setTextTexture(&releaseLabel, "Release");

    setTextTexture(&levelsLabel, "LEVELS");
    setTextTexture(&startLabel, "Initial");
    setTextTexture(&sustainLabel, "Sustain");

    setTextTexture(&attackValue, "0.0");
    setTextTexture(&decayValue, "0.0");
    setTextTexture(&sustainValue, "0.0");
    setTextTexture(&releaseValue, "0.0");
    setTextTexture(&startValue, "0.0");



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


    //debug controls
    setTextTexture(&debugBlocksLeftLabel, "0");
    setTextTexture(&debugMaxSampleLabel, "0.0");
    setTextTexture(&debugCurrentFrequencyLabel, "0.0");
    setTextTexture(&debugEnvelopeMomentLabel, "0");
}

void InstrumentMenu::updateTextures()
{
    // waveform textures
    setWaveImage(&mainWaveSelector, editedOsc->waveType);
    setTextTexture(&oscCounter, "OSCILLATOR " + std::to_string(currentOsc+1) + " OF " + std::to_string(editedInstrument->oscillators.size()));
    setTextTexture(&mainFreqValue, doubleToStr(editedOsc->freqModifier, 3));
    setTextTexture(&mainAmpValue, doubleToStr(editedOsc->ampModifier, 3));

    if (editedOsc->lfo != nullptr) {
        setWaveImage(&lfoWaveSelector, editedOsc->lfo->waveType);
        setTextTexture(&lfoFreqValue, doubleToStr(editedOsc->lfo->currentFrequency, 3)+"Hz");
        setTextTexture(&lfoAmpValue, doubleToStr(editedOsc->lfo->ampModifier, 3) );
        lfoCheckBox.isChecked = true;
    }
    else {
        setTextTexture(&lfoFreqValue, "-");
        setTextTexture(&lfoAmpValue, "-");
        lfoWaveSelector.loadTextControl(SelectorType::WAVETYPE, &undefinedLabel, window);
        lfoCheckBox.isChecked = false;
    }

    // envelope textures

    setTextTexture(&startValue, doubleToStr(editedInstrument->env.startingAmplitude, 1));
    setTextTexture(&attackValue, doubleToStr(editedInstrument->env.attackDuration, 1));
    setTextTexture(&decayValue, doubleToStr(editedInstrument->env.decayDuration, 1));
    setTextTexture(&sustainValue, doubleToStr(editedInstrument->env.sustainAmplitude, 1));
    setTextTexture(&releaseValue, doubleToStr(editedInstrument->env.releaseDuration, 1));


    // debug controls
    setTextTexture(&debugBlocksLeftLabel, std::to_string(musicBox->blocksAvailable));
    setTextTexture(&debugMaxSampleLabel, std::to_string(musicBox->maxSample));

    if (musicBox->instruments.front()->oscillators.front()->lfo != nullptr)
        setTextTexture(&debugCurrentFrequencyLabel, std::to_string(sin(
            musicBox->instruments.front()->oscillators.front()->getLfoInterpolatedSample(musicBox->globalTime))));
    setTextTexture(&debugEnvelopeMomentLabel, std::to_string(musicBox->instruments.front()->envelopeMoment));
}

int InstrumentMenu::xByPercent(Texture* texture, double percent){
    return window->w * percent - texture->w * 0.5;
}

int InstrumentMenu::yByPercent(Texture* texture, double percent){
    return window->h * percent - texture->h * 0.5;
}

void InstrumentMenu::render(){
    updateTextures();

    if (showDebug){
        debugBlocksLeftLabel.render(xByPercent(&debugBlocksLeftLabel, 0.75),
                                    yByPercent(&debugBlocksLeftLabel, 0.75));
        debugMaxSampleLabel.render(xByPercent(&debugMaxSampleLabel, 0.75),
                                   yByPercent(&debugMaxSampleLabel, 0.80));
        debugCurrentFrequencyLabel.render(xByPercent(&debugCurrentFrequencyLabel, 0.75),
                                          yByPercent(&debugCurrentFrequencyLabel, 0.85));
        debugEnvelopeMomentLabel.render(xByPercent(&debugCurrentFrequencyLabel, 0.75),
                                        yByPercent(&debugCurrentFrequencyLabel, 0.90));
    }

    // MISC
    screenTitle.render(xByPercent(&screenTitle, 0.5),
                       yByPercent(&screenTitle, 0.1));
    helpBar.render(window->borderSize * 2, window->mainArea.h - helpBar.h);

    // MAIN OSCILLATOR LABELS
    wavetypeLabel.render(xByPercent(&wavetypeLabel, 0.15),
                         yByPercent(&wavetypeLabel, 0.3));
    frequencyLabel.render(xByPercent(&frequencyLabel, 0.15 + 0.12),
                          yByPercent(&frequencyLabel, 0.3));
    amplitudeLabel.render(xByPercent(&amplitudeLabel, 0.15 + 0.12 * 2),
                          yByPercent(&amplitudeLabel, 0.3));

    // MAIN OSCILLATOR SELECT CONTROLS
    oscSelector.render(xByPercent(&oscCounter, 0.15 + 0.12),
                          yByPercent(&oscCounter, 0.2));
    mainWaveSelector.render(xByPercent(mainWaveSelector.mainTexture, 0.15),
                            yByPercent(mainWaveSelector.mainTexture, 0.4));
    mainFreqSelector.render(xByPercent(&mainFreqValue, 0.15 + 0.12),
                       yByPercent(&mainFreqValue, 0.4));
    mainAmpSelector.render(xByPercent(&mainAmpValue, 0.15 + 0.12 * 2),
                            yByPercent(&mainFreqValue, 0.4));

    // LFO LABELS
//    lfoLabel.render(window->w / 8, window->h * 3 / 4);
    lfoCheckBox.render(xByPercent(lfoCheckBox.labelTexture, 0.05),
                       yByPercent(lfoCheckBox.labelTexture, 0.5));

    // LFO SELECT CONTROLS
    lfoWaveSelector.render(xByPercent(lfoWaveSelector.mainTexture, 0.15),
                           yByPercent(lfoWaveSelector.mainTexture, 0.5));
    lfoFreqSelector.render(xByPercent(&lfoFreqValue, 0.15 + 0.12),
                           yByPercent(&lfoFreqValue, 0.5));
    lfoAmpSelector.render(xByPercent(&lfoAmpValue, 0.15 + 0.12 * 2),
                          yByPercent(&lfoAmpValue, 0.5));

    // ENVELOPE LABELS
    envelopeLabel.render(xByPercent(&envelopeLabel, 0.75),
                         yByPercent(&envelopeLabel, 0.2));


    levelsLabel.render(xByPercent(&levelsLabel, 0.75),
                       yByPercent(&levelsLabel, 0.3));
    startLabel.render(xByPercent(&startLabel, 0.7),
                         yByPercent(&startLabel, 0.35));
    sustainLabel.render(xByPercent(&sustainLabel, 0.7),
                      yByPercent(&sustainLabel, 0.4));

    durationsLabel.render(xByPercent(&durationsLabel, 0.75),
                          yByPercent(&durationsLabel, 0.5));
    attackLabel.render(xByPercent(&attackLabel, 0.7),
                      yByPercent(&attackLabel, 0.55));
    decayLabel.render(xByPercent(&decayLabel, 0.7),
                       yByPercent(&decayLabel, 0.6));
    releaseLabel.render(xByPercent(&releaseLabel, 0.7),
                      yByPercent(&releaseLabel, 0.65));


    // ENVELOPE CONTROLS
    envStartingSelector.render(xByPercent(envStartingSelector.mainTexture, 0.8),
                      yByPercent(envStartingSelector.mainTexture, 0.35));
    envSustainSelector.render(xByPercent(envSustainSelector.mainTexture, 0.8),
                        yByPercent(envSustainSelector.mainTexture, 0.4));

    envAttackSelector.render(xByPercent(envAttackSelector.mainTexture, 0.8),
                       yByPercent(envAttackSelector.mainTexture, 0.55));
    envDecaySelector.render(xByPercent(envDecaySelector.mainTexture, 0.8),
                      yByPercent(envDecaySelector.mainTexture, 0.6));
    envReleaseSelector.render(xByPercent(envReleaseSelector.mainTexture, 0.8),
                        yByPercent(envReleaseSelector.mainTexture, 0.65));
}


void InstrumentMenu::setTextTexture(Texture* texture, std::string text){
    texture->loadFromText(renderer, text, textColor, window->mainFont);
}

void InstrumentMenu::setTextTexture(Texture* texture, std::string text, TTF_Font* font){
    texture->loadFromText(renderer, text, textColor, font);
}

void InstrumentMenu::setImageTexture(Texture* texture, std::string imagePath){
    texture->loadFromFile(renderer, imagePath);
}

std::string InstrumentMenu::doubleToStr(double d, int precision){
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << d;
    return stream.str();
}

void InstrumentMenu::selectFocusedControl() {
    getFocusedControl()->activate();
}

Control* InstrumentMenu::getFocusedControl(){
    return controlArray[focusedControlRow][focusedControlCol];
}

void InstrumentMenu::changeControlFocus(Direction direction){
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
    if (focusedControlCol == 4)
        bool retard = true;
    getFocusedControl()->switchHighlight();
}


void InstrumentMenu::handleKeyPress(SDL_Keycode key) {
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
        case SDLK_i:
            drawIntegral = !drawIntegral;
            break;
        case SDLK_9:
            setLFO();
        case SDLK_UP:
            if (getFocusedControl()->isHighlighted){
                changeControlFocus(Direction::UP);
            }
            if (getFocusedControl()->isEditing){
                getFocusedControl()->increment(true);
            }
            break;
        case SDLK_DOWN:
            if (getFocusedControl()->isHighlighted){
                changeControlFocus(Direction::DOWN);
            }
            if (getFocusedControl()->isEditing){
                getFocusedControl()->decrement(true);
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

        case SDLK_w:
            if (musicBox->outputFile == nullptr){
                musicBox->openFile();
            }
            else {
                musicBox->closeFile();
            }

            break;
        default:
            break;
    }
}

void InstrumentMenu::setLFO(){
    if (editedOsc->lfo == nullptr){
        editedOsc->setLFO(44100, WaveformType::SINE);
        editedOsc->lfo->setFrequency(1);
        editedOsc->lfo->setAmpMod(0.01);
    }
    else {
        editedOsc->unsetLFO();
    }
}

void InstrumentMenu::setWaveImage(SelectControl* control, WaveformType wavetype){
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

void InstrumentMenu::nextOsc(bool reverse){
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

