//
// Created by alberto on 11/20/21.
//

#include "WaveformMenu.h"
#include "MainWindow.h"
#include <string>

void WaveformMenu::init(){
    textColor = {255,255,255};
//    screenTitle.loadFromText(window->renderer, "Create a waveform", textColor, window->mainFont);
    setTextLabel(&screenTitle, "Create a waveform");
    setTextLabel(&upArrow, "v");
    setTextLabel(&downArrow, "v");
    setTextLabel(&oscCounter, "Oscillator " + std::to_string(currentOsc) + " of " + std::to_string(maxOsc));
    setTextLabel(&wavetypeLabel, "wave type");
    setTextLabel(&wavetypeSign, "~");
    setTextLabel(&frequencyLabel, "FREQ");
    setTextLabel(&amplitudeLabel, "AMP");
}

void WaveformMenu::render(){
    screenTitle.render((window->windowWidth - screenTitle.width) / 2, window->windowHeight / 6);
}

void WaveformMenu::setTextLabel(Texture* texture, std::string text){
    texture->loadFromText(window->renderer, text, textColor, window->mainFont);
}

WaveformMenu::WaveformMenu(MainWindow* mainWindow) {
    window = mainWindow;
}
