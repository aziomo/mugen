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

// index translation scheme: (3,14 / 2PI) * 1024 = 512

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
    double Pc = fmod(getPhaseV2(dTime, lfo->currentFrequency), TWOPI);
    return lfoLookupTable[phaseToIndex(Pc)];
}

double Oscillator::getLfoInterpolatedSample(double dTime){

    double Pc = fmod(getPhaseV2(dTime, lfo->currentFrequency), TWOPI);

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

//    return lfoLookupTable[phaseToIndex(Pc)];
}



double Oscillator::sineTick()
{
    double value = sin(currentPhase); // = sin(TWOPI * currentFrequency / sampleRate * totalTicks);
    shiftPhase();                     // totalTicks++;
    return value;
}

double Oscillator::newGetSample(double dTime)
{
    return ampModifier * (this->*newGetTick)(dTime);
}

double Oscillator::newSineTick(double dTime)
{
//    if (lfo != nullptr){
//        return sin(getPhase(dTime) + baseFrequency * lfo->newGetSample(dTime));
//    }
    return sin(getPhase(dTime));
}

void Oscillator::newSetFrequencyWithLFO(double dTime)
{
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

double Oscillator::getPhase(double dTime)
{
    if (lfo != nullptr){
//        return TWOPI * currentFrequency * dTime + lfo->ampModifier * getLfoTruncatedSample(dTime);
        return TWOPI * currentFrequency * dTime + lfo->ampModifier * 10.0 / lfo->currentFrequency * getLfoInterpolatedSample(dTime);
    }
    return TWOPI * currentFrequency * dTime;
}

double Oscillator::getPhaseV2(double dTime, double frequency){
    return TWOPI * frequency * dTime;
}

double Oscillator::newSquareTick(double dTime)
{
//    if (lfo != nullptr){
//        return sin(getPhase(dTime) + baseFrequency * lfo->newGetSample(dTime)) <= 0 ? 1.0 : -1.0;
//    }
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

double Oscillator::newTriangleTick(double dTime)
{

    double value;

    value = 2.0 * (fmod(getPhase(dTime), TWOPI) * (1.0 / TWOPI)) - 1;

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
