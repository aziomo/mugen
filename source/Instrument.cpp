#include "../include/Instrument.h"


Instrument::Instrument(int blockSize) {
    oscillators.push_back(new Oscillator(44100));
    this->blockSize = blockSize;
}

Instrument::~Instrument() {
    for (int i = 0; i < oscillators.size(); i++)
        oscillators.pop_back();
}

float Instrument::generateSample(double frequency, double dTime) {
    float sample = 0;
    int oscCount = (int) oscillators.size();
    for (int i = 0; i < oscCount; i++){
        auto* osc = oscillators.at(i);
        osc->setFrequency(frequency);
        sample += (float) osc->getSample(dTime);
    }
    if (oscCount > 1)
        sample /= (float) oscCount;
    return sample;
}

void Instrument::addOscillator(){
    oscillators.push_back(new Oscillator(44100));
}

void Instrument::removeOscillator(){
    oscillators.pop_back();
}

void Instrument::addToMainBufferSegment(float *mainBuffer, int offset, double frequency, double timePoint) {
    double timestep = 1.0 / 44100.0;
    for (int i = 0; i < blockSize; i++) {
        mainBuffer[offset + i] += generateSample(frequency, timePoint);
        timePoint += timestep;
    }
}
