//
// Created by alberto on 11/22/21.
//

#ifndef MUGEN_INSTRUMENT_H
#define MUGEN_INSTRUMENT_H

#include <vector>
#include <string>
#include "Oscillator.h"
#include "Envelope.h"

class Instrument {
public:
    Instrument(int blockSize);
    ~Instrument();
    std::string name;
    int blockSize;
    std::vector<Oscillator*> oscillators;
    Envelope env;

    int envelopeMoment = 0;

    void addOscillator();
    void removeOscillator();

    float generateSample(Note* note, double dTime);

    void addToBufferBlock(float *mainBuffer, Note* note, double timePoint);
};


#endif //MUGEN_INSTRUMENT_H
