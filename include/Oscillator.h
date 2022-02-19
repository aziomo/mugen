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
    int sampleRate;

    double (Oscillator::*getTick)(double);

    static constexpr int lookupTableSize = 512;
    double lfoLookupTable[lookupTableSize];
public:
    double frequency;
    Oscillator *lfo;
    double freqModifier = 1.0;
    double ampModifier = 1.0;
    WaveformType waveType;

    Oscillator(int sampleRate, WaveformType waveformType = WaveformType::SINE);
    ~Oscillator();

    void setWaveformType(WaveformType type);
    void setFrequency(double frequency);
    void setLFO(WaveformType waveformType);
    void unsetLFO();

    double getPhase(double dTime);

    static double getPhase(double dTime, double frequency);

    double getSample(double dTime);

    double sineTick(double dTime);
    double squareTick(double dTime);
    double triangleTick(double dTime);
    double sawDownTick(double dTime);
    double sawUpTick(double dTime);
    double noiseTick(double dTime);

    static double YofX(double x, WaveformType function);

    void setupLfoLookup(WaveformType type);

    static int phaseToIndex(double phase);

    static double indexToPhase(int index);

    double getLfoInterpSample(double dTime);

};


#endif //MUGEN_CPP_OSCILLATOR_H
