#include "../include/WaveformMenu.h"
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

void WaveformMenu::updateTextures(){
    setWaveImage(&mainWaveSelector, editedOsc->waveType);
    setTextTexture(&oscCounter, "oscillator " + std::to_string(currentOsc+1) + " of " + std::to_string(editedInstrument->oscillators.size()));
    setTextTexture(&mainFreqValue, doubleToStr(editedOsc->freqModifier, 2));
    setTextTexture(&mainAmpValue, doubleToStr(editedOsc->ampModifier, 2));



    if (editedOsc->lfo != nullptr) {
        setWaveImage(&lfoWaveSelector, editedOsc->lfo->waveType);
        setTextTexture(&lfoFreqValue, doubleToStr(editedOsc->lfo->currentFrequency, 2)+"Hz");
        setTextTexture(&lfoAmpValue, doubleToStr(editedOsc->lfo->ampModifier, 2) );
        lfoCheckBox.isChecked = true;
    }
    else {
        setTextTexture(&lfoFreqValue, "-");
        setTextTexture(&lfoAmpValue, "-");
        lfoWaveSelector.loadTextControl(SelectorType::WAVETYPE, &undefinedLabel, window);
        lfoCheckBox.isChecked = false;
    }

    // debug controls
    setTextTexture(&debugBlocksLeftLabel, std::to_string(musicBox->blocksAvailable));
    setTextTexture(&debugMaxSampleLabel, std::to_string(musicBox->maxSample));

    if (musicBox->instruments.front()->oscillators.front()->lfo != nullptr)
        setTextTexture(&debugCurrentFrequencyLabel, std::to_string(sin(
            musicBox->instruments.front()->oscillators.front()->getLfoInterpolatedSample(musicBox->globalTime))));
    setTextTexture(&debugEnvelopeMomentLabel, std::to_string(musicBox->instruments.front()->envelopeMoment));
}

int WaveformMenu::xByPercent(Texture* texture, double percent){
    return window->w * percent - texture->w * 0.5;
}

int WaveformMenu::yByPercent(Texture* texture, double percent){
    return window->h * percent - texture->h * 0.5;
}

void WaveformMenu::render(){
    updateTextures();

    debugBlocksLeftLabel.render(xByPercent(&debugBlocksLeftLabel, 0.75),
                                yByPercent(&debugBlocksLeftLabel, 0.75));
    debugMaxSampleLabel.render(xByPercent(&debugMaxSampleLabel, 0.75),
                               yByPercent(&debugMaxSampleLabel, 0.80));
    debugCurrentFrequencyLabel.render(xByPercent(&debugCurrentFrequencyLabel, 0.75),
                               yByPercent(&debugCurrentFrequencyLabel, 0.85));
    debugEnvelopeMomentLabel.render(xByPercent(&debugCurrentFrequencyLabel, 0.75),
                                    yByPercent(&debugCurrentFrequencyLabel, 0.90));

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

void WaveformMenu::renderGraph(bool fullscreen){
    // have a rect destined for drawing
    SDL_Rect drawingArea;
    if (fullscreen){
        drawingArea = {window->borderSize*10, window->borderSize*10, window->w - window->borderSize*20, window->h - window->borderSize*20};
    }
    else {
        drawingArea = {window->w*5/9 + window->borderSize, window->borderSize*2, window->w*4/9 - window->borderSize*3, window->h/2 - window->borderSize*2};
    }

    double xIncrement = TWOPI / drawingArea.w;
    double startingX = 0.0;
    double currentX = startingX;
    double currentY = YofX(currentX);

    double xAxisAltitude = drawingArea.y + (drawingArea.h * 0.5);

    SDL_Point point, lastPoint;
//    bool drawIntegral = true;

    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer, drawingArea.x, xAxisAltitude, drawingArea.x + drawingArea.w, xAxisAltitude);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);


    if (drawIntegral)
    {
        double maxSample = 0.0;
        double minSample = abs(currentY);

        for (int i = 0; i < drawingArea.w; i++){
            currentX += xIncrement;
            currentY += YofX(currentX);

            if (maxSample < abs(currentY)){
                maxSample = abs(currentY);
            }
            if (minSample > abs(currentY)){
                minSample = abs(currentY);
            }
        }

        currentX = startingX;
        currentY = YofX(currentX);

        double normalizedY = (currentY - minSample)/(maxSample - minSample);

        point = {drawingArea.x, static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * normalizedY)};
        lastPoint = {drawingArea.x, static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * normalizedY)};

        for (int i = 0; i < drawingArea.w; i++){
            currentX += xIncrement;
            currentY += YofX(currentX);

            normalizedY = (currentY - minSample)/(maxSample - minSample);
            point.x = drawingArea.x+i;
            point.y = static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * normalizedY);

            SDL_RenderDrawLine(renderer, lastPoint.x, lastPoint.y, point.x, point.y);
            lastPoint = point;
        }
    }
    else
    {
        point = {drawingArea.x, static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * currentY)};
        lastPoint = {drawingArea.x, static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * currentY)};

        for (int i = 0; i < drawingArea.w; i++){
            currentX += xIncrement;
            currentY = YofX(currentX);

            point.x = drawingArea.x+i;
            point.y = static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * currentY);

            SDL_RenderDrawLine(renderer, lastPoint.x, lastPoint.y, point.x, point.y);

            lastPoint = point;
        }
    }
}

double WaveformMenu::YofX(double x){

    double value;

    switch (editedOsc->waveType) {
        case WaveformType::SINE:
            value = sin(x);
            break;
        case WaveformType::SQUARE:
            value = sin(x) > 0 ? 1.0 : -1.0;
            break;
        case WaveformType::TRIANGLE:
            value = asin(sin(x)) * 2.0/PI;
            break;
        case WaveformType::SAWTOOTHDOWN:
            value = 1.0 - 2.0 * (fmod(x, TWOPI) * (1.0 / TWOPI));
            break;
        case WaveformType::SAWTOOTHUP:
            value = (2.0 * (fmod(x, TWOPI) * (1.0 / TWOPI))) - 1.0;
            break;
        case WaveformType::NOISE:
            value = 2.0 * ((double)rand() / (double)(RAND_MAX)) - 1.0;
            break;
    }

    return value;
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

void WaveformMenu::setLFO(){
    if (editedOsc->lfo == nullptr){
        editedOsc->setLFO(44100, WaveformType::SINE);
        editedOsc->lfo->setFrequency(1);
        editedOsc->lfo->setAmpMod(0.01);
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

