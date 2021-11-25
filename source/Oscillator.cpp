//
// Created by alberto on 11/11/21.
//

#include "../include/Oscillator.h"


void Oscillator::setWaveformType(WaveformType type) {
        switch (type) {
        case WaveformType::SINE:
            waveType = WaveformType::SINE;
            getTick = &Oscillator::sineTick; break;
        case WaveformType::SQUARE:
            waveType = WaveformType::SQUARE;
            getTick = &Oscillator::squareTick; break;
        case WaveformType::TRIANGLE:
            waveType = WaveformType::TRIANGLE;
            getTick = &Oscillator::triangleTick; break;
        case WaveformType::SAWTOOTH:
            waveType = WaveformType::SAWTOOTH;
            getTick = &Oscillator::sawtoothTick; break;
        case WaveformType::NOISE:
            waveType = WaveformType::NOISE;
            getTick = &Oscillator::noiseTick; break;
    }
}

void Oscillator::setFrequency(double frequency){
    currentFrequency = frequency * freqModifier;
    phaseIncrement = twoPiOverSampleRate * currentFrequency;
}

Oscillator::Oscillator(int sampleRate, WaveformType waveformType) {
    setWaveformType(waveformType);
    twoPiOverSampleRate = TWOPI / sampleRate;
    currentFrequency = 0.0;
    currentPhase = 0.0;
    phaseIncrement = 0.0;
    freqModifier = 1.0;
    ampModifier = 1.0;
}

Oscillator::~Oscillator() {
    getTick = nullptr;
}

double Oscillator::sineTick()
{
    double value = sin(currentPhase);
    shiftPhase();
    return value;
}

double Oscillator::squareTick()
{
    double value = currentPhase <= PI ? 1.0 : -1.0;
    shiftPhase();
    return value;
}

double Oscillator::triangleTick() {
    double value = 2.0 * (currentPhase * (1.0 / TWOPI)) - 1;
    if (value < 0.0)
        value = -value;
    value = 2.0 * (value - 0.5);
    shiftPhase();
    return value;
}

double Oscillator::sawtoothTick(){
    double value = 1.0 - 2.0 * (currentPhase * (1.0 / TWOPI));
    shiftPhase();
    return value;
}

double Oscillator::noiseTick(){
    double value = 2.0 * ((double)rand() / (double)(RAND_MAX)) - 1.0;
    shiftPhase();
    return value;
}

void Oscillator::shiftPhase(){
    currentPhase += phaseIncrement;
    if (currentPhase >= TWOPI){
        currentPhase -= TWOPI;
    }
    if (currentPhase < 0.0){
        currentPhase += TWOPI;
    }
}

double Oscillator::getSample() {
    double sample = ampModifier * (this->*getTick)();
    return sample;
}

void Oscillator::setAmpMod(double modifier){
    ampModifier = modifier;
}

void Oscillator::setFreqMod(double modifier){
    freqModifier = modifier;
}
