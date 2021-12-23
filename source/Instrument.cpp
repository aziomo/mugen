#include "../include/Instrument.h"


Instrument::Instrument(int blockSize) {
    oscillators.push_back(new Oscillator(44100));
    this->blockSize = blockSize;
    env.attackTimespan = 0.3;
    env.attackValue = 0.9;
    env.decayTimespan = 1.0;
    env.sustainValue = 0.3;
    env.releaseTimespan = 2.0;
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


//        sample += (float) (getEnvelopeAmplifier(dTime, note) * osc->getSample(dTime));
        float newSample = (float) (getEnvelopeAmplifier(dTime, note) * osc->getSample(dTime));

        if (lastSample < 0.0001 && newSample > 0.2){
            auto fuckyou = 1;
            helpingBool = true;
        }
        if (helpingBool){
            auto fuckyoutoo = 0;
            helpingBool = false;
        }

        sample += newSample;
        lastSample = newSample;

    }
    if (oscCount > 1)
        sample /= (float) oscCount;
    return sample;
}

double Instrument::getEnvelopeAmplifier(double globalTime, Note* note){

    if (note->isPressed){

        if (globalTime >= note->pressedOnTime && globalTime < note->pressedOnTime + env.attackTimespan){
            // return some value from attack phase
            note->isPlaying = true;
            envelopeMoment = 1;
            return ((globalTime - note->pressedOnTime) / env.attackTimespan)  * env.attackValue;


        } else if (globalTime < note->pressedOnTime + env.attackTimespan + env.decayTimespan){
            // return some value from decay phase
            envelopeMoment = 2;
            return env.attackValue - (globalTime - note->pressedOnTime - env.attackTimespan) / env.decayTimespan
                                     * (env.attackValue - env.sustainValue);


        } else { // in other words, if note is still pressed
            envelopeMoment = 3;
            return env.sustainValue;
        }
    } else if (globalTime < note->releasedOnTime + env.releaseTimespan){

        if (note->releasedOnTime - note->pressedOnTime < env.attackTimespan) {
            envelopeMoment = 4;
            double ampAtRelease = ((note->releasedOnTime - note->pressedOnTime) / env.attackTimespan) * env.attackValue;
            return ampAtRelease - ampAtRelease * ((globalTime - note->releasedOnTime) / env.releaseTimespan);
        }

        else if (note->releasedOnTime - note->pressedOnTime < env.attackTimespan + env.decayTimespan) {
            envelopeMoment = 5;
            double ampAtRelease = env.attackValue - (note->releasedOnTime - note->pressedOnTime - env.attackTimespan) / env.decayTimespan
                                                    * (env.attackValue - env.sustainValue);
            return ampAtRelease - ampAtRelease * ((globalTime - note->releasedOnTime) / env.releaseTimespan);
        }

        else{
            envelopeMoment = 6;
            return env.sustainValue - env.sustainValue * ((globalTime - note->releasedOnTime) / env.releaseTimespan);
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
        float newSample = 0.2 * generateSample(note, timePoint);


//        mainBuffer[i] += 0.2 * generateSample(note, timePoint);
        mainBuffer[i] += newSample;
        timePoint += timestep;
        lastSample = newSample;
    }
}