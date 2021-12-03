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
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

const double SEMITONE_RATIO = pow(2.0, 1.0 / 12.0);

class MusicBox {
public:
    MusicBox();
    ~MusicBox();
    bool isRunning;
    bool pressedKeys[KEYBOARD_SIZE];
    void playMidiNote(int offset = 0);
    void startPlaying();
    void stopPlaying();
    void loadBlockOfSamples(float* frame);
    //void playTwoNotes(int midiNote1, int midiNote2);
    int getRootCPosition();
    std::vector<Instrument*> instruments;
    int currentInstrument = 0;
    int blockSize;
    void putMidiNoteInQueue(int offset);

//private:
    std::mutex blocksQueueMutex;
    std::queue<float*> blocksQueue;
    std::condition_variable queueFull;
    int blocksLeft;
//    std::atomic<int> blocksLeft;

    const int octaveSize = 12;
    int currentOctave = 4;
    std::thread mainThread;

    std::queue<float*> frameQueue;
    AudioAPI* audioApi;

    double midiToFrequency(int midiNote);
    void mainLoop();
//    float* readBlockOfSamples();

    void copyBlock(float *source, float *destination);

    void readBlockOfSamples(float *outputBlock);


};


#endif //MUGEN_CPP_MUSICBOX_H
