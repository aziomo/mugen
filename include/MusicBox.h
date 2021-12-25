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
    Note pressedNotes[KEYBOARD_SIZE];

    SNDFILE* outputFile;

    float maxSample;
    void startPlaying();
    void stopPlaying();
    int getRootCPosition() const;
    std::vector<Instrument*> instruments;
    int currentInstrument = 0;
    int blockSize;
//private:
    int maxBlockCount = 4;
    std::mutex mutexBlocksReadyToRead;
    std::queue<float*> blocksBuffer;
    std::condition_variable blocksReadyToRead;
    std::atomic<int> blocksAvailable;

    double globalTime;
    double timeStep;

    const int octaveSize = 12;
    int currentOctave = 4;
    std::thread readThread, writeThread;
    AudioAPI* audioApi;

    static double midiToFrequency(int midiNote);

    void copyBlock(float *source, float *destination);

    void writePressedKeysToBuffer();
    bool readBlockFromBuffer(float *outputBlock);

    void bufferWriteLoop();
    void bufferReadLoop();

    void openFile();
    void closeFile();

    long writeBlockToFile(float *block);

    template <typename T>
    void zeroOutArray(T *array, int arraySize);

    void pressNoteKey(int keyPosition);

    void releaseNoteKey(int keyPosition);
};


#endif //MUGEN_CPP_MUSICBOX_H
