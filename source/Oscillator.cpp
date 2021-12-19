#include "../include/Oscillator.h"


Oscillator::Oscillator(int sampleRate, WaveformType waveformType) {
    setWaveformType(waveformType);
    this->sampleRate = sampleRate;
    currentFrequency = 0.0;
    freqModifier = 1.0;
    ampModifier = 1.0;
    lfo = nullptr;
}

void Oscillator::setFrequency(double frequency){
    currentFrequency = frequency * freqModifier;
}


void Oscillator::setupLfoLookup(WaveformType type){
    for (double & i : lfoLookupTable){
        i = 0;
    }

    double xIncrement = TWOPI / (double) lookupTableSize;
    double startingX = 0.0;
    double currentX = startingX;
    double currentY = YofX(currentX,type);

    double maxSample = 0.0;
    double minSample = std::abs(currentY);

    for (int i = 0; i < lookupTableSize; i++){
        currentX += xIncrement;
        currentY += YofX(currentX, type);

        if (maxSample < std::abs(currentY)){
            maxSample = std::abs(currentY);
        }
        if (minSample > std::abs(currentY)){
            minSample = std::abs(currentY);
        }
    }

    currentX = startingX;
    currentY = YofX(currentX, type);

    double normalizedY = (currentY - minSample)/(maxSample - minSample);

    for (int i = 0; i < lookupTableSize; i++){
        lfoLookupTable[i] = normalizedY;
        currentX += xIncrement;
        currentY += YofX(currentX, type);
        normalizedY = (currentY - minSample)/(maxSample - minSample);
    }
}

int Oscillator::phaseToIndex(double phase){
    return (int)(phase / TWOPI * lookupTableSize);
}

double Oscillator::indexToPhase(int index){
    return (double)index / lookupTableSize * TWOPI;
}

double Oscillator::YofX(double x, WaveformType function)
{
    double value;
    switch (function) {
        case WaveformType::SINE:
            value = sin(x);
            break;
        case WaveformType::SQUARE:
            value = sin(x) > 0 ? 1.0 : -1.0;
            break;
        case WaveformType::TRIANGLE:
            value = asin(sin(x)) * 2.0/PI;
            break;
        case WaveformType::SAWTOOTHDOWN:
            value = 1.0 - 2.0 * (fmod(x, TWOPI) * (1.0 / TWOPI));
            break;
        case WaveformType::SAWTOOTHUP:
            value = (2.0 * (fmod(x, TWOPI) * (1.0 / TWOPI))) - 1.0;
            break;
        case WaveformType::NOISE:
            value = 2.0 * ((double)rand() / (double)(RAND_MAX)) - 1.0;
            break;
    }
    return value;
}

double Oscillator::getLfoTruncatedSample(double dTime){
    double Pc = fmod(getPhase(dTime, lfo->currentFrequency), TWOPI);
    return lfoLookupTable[phaseToIndex(Pc)];
}

double Oscillator::getLfoInterpolatedSample(double dTime){

    double Pc = fmod(getPhase(dTime, lfo->currentFrequency), TWOPI);
    int P1index = phaseToIndex(Pc); // rounds down
    int P2index = (P1index == lookupTableSize - 1) ?
            0 : P1index + 1;

    if (Pc == lfoLookupTable[P1index]){
        return lfoLookupTable[P1index];
    }

    double P1 = indexToPhase(P1index);
    double P2 = indexToPhase(P2index);

    double VP1 = lfoLookupTable[P1index];
    double VP2 = lfoLookupTable[P2index];
    double V = std::abs(VP2 - VP1);

    double Si = std::abs(P2 - Pc);
    double S = std::abs(P2 - P1);
    double Rs = Si / S;

    return VP1 + V * Rs;
}


double Oscillator::getSample(double dTime)
{
    return ampModifier * (this->*getTick)(dTime);
}

double Oscillator::getPhase(double dTime)
{
    if (lfo != nullptr){
        return TWOPI * currentFrequency * dTime
        + lfo->ampModifier * 10.0 / lfo->currentFrequency * getLfoTruncatedSample(dTime);
    }
    return TWOPI * currentFrequency * dTime;
}

double Oscillator::getPhase(double dTime, double frequency){
    return TWOPI * frequency * dTime;
}

double Oscillator::sineTick(double dTime)
{
    return sin(getPhase(dTime));
}

double Oscillator::squareTick(double dTime)
{
    return sin(getPhase(dTime)) <= 0 ? 1.0 : -1.0;
}

double Oscillator::triangleTick(double dTime)
{
    double value = 2.0 * (fmod(getPhase(dTime), TWOPI) * (1.0 / TWOPI)) - 1;

    if (value < 0.0)
        value = -value;
    value = 2.0 * (value - 0.5);
    return value;
}

double Oscillator::sawDownTick(double dTime){
    return 1.0 - 2.0 * (fmod(getPhase(dTime), TWOPI) * (1.0 / TWOPI));
}

double Oscillator::sawUpTick(double dTime){
    return (2.0 * (fmod(getPhase(dTime), TWOPI) * (1.0 / TWOPI))) - 1.0;
}

double Oscillator::noiseTick(double dTime){
    return 2.0 * ((double)rand() / (double)(RAND_MAX)) - 1.0;
}

void Oscillator::setLFO(int sampleRate, WaveformType waveformType){
    lfo = new Oscillator(sampleRate, waveformType);
    setupLfoLookup(waveformType);
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
            break;
        case WaveformType::SQUARE:
            waveType = WaveformType::SQUARE;
            getTick = &Oscillator::squareTick;
            break;
        case WaveformType::TRIANGLE:
            waveType = WaveformType::TRIANGLE;
            getTick = &Oscillator::triangleTick;
            break;
        case WaveformType::SAWTOOTHDOWN:
            waveType = WaveformType::SAWTOOTHDOWN;
            getTick = &Oscillator::sawDownTick;
            break;
        case WaveformType::SAWTOOTHUP:
            waveType = WaveformType::SAWTOOTHUP;
            getTick = &Oscillator::sawUpTick;
            break;
        case WaveformType::NOISE:
            waveType = WaveformType::NOISE;
            getTick = &Oscillator::noiseTick;
            break;
    }
}
