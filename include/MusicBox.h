//
// Created by alberto on 11/11/21.
//

#ifndef MUGEN_CPP_MUSICBOX_H
#define MUGEN_CPP_MUSICBOX_H

#define KEYBOARD_SIZE 17

#include <queue>
#include "Oscillator.h"
#include "AudioAPI.h"
#include "Instrument.h"
#include <thread>

const double SEMITONE_RATIO = pow(2.0, 1.0 / 12.0);

class MusicBox {
public:
    MusicBox(int oscillators);
    ~MusicBox();
    bool isRunning;
    bool pressedKeys[KEYBOARD_SIZE];
    float getSample(double frequency);
    void fillSampleFrame(float* frame, double frequency);
    void playSound();
    void playMidiNote(int offset = 0);
    void startPlaying();
    void stopPlaying();
    void loadBlockOfSamples(float* frame);
    void playTwoNotes(int midiNote1, int midiNote2);
    void addOscillator(WaveformType wavetype);
    void popOscillator();
    int getRootCPosition();
    std::vector<Instrument*> instruments;
    int currentInstrument = 0;
    std::vector<Oscillator*> oscillators;
private:
    const int octaveSize = 12;
    int currentOctave = 4;
    std::thread mainThread;
    int blockSize;

    std::queue<float*> frameQueue;
    AudioAPI* audioApi;

    double midiToFrequency(int midiNote);
    void mainLoop();
    float* readBlockOfSamples();
    float getSampleAllOscs(double frequency);
};


#endif //MUGEN_CPP_MUSICBOX_H
