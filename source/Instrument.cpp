#include "../include/Instrument.h"


Instrument::Instrument(int blockSize) {
    oscillators.push_back(new Oscillator(44100));
    this->blockSize = blockSize;
}

Instrument::~Instrument() {
    for (int i = 0; i < oscillators.size(); i++)
        oscillators.pop_back();
}

float Instrument::generateSample(double frequency) {
    float sample = 0;
    int oscCount = (int) oscillators.size();
    for (int i = 0; i < oscCount; i++){
        auto* osc = oscillators.at(i);
        osc->setFrequency(frequency);
        sample += (float) osc->getSample();
    }
    if (oscCount > 1)
        sample /= (float) oscCount;
    return sample;
}

void Instrument::fillSampleBlock(float* frame, double frequency){
    double volume = 1.0;
    for (int i = 0; i < blockSize; i++) {
        frame[i] = generateSample(frequency) * volume;
    }
}

void Instrument::addOscillator(){
    oscillators.push_back(new Oscillator(44100));
}

void Instrument::removeOscillator(){
    oscillators.pop_back();
}

void Instrument::fillMainBufferSegment(float *mainBuffer, int offset, double frequency) {
    for (int i = 0; i < blockSize; i++) {
        mainBuffer[offset + i] = generateSample(frequency);
    }
}

void Instrument::addToMainBufferSegment(float *mainBuffer, int offset, double frequency) {
    for (int i = 0; i < blockSize; i++) {
        mainBuffer[offset + i] += generateSample(frequency);
    }
}

void Instrument::testAddTwoNotesToMainBufferSegment(float *mainBuffer, int offset, double frequency) {
    for (int i = 0; i < blockSize; i++) {
        mainBuffer[offset + i] += generateSample(frequency);
        mainBuffer[offset + i] += generateSample(frequency);
    }
//    for (int i = 0; i < blockSize; i++)
//        mainBuffer[offset + i] += generateSample(frequency);

}
