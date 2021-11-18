//
// Created by alberto on 11/11/21.
//

#include "oscillator.h"


void Oscillator::setWaveformType(WaveformType type) {
        switch (type) {
        case WaveformType::SINE:
            getTick = &Oscillator::sineTick;
            break;
        case WaveformType::SQUARE:
            getTick = &Oscillator::squareTick;
            break;
        case WaveformType::TRIANGLE:
        case WaveformType::DOWNSAW:
        case WaveformType::UPSAW:
        case WaveformType::NOISE:
            break;
    }
}

void Oscillator::setFrequency(double frequency){
    currentFrequency = frequency;
    phaseIncrement = twoPiOverSampleRate * frequency;
}

Oscillator::Oscillator(int sampleRate, WaveformType waveformType) {
    setWaveformType(waveformType);
    twoPiOverSampleRate = TWOPI / sampleRate;
    currentFrequency = 0.0;
    currentPhase = 0.0;
    phaseIncrement = 0.0;
}

Oscillator::~Oscillator() {
    delete &getTick;
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

//double Oscillator::triangleTick() {
//    double value = 2.0 * (currentPhase * (1.0 / TWOPI)) - 1;
//    if (value < 0.0)
//        value = -value;
//    value = 2.0 * (value - 0.5);
//    return value;
//}

void Oscillator::shiftPhase(){
    currentPhase += phaseIncrement;
    if (currentPhase >= TWOPI){
        currentPhase -= TWOPI;
    }
    if (currentPhase < 0.0){
        currentPhase += TWOPI;
    }
}

double Oscillator::getSample(double amplitude) {
    double sample = amplitude * (this->*getTick)();
    return sample;
}
