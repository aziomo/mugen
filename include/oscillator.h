//
// Created by alberto on 11/11/21.
//

#ifndef MUGEN_CPP_OSCILLATOR_H
#define MUGEN_CPP_OSCILLATOR_H


#include "WaveformType.h"
#include <cmath>

const double PI = 2.0 * acos(0.0);
const double TWOPI = 2.0 * PI;

class Oscillator {

private:
    double twoPiOverSampleRate;
    double currentFrequency;
    double currentPhase;
    double phaseIncrement;
    double(Oscillator::*getTick)();

public:

    Oscillator(int sampleRate, WaveformType waveformType = WaveformType::SINE);
    ~Oscillator();
    void setWaveformType(WaveformType type);
    void setFrequency(double frequency);


    double sineTick();
    double squareTick();

    double getSample(double amplitude);

    void shiftPhase();
};


#endif //MUGEN_CPP_OSCILLATOR_H
