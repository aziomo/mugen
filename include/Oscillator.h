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
    double sampleRate;
    double currentPhase;
    double phaseIncrement;
    double(Oscillator::*getTick)();
    double(Oscillator::*newGetTick)(double);
public:
    double currentFrequency;
    double baseFrequency;
    Oscillator* lfo;
    double freqModifier = 1.0;
    double ampModifier = 1.0;

    WaveformType waveType;

    Oscillator(int sampleRate, WaveformType waveformType = WaveformType::SINE);
    ~Oscillator();
    void setWaveformType(WaveformType type);
    void setFrequency(double frequency);


    double sineTick();
    double squareTick();

    double getSample();

    void shiftPhase();

    void setAmpMod(double modifier);

    void setFreqMod(double modifier);

    double noiseTick();

    double triangleTick();

    double sawDownTick();

    void setLFO(int sampleRate, WaveformType waveformType);

    void unsetLFO();

    void setFrequencyWithLFO();

    double sawUpTick();

    double newSineTick(double dTime);

    double newGetSample(double dTime);

    double newSquareTick(double dTime);

    double getPhase(double dTime);

    double newTriangleTick(double dTime);

    double newNoiseTick();

    double newSawDownTick(double dTime);

    double newSawUpTick(double dTime);

    double newNoiseTick(double dTime);

    void newSetFrequencyWithLFO(double dTime);
};


#endif //MUGEN_CPP_OSCILLATOR_H
