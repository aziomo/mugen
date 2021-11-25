//
// Created by alberto on 11/22/21.
//

#ifndef MUGEN_INSTRUMENT_H
#define MUGEN_INSTRUMENT_H

#include <vector>
#include <string>
#include "Oscillator.h"

class Instrument {

    Instrument(int blockSize);
    ~Instrument();

    std::string name;

    int blockSize;

    float generateSample(double frequency);
    void fillSampleFrame(float *frame, double frequency);
    void addOscillator();
    void removeOscillator();
    void setOscAmpMod(int oscNumber, double modifier);
    void setOscFreqMod(int oscNumber, double modifier);
    void setOscWavetype(int oscNumber, WaveformType type);

public:
    std::vector<Oscillator*> oscillators;
};


#endif //MUGEN_INSTRUMENT_H
