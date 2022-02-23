#include "../include/Instrument.h"


Instrument::Instrument(int blockSize) {
    oscillators.push_back(new Oscillator(44100));
    this->blockSize = blockSize;
    env.initialAmplitude = 0.9;
    env.sustainAmplitude = 0.9;
    env.attackDuration = 0.1;
    env.decayDuration = 0.1;
    env.releaseDuration = 0.1;
}

Instrument::~Instrument() {
    for (int i = 0; i < oscillators.size(); i++)
        oscillators.pop_back();
}

float Instrument::generateSample(Note* note, double dTime) {
    float sample = 0;
    int oscCount = (int) oscillators.size();

    for (int i = 0; i < oscCount; i++){
        auto* osc = oscillators.at(i);
        osc->setFrequency(note->frequency);
        sample += (float) (env.getAmplifier(dTime, note) * osc->getSample(dTime));
    }
//    if (oscCount > 1)
//        sample /= (float) oscCount;
    return sample / (float) oscCount;
}

void Instrument::addOscillator(){
    oscillators.push_back(new Oscillator(44100));
}

void Instrument::removeOscillator(){
    oscillators.pop_back();
}

void Instrument::addToBufferBlock(float *block, Note* note, double timePoint) {
    double timestep = 1.0 / 44100.0;
    for (int i = 0; i < blockSize; i++) {
        block[i] += 0.2 * generateSample(note, timePoint);
        timePoint += timestep;
    }
}