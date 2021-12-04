//
// Created by alberto on 11/22/21.
//

#ifndef MUGEN_INSTRUMENT_H
#define MUGEN_INSTRUMENT_H

#include <vector>
#include <string>
#include "Oscillator.h"

class Instrument {
public:
    Instrument(int blockSize);
    ~Instrument();
    std::string name;
    int blockSize;
    float generateSample(double frequency);
    void fillSampleBlock(float *frame, double frequency);
    void fillMainBufferSegment(float* mainBuffer, int offset, double frequency);
    void addOscillator();
    void removeOscillator();
    std::vector<Oscillator*> oscillators;

    void addToMainBufferSegment(float *mainBuffer, int offset, double frequency);
};


#endif //MUGEN_INSTRUMENT_H
