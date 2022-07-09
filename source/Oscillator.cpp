#include "../include/Oscillator.h"

using std::abs, std::max, std::min;

Oscillator::Oscillator(int sampleRate, WaveformType waveformType) {
    setWaveformType(waveformType);
    this->sampleRate = sampleRate;
    frequency = 0.0;
    freqModifier = 1.0;
    ampModifier = 1.0;
    lfo = nullptr;
}

void Oscillator::setFrequency(double frequency) {
    this->frequency = frequency * freqModifier;
}

void Oscillator::setupLfoLookup(WaveformType type) {
    double xIncrement = TWOPI / (double) lookupTableSize,
    startingX = 0.0,
    currentX = startingX,
    currentY = YofX(currentX, type),
    maxSample = 0.0,
    minSample = abs(currentY);

    for (double &sample: lfoLookupTable)
        sample = 0;

    for (double & i : lfoLookupTable) {
        currentX += xIncrement;
        currentY += YofX(currentX, type);
        maxSample = max(maxSample, abs(currentY));
        minSample = min(minSample, abs(currentY));
    }

    currentX = startingX;
    currentY = YofX(currentX, type);

    for (double &sample : lfoLookupTable) {
        double normalizedY = (currentY - minSample) / (maxSample - minSample);
        sample = normalizedY;
        currentX += xIncrement;
        currentY += YofX(currentX, type);
    }
}

int Oscillator::phaseToIndex(double phase) {
    return (int) (phase / TWOPI * lookupTableSize);
}

double Oscillator::indexToPhase(int index) {
    return (double) index / lookupTableSize * TWOPI;
}

double Oscillator::getLfoInterpSample(double dTime) {
    double x = fmod(getPhase(dTime, lfo->frequency), TWOPI);
    int x0Index = phaseToIndex(x);
    int x1Index = (x0Index == lookupTableSize - 1) ? 0 : x0Index + 1;
    if (x == lfoLookupTable[x0Index])
        return lfoLookupTable[x0Index];

    double x0 = indexToPhase(x0Index),
    x1 = indexToPhase(x1Index),
    y0 = lfoLookupTable[x0Index],
    y1 = lfoLookupTable[x1Index];
    return y0 + (y1-y0)/(x1-x0) * (x - x0);
}

double Oscillator::YofX(double x, WaveformType function) {
    switch (function) {
        case WaveformType::SINE:
            return sin(x);
        case WaveformType::SQUARE:
            return sin(x) > 0 ? 1.0 : -1.0;
        case WaveformType::TRIANGLE:
            return asin(sin(x)) * 2.0 / PI;
        case WaveformType::SAWTOOTHDOWN:
            return 1.0 - 2.0 * (fmod(x, TWOPI) * (1.0 / TWOPI));
        case WaveformType::SAWTOOTHUP:
            return (2.0 * (fmod(x, TWOPI) * (1.0 / TWOPI))) - 1.0;
        case WaveformType::NOISE:
            return 2.0 * ((double) rand() / (double) RAND_MAX) - 1.0;
    }
    return sin(x);
}

double Oscillator::getPhase(double dTime) {
    if (lfo != nullptr) {
        return TWOPI * this->frequency * dTime
               + lfo->ampModifier * 100.0 / lfo->frequency * getLfoInterpSample(dTime);
    }
    return TWOPI * this->frequency * dTime;
}

void Oscillator::setLFO(WaveformType waveformType) {
    this->lfo = new Oscillator(sampleRate, waveformType);
    setupLfoLookup(waveformType);
}

double Oscillator::getSample(double dTime) {
    return ampModifier * (this->*getTick)(dTime);
}

double Oscillator::getPhase(double dTime, double frequency) {
    return TWOPI * frequency * dTime;
}
double Oscillator::sineTick(double dTime) {
    return sin(getPhase(dTime));
}
double Oscillator::squareTick(double dTime) {
    return sin(getPhase(dTime)) <= 0 ? 1.0 : -1.0;
}
double Oscillator::triangleTick(double dTime) {
    return asin(sin(getPhase(dTime))) * 2.0 / PI;
}
double Oscillator::sawDownTick(double dTime) {
    double phase = fmod(getPhase(dTime), TWOPI);
    return 1.0 - 2.0 * (phase * (1.0 / TWOPI));
}
double Oscillator::sawUpTick(double dTime) {
    double phase = fmod(getPhase(dTime), TWOPI);
    return (2.0 * (phase * (1.0 / TWOPI))) - 1.0;
}
double Oscillator::noiseTick(double dTime) {
    return 2.0 * ((double) rand() / (double) RAND_MAX) - 1.0;
}

void Oscillator::unsetLFO() {
    delete lfo;
    lfo = nullptr;
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
