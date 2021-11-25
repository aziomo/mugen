#include "../include/Instrument.h"


Instrument::Instrument(int blockSize) {
    Oscillator* osc = new Oscillator(44100);
    oscillators.push_back(osc);
    this->blockSize = blockSize;
}

Instrument::~Instrument() {
    for (int i = 0; i < oscillators.size(); i++)
        oscillators.pop_back();
}

float Instrument::generateSample(double frequency) {
    float sample = 0;
    int oscCount = oscillators.size();
    for (int i = 0; i < oscCount; i++){
        auto* osc = oscillators.at(i);
        osc->setFrequency(frequency);
        osc->setFreqMod(1.0 + i * 0.5);
        sample += (float) osc->getSample();
    }
    if (oscCount > 1)
        sample /= oscCount;
    return sample;
}

void Instrument::fillSampleFrame(float* frame, double frequency){
    for (int i = 0; i < blockSize; i++) {
        frame[i] = generateSample(frequency);
    }
}

void Instrument::addOscillator(){
    Oscillator* osc = new Oscillator(44100);
    oscillators.push_back(osc);
}

void Instrument::removeOscillator(){
    oscillators.pop_back();
}

void Instrument::setOscFreqMod(int oscNumber, double modifier){
    oscillators.at(oscNumber)->setFreqMod(modifier);
}

void Instrument::setOscAmpMod(int oscNumber, double modifier){
    oscillators.at(oscNumber)->setAmpMod(modifier);
}

void Instrument::setOscWavetype(int oscNumber, WaveformType type){
    oscillators.at(oscNumber)->setWaveformType(type);
}