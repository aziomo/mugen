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
    for (auto control : controls) {
        control = nullptr;
    }
    musicBox = nullptr;
    window = nullptr;
    editedInstrument = nullptr;
    editedOsc = nullptr;
}

void WaveformMenu::init(){
    textColor = {255,255,255};
    editedOsc = musicBox->oscillators.front();
    loadTextures();
    focusedSelectorIndex = 0;
    std::vector<Texture*> waveImages {&sineImage, &squareImage, &triangleImage,
                                      &sawtoothImage, &noiseImage};
    wavetypeSelector.loadImageControl(editedOsc, waveImages, window);
    frequencySelector.loadTextControl(&editedOsc->freqModifier, &frequencyValue, window);
    amplitudeSelector.loadTextControl(&editedOsc->ampModifier, &amplitudeValue, window);
    wavetypeSelector.isHighlighted = true;
//    editedInstrument = musicBox->instruments.front();
//    editedOsc = editedInstrument->oscillators.front();

}

void WaveformMenu::loadTextures(){
    setTextTexture(&screenTitle, "instrument creator");
    setTextTexture(&oscCounter, "Oscillator " + std::to_string(currentOsc) + " of " + std::to_string(musicBox->instruments.size()));
    setTextTexture(&wavetypeLabel, "WAVE");
    setTextTexture(&wavetypeSign, "~");
    setTextTexture(&frequencyLabel, "FREQ");
    setTextTexture(&amplitudeLabel, "AMP");
    setTextTexture(&frequencyValue, doubleToStr(currentFreqMod, 2));
    setTextTexture(&amplitudeValue, doubleToStr(currentAmplitude, 2));
    setImageTexture(&arrow, assets_dir + "arrow.png");
    setImageTexture(&sineImage, assets_dir + "sine.png");
    setImageTexture(&squareImage, assets_dir + "square.png");
    setImageTexture(&triangleImage, assets_dir + "triangle.png");
    setImageTexture(&sawtoothImage, assets_dir + "sawtooth.png");
    setImageTexture(&noiseImage, assets_dir + "noise.png");
}

void WaveformMenu::updateTextures(){
    setTextTexture(&oscCounter, "Oscillator " + std::to_string(currentOsc) + " of " + std::to_string(musicBox->instruments.size()));
}


void WaveformMenu::render(){
    updateTextures();
    screenTitle.render((window->windowWidth - screenTitle.width) /2, window->windowHeight /10);
    oscCounter.render((window->windowWidth - oscCounter.width) /2, window->windowHeight /4);
    wavetypeLabel.render((window->windowWidth - wavetypeLabel.width) /4, window->windowHeight *2/5);
    frequencyLabel.render((window->windowWidth - frequencyLabel.width) /2, window->windowHeight *2/5);
    amplitudeLabel.render((window->windowWidth - amplitudeLabel.width) *3/4, window->windowHeight *2/5);
    wavetypeSelector.render((window->windowWidth - wavetypeSign.width) /4, window->windowHeight /2);
    frequencySelector.render((window->windowWidth - frequencyValue.width) /2, window->windowHeight /2);
    amplitudeSelector.render((window->windowWidth - amplitudeValue.width) *3/4, window->windowHeight /2);
}

void WaveformMenu::focusNextControl(){
    controls[focusedSelectorIndex]->switchHighlight();
    focusedSelectorIndex++;
    if (focusedSelectorIndex == controls.size())
        focusedSelectorIndex = 0;
    controls[focusedSelectorIndex]->switchHighlight();
}

void WaveformMenu::focusPrevControl(){
    controls[focusedSelectorIndex]->switchHighlight();
    focusedSelectorIndex--;
    if (focusedSelectorIndex < 0)
        focusedSelectorIndex = controls.size() - 1;
    controls[focusedSelectorIndex]->switchHighlight();
}

void WaveformMenu::setTextTexture(Texture* texture, std::string text){
    texture->loadFromText(renderer, text, textColor, window->mainFont);
}

void WaveformMenu::setImageTexture(Texture* texture, std::string imagePath){
    texture->loadFromFile(renderer, imagePath);
}

std::string WaveformMenu::doubleToStr(double d, int precision){
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << d;
    return stream.str();
}

void WaveformMenu::chooseFocusedControl() {
    controls[focusedSelectorIndex]->switchHighlight();
    controls[focusedSelectorIndex]->switchEditing();
}

void WaveformMenu::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
            if (controls[focusedSelectorIndex]->isHighlighted){

            }

            break;
        case SDLK_DOWN:

            break;
        case SDLK_LEFT:
            if (controls[focusedSelectorIndex]->isHighlighted){
                focusPrevControl();
            }
            if (controls[focusedSelectorIndex]->isEditing){
                controls[focusedSelectorIndex]->decrement();
            }
             break;
        case SDLK_RIGHT:
            if (controls[focusedSelectorIndex]->isHighlighted) {
                focusNextControl();
            }
            if (controls[focusedSelectorIndex]->isEditing){
                controls[focusedSelectorIndex]->increment();
            }
            break;
        case SDLK_RETURN:
            chooseFocusedControl(); break;
        default:
            break;

    }
}

WaveformType WaveformMenu::nextWaveType(WaveformType type, bool increment){
    switch (type) {
        case WaveformType::SINE:
            return increment ? WaveformType::SQUARE : WaveformType::NOISE;
        case WaveformType::SQUARE:
            return increment ? WaveformType::TRIANGLE : WaveformType::SINE;
        case WaveformType::TRIANGLE:
            return increment ? WaveformType::SAWTOOTH : WaveformType::SQUARE;
        case WaveformType::SAWTOOTH:
            return increment ? WaveformType::NOISE : WaveformType::TRIANGLE;
        case WaveformType::NOISE:
            return increment ? WaveformType::SINE : WaveformType::SAWTOOTH;
    }
}
