//
// Created by alberto on 11/20/21.
//

#include "../include/WaveformMenu.h"
#include <string>
#include <iomanip>
#include <sstream>

void WaveformMenu::init(){
    textColor = {255,255,255};
    loadTextures();
    focusedSelectorIndex = 0;
    std::vector<Texture*> waveImages {&sineImage, &squareImage, &triangleImage,
                                      &sawupImage, &sawdownImage, &noiseImage};
    wavetypeSelector.loadControl(waveImages, window);
    frequencySelector.loadControl(currentFreqMod, &frequencyValue, window);
    amplitudeSelector.loadControl(currentAmplitude, &amplitudeValue, window);
    wavetypeSelector.isHighlighted = true;
}

void WaveformMenu::loadTextures(){
    setTextTexture(&screenTitle, "instrument creator");
    setTextTexture(&oscCounter, "Oscillator " + std::to_string(currentOsc) + " of " + std::to_string(maxOsc));
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
    setImageTexture(&sawdownImage, assets_dir + "sawdown.png");
    setImageTexture(&sawupImage, assets_dir + "sawup.png");
    setImageTexture(&noiseImage, assets_dir + "noise.png");
}

void WaveformMenu::render(){
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

WaveformMenu::WaveformMenu(MainWindow* mainWindow) {
    window = mainWindow;
    renderer = mainWindow->renderer;
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
