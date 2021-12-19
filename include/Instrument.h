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
    std::vector<Oscillator*> oscillators;

    void addOscillator();
    void removeOscillator();

    float generateSample(double frequency, double dTime);

    void addToMainBufferSegment(float *mainBuffer, int offset, double frequency, double timePoint);
};


#endif //MUGEN_INSTRUMENT_H
