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
#include <sndfile.h>

const double SEMITONE_RATIO = pow(2.0, 1.0 / 12.0);

class MusicBox {
public:
    MusicBox();
    ~MusicBox();
    bool isRunning;

    bool pressedKeys[KEYBOARD_SIZE];
//    std::vector<bool> pressedKeys = {};

    SNDFILE* outputFile;

    float maxSample;
    void playMidiNote(int offset = 0);
    void startPlaying();
    void stopPlaying();
    void loadBlockToQueue(float* frame);
    //void playTwoNotes(int midiNote1, int midiNote2);
    int getRootCPosition();
    std::vector<Instrument*> instruments;
    int currentInstrument = 0;
    int blockSize;
    void putMidiNoteInQueue(int rootCOffset);
    void putMidiNoteInMainBuffer(int rootCOffset);
    bool readFromMainBuffer(float* outputBlock);
//private:
    int maxBlockCount = 4;
    std::mutex mutexBlocksReadyToRead;
    std::queue<float*> blocksQueue;
    float* mainBuffer;
    std::condition_variable blocksReadyToRead;
//    int blocksAvailable;
    std::atomic<int> blocksAvailable;

    double globalTime;
    double timeStep;

    int currentWriteBlockIndex = 0;
    int currentReadBlockIndex = 0; // starting from 1

    const int octaveSize = 12;
    int currentOctave = 4;
    std::thread readThread, writeThread;
    AudioAPI* audioApi;

    double midiToFrequency(int midiNote);

    void copyBlock(float *source, float *destination);

    bool readBlockFromQueue(float *outputBlock);

    void writePressedKeysToMainBuffer();

    void bufferWriteLoop();

    void bufferReadLoop();

    void openFile();

    void closeFile();

    long writeBlockToFile(float *block);

    void writePressedKeysToQueue();

    void newWritePressedKeysToQueue();

    template <typename T>
    void zeroOutArray(T *array, int arraySize);

};


#endif //MUGEN_CPP_MUSICBOX_H
