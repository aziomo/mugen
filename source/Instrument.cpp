#include "../include/Instrument.h"


Instrument::Instrument(int blockSize) {
    oscillators.push_back(new Oscillator(44100));
    this->blockSize = blockSize;
    env.attackDuration = 0.3;
    env.startingAmplitude = 0.9;
    env.decayDuration = 1.0;
    env.sustainAmplitude = 0.3;
    env.releaseDuration = 2.0;
}

Instrument::~Instrument() {
    for (int i = 0; i < oscillators.size(); i++)
        oscillators.pop_back();
}

float Instrument::generateSample(double frequency, double dTime) {
    float sample = 0;
    int oscCount = (int) oscillators.size();

    for (int i = 0; i < oscCount; i++){
        auto* osc = oscillators.at(i);
        osc->setFrequency(frequency);

        sample += (float) osc->getSample(dTime);
    }
    if (oscCount > 1)
        sample /= (float) oscCount;
    return sample;
}

float Instrument::generateSample(Note* note, double dTime) {
    float sample = 0;
    int oscCount = (int) oscillators.size();

    for (int i = 0; i < oscCount; i++){
        auto* osc = oscillators.at(i);
        osc->setFrequency(note->frequency);
        sample += (float) (getEnvelopeAmplifier(dTime, note) * osc->getSample(dTime));
    }
    if (oscCount > 1)
        sample /= (float) oscCount;
    return sample;
}

double Instrument::getEnvelopeAmplifier(double globalTime, Note* note){

    if (note->isPressed){

        if (globalTime >= note->pressedOnTime && globalTime < note->pressedOnTime + env.attackDuration){
            // return some value from attack phase
            note->isPlaying = true;
            envelopeMoment = 1;
            return ((globalTime - note->pressedOnTime) / env.attackDuration) * env.startingAmplitude;


        } else if (globalTime < note->pressedOnTime + env.attackDuration + env.decayDuration){
            // return some value from decay phase
            envelopeMoment = 2;
            return env.startingAmplitude - (globalTime - note->pressedOnTime - env.attackDuration) / env.decayDuration
                                           * (env.startingAmplitude - env.sustainAmplitude);


        } else { // in other words, if note is still pressed
            envelopeMoment = 3;
            return env.sustainAmplitude;
        }
    } else if (globalTime < note->releasedOnTime + env.releaseDuration){

        if (note->releasedOnTime - note->pressedOnTime < env.attackDuration) {
            envelopeMoment = 4;
            double ampAtRelease = ((note->releasedOnTime - note->pressedOnTime) / env.attackDuration) * env.startingAmplitude;
            return ampAtRelease - ampAtRelease * ((globalTime - note->releasedOnTime) / env.releaseDuration);
        }

        else if (note->releasedOnTime - note->pressedOnTime < env.attackDuration + env.decayDuration) {
            envelopeMoment = 5;
            double ampAtRelease = env.startingAmplitude - (note->releasedOnTime - note->pressedOnTime - env.attackDuration) / env.decayDuration
                                                          * (env.startingAmplitude - env.sustainAmplitude);
            return ampAtRelease - ampAtRelease * ((globalTime - note->releasedOnTime) / env.releaseDuration);
        }

        else{
            envelopeMoment = 6;
            return env.sustainAmplitude - env.sustainAmplitude * ((globalTime - note->releasedOnTime) / env.releaseDuration);
        }

    } else {
        envelopeMoment = 0;
        note->isPlaying = false;
        return 0.0;
    }
}

void Instrument::addOscillator(){
    oscillators.push_back(new Oscillator(44100));
}

void Instrument::removeOscillator(){
    oscillators.pop_back();
}

//void Instrument::addToBufferBlock(float *bufferBlock, double frequency, double timePoint) {
//    double timestep = 1.0 / 44100.0;
//    for (int i = 0; i < blockSize; i++) {
//        bufferBlock[i] += 0.2 * generateSample(frequency, timePoint);
//        timePoint += timestep;
//    }
//}

void Instrument::addToBufferBlock(float *mainBuffer, Note* note, double timePoint) {
    double timestep = 1.0 / 44100.0;
    for (int i = 0; i < blockSize; i++) {
        mainBuffer[i] += 0.2 * generateSample(note, timePoint);
        timePoint += timestep;
    }
}