//
// Created by alberto on 11/20/21.
//

#include "../include/WaveformMenu.h"
#include "../include/MainWindow.h"
#include <string>
#include <iomanip>
#include <sstream>

void WaveformMenu::init(){
    textColor = {255,255,255};

    setTextLabel(&screenTitle, "instrument creator");
    setTextLabel(&upArrow, "^");
    setTextLabel(&downArrow, "v");
    setTextLabel(&oscCounter, "Oscillator " + std::to_string(currentOsc) + " of " + std::to_string(maxOsc));
    setTextLabel(&wavetypeLabel, "WAVE");
    setTextLabel(&wavetypeSign, "~");
    setTextLabel(&frequencyLabel, "FREQ");
    setTextLabel(&amplitudeLabel, "AMP");
    setTextLabel(&frequencyValue, doubleToStr(currentFrequency, 3));
    setTextLabel(&amplitudeValue, doubleToStr(currentAmplitude, 1));
    sineImage.loadFromFile(window->renderer, assets_dir + "sine.png");
    squareImage.loadFromFile(window->renderer, assets_dir + "square.png");
    triangleImage.loadFromFile(window->renderer, assets_dir + "triangle.png");
    sawdownImage.loadFromFile(window->renderer, assets_dir + "sawdown.png");
    sawupImage.loadFromFile(window->renderer, assets_dir + "sawup.png");
    noiseImage.loadFromFile(window->renderer, assets_dir + "noise.png");
    focusedSelectorIndex = 0;
    wavetypeSelector.loadControl(&sineImage, window->renderer);
    wavetypeSelector.isHighlighted = true;
    frequencySelector.loadControl(&frequencyValue, window->renderer);
    amplitudeSelector.loadControl(&amplitudeValue, window->renderer);
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

void WaveformMenu::highlightShit(){
    wavetypeSelector.switchHighlight();
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

void WaveformMenu::setFocusedControl(){
    controls[focusedSelectorIndex]->switchHighlight();
    focusedSelectorIndex++;
    if (focusedSelectorIndex == controls.size())
        focusedSelectorIndex = 0;
    controls[focusedSelectorIndex]->switchHighlight();
}

void WaveformMenu::setTextLabel(Texture* texture, std::string text){
    texture->loadFromText(window->renderer, text, textColor, window->mainFont);
}

WaveformMenu::WaveformMenu(MainWindow* mainWindow) {
    window = mainWindow;
}

std::string WaveformMenu::doubleToStr(double d, int precision){
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << d;
    return stream.str();
}
