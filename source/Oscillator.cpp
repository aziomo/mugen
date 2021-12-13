#include "../include/Oscillator.h"


Oscillator::Oscillator(int sampleRate, WaveformType waveformType) {
    setWaveformType(waveformType);
    twoPiOverSampleRate = TWOPI / sampleRate;
    this->sampleRate = sampleRate;
    currentFrequency = 0.0;
    baseFrequency = 0.0;
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

void Oscillator::setFrequency(double frequency){
    baseFrequency = frequency;
    currentFrequency = frequency * freqModifier;
    phaseIncrement = TWOPI / sampleRate * currentFrequency;
}


void Oscillator::shiftPhase(){
    currentPhase += phaseIncrement;
    // currentPhase += someValue + dTime || someValue * dTime;
    if (currentPhase >= TWOPI){
        currentPhase -= TWOPI;
    }
    if (currentPhase < 0.0){
        currentPhase += TWOPI;
    }
}

double Oscillator::getPhase(double dTime)
{
    if (lfo != nullptr && lfo->waveType != WaveformType::SINE){
        return TWOPI * (currentFrequency + currentFrequency * lfo->newGetSample(dTime)) * dTime;
//        return lfo->getPhase(dTime);
    }
    return TWOPI * currentFrequency * dTime;
}

double Oscillator::sineTick()
{
    double value = sin(currentPhase); // = sin(TWOPI * currentFrequency / sampleRate * totalTicks);
    shiftPhase();                     // totalTicks++;
    return value;
}

double Oscillator::newGetSample(double dTime)
{
//    if (lfo != nullptr && lfo->waveType != WaveformType::SINE){
//        newSetFrequencyWithLFO(dTime);
//    } else {
//        setFrequency(baseFrequency);
//    }
    return ampModifier * (this->*newGetTick)(dTime);
}

double Oscillator::newSineTick(double dTime)
{
    if (lfo != nullptr){
        return sin(getPhase(dTime) + baseFrequency * lfo->newGetSample(dTime));
    }
    return sin(getPhase(dTime));
}

void Oscillator::newSetFrequencyWithLFO(double dTime){
    currentFrequency = baseFrequency + (baseFrequency * lfo->newGetSample(dTime));
}

void Oscillator::setFrequencyWithLFO(){
    phaseIncrement = twoPiOverSampleRate * (currentFrequency  + (currentFrequency * lfo->getSample()));
}

double Oscillator::squareTick()
{
    double value = currentPhase <= PI ? 1.0 : -1.0;
    shiftPhase();
    return value;
}

double Oscillator::newSquareTick(double dTime)
{
    if (lfo != nullptr){
        return sin(getPhase(dTime) + baseFrequency * lfo->newGetSample(dTime)) <= 0 ? 1.0 : -1.0;
    }
    return sin(getPhase(dTime)) <= 0 ? 1.0 : -1.0;
}

double Oscillator::triangleTick() {
    double value = 2.0 * (currentPhase * (1.0 / TWOPI)) - 1;
    if (value < 0.0)
        value = -value;
    value = 2.0 * (value - 0.5);
    shiftPhase();
    return value;
}

double Oscillator::newTriangleTick(double dTime) {

    double firstValue;
    if (lfo != nullptr){
        firstValue = getPhase(dTime) + baseFrequency * lfo->newGetSample(dTime);
    } else {
        firstValue = getPhase(dTime);
    }


    double value = 2.0 * (fmod(firstValue, TWOPI) * (1.0 / TWOPI)) - 1;
    if (value < 0.0)
        value = -value;
    value = 2.0 * (value - 0.5);
    return value;
}

double Oscillator::sawDownTick(){
    double value = 1.0 - 2.0 * (currentPhase * (1.0 / TWOPI));
    shiftPhase();
    return value;
}

double Oscillator::newSawDownTick(double dTime){
    return 1.0 - 2.0 * (fmod(getPhase(dTime), TWOPI) * (1.0 / TWOPI));
}

double Oscillator::sawUpTick(){
    double value = (2.0 * (currentPhase * (1.0 / TWOPI))) - 1.0;
    shiftPhase();
    return value;
}

double Oscillator::newSawUpTick(double dTime){
    return (2.0 * (fmod(getPhase(dTime), TWOPI) * (1.0 / TWOPI))) - 1.0;
}

double Oscillator::noiseTick(){
    double value = 2.0 * ((double)rand() / (double)(RAND_MAX)) - 1.0;
    shiftPhase();
    return value;
}

double Oscillator::newNoiseTick(double dTime){
    return 2.0 * ((double)rand() / (double)(RAND_MAX)) - 1.0;
}

void Oscillator::setLFO(int sampleRate, WaveformType waveformType){
    lfo = new Oscillator(sampleRate, waveformType);
}

void Oscillator::unsetLFO(){
    delete lfo;
    lfo = nullptr;
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
            getTick = &Oscillator::squareTick;
            newGetTick = &Oscillator::newSquareTick;
            break;
        case WaveformType::TRIANGLE:
            waveType = WaveformType::TRIANGLE;
            getTick = &Oscillator::triangleTick;
            newGetTick = &Oscillator::newTriangleTick;
            break;
        case WaveformType::SAWTOOTHDOWN:
            waveType = WaveformType::SAWTOOTHDOWN;
            getTick = &Oscillator::sawDownTick;
            newGetTick = &Oscillator::newSawDownTick;
            break;
        case WaveformType::SAWTOOTHUP:
            waveType = WaveformType::SAWTOOTHUP;
            getTick = &Oscillator::sawUpTick;
            newGetTick = &Oscillator::newSawUpTick;
            break;
        case WaveformType::NOISE:
            waveType = WaveformType::NOISE;
            getTick = &Oscillator::noiseTick;
            newGetTick = &Oscillator::newNoiseTick;
            break;

    }
}
