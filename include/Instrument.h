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

    float lastSample = 0;
    bool helpingBool = false;

    int envelopeMoment = 0;

    void addOscillator();
    void removeOscillator();

    float generateSample(double frequency, double dTime);

    //void addToBufferBlock(float *bufferBlock, double frequency, double timePoint);

    double getEnvelopeAmplifier(double globalTime, Note* note);

    float generateSample(Note* note, double dTime);

    void addToBufferBlock(float *mainBuffer, Note* note, double timePoint);
};


#endif //MUGEN_INSTRUMENT_H
