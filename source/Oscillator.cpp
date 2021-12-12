#include "../include/Oscillator.h"


Oscillator::Oscillator(int sampleRate, WaveformType waveformType) {
    setWaveformType(waveformType);
    twoPiOverSampleRate = TWOPI / sampleRate;
    this->sampleRate = sampleRate;
    currentFrequency = 0.0;
    currentPhase = 0.0;
    phaseIncrement = 0.0;
    freqModifier = 1.0;
    ampModifier = 1.0;
    lfo = nullptr;
}

double Oscillator::getSample() {
    if (lfo != nullptr){
        setFrequencyWithLFO();
    }
    double sample = ampModifier * (this->*getTick)();
    return sample;
}

double Oscillator::newGetSample(double dTime) {
    if (lfo != nullptr){
        setFrequencyWithLFO();
    }
    double sample = ampModifier * (this->*newGetTick)(dTime);
    return sample;
}


void Oscillator::setFrequency(double frequency){
    currentFrequency = frequency * freqModifier;
    phaseIncrement = TWOPI / sampleRate * currentFrequency;
}

void Oscillator::shiftPhase(){
    currentPhase += phaseIncrement;
    // currentPhase += someValue + dTime || someValue * dTime;
    /*if (currentPhase >= TWOPI){
        currentPhase -= TWOPI;
    }
    if (currentPhase < 0.0){
        currentPhase += TWOPI;
    }*/
}

double Oscillator::newSineTick(double dTime){
    double value = sin(TWOPI * currentFrequency * dTime);
    return value;
}

double Oscillator::sineTick()
{
    double value = sin(currentPhase);
    // double value = sin(someValue + dTime);
    shiftPhase();
    return value;
}

double Oscillator::squareTick()
{
    double value = currentPhase <= PI ? 1.0 : -1.0;
    shiftPhase();
    return value;
}

void Oscillator::setFrequencyWithLFO(){
    phaseIncrement = TWOPI / sampleRate * (currentFrequency  + (currentFrequency * lfo->getSample()));
}

void Oscillator::setLFO(int sampleRate, WaveformType waveformType){
    lfo = new Oscillator(sampleRate, waveformType);
}

void Oscillator::unsetLFO(){
    delete lfo;
    lfo = nullptr;
}

double Oscillator::triangleTick() {
    double value = 2.0 * (currentPhase * (1.0 / TWOPI)) - 1;
    if (value < 0.0)
        value = -value;
    value = 2.0 * (value - 0.5);
    shiftPhase();
    return value;
}

double Oscillator::sawtoothTick(){
    double value = 1.0 - 2.0 * (currentPhase * (1.0 / TWOPI));
    shiftPhase();
    return value;
}

double Oscillator::sawtoothUpTick(){
    double value = (2.0 * (currentPhase * (1.0 / TWOPI))) - 1.0;
    shiftPhase();
    return value;
}

double Oscillator::noiseTick(){
    double value = 2.0 * ((double)rand() / (double)(RAND_MAX)) - 1.0;
    shiftPhase();
    return value;
}

void Oscillator::setAmpMod(double modifier){
    ampModifier = modifier;
}

void Oscillator::setFreqMod(double modifier){
    freqModifier = modifier;
}

Oscillator::~Oscillator() {
    getTick = nullptr;
}

void Oscillator::setWaveformType(WaveformType type) {
    switch (type) {
        case WaveformType::SINE:
            waveType = WaveformType::SINE;
            getTick = &Oscillator::sineTick;
            newGetTick = &Oscillator::newSineTick;
                    break;
        case WaveformType::SQUARE:
            waveType = WaveformType::SQUARE;
            getTick = &Oscillator::squareTick; break;
        case WaveformType::TRIANGLE:
            waveType = WaveformType::TRIANGLE;
            getTick = &Oscillator::triangleTick; break;
        case WaveformType::SAWTOOTHDOWN:
            waveType = WaveformType::SAWTOOTHDOWN;
            getTick = &Oscillator::sawtoothTick; break;
        case WaveformType::SAWTOOTHUP:
            waveType = WaveformType::SAWTOOTHUP;
            getTick = &Oscillator::sawtoothUpTick; break;
        case WaveformType::NOISE:
            waveType = WaveformType::NOISE;
            getTick = &Oscillator::noiseTick; break;

    }
}
