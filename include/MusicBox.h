#ifndef MUGEN_CPP_MUSICBOX_H
#define MUGEN_CPP_MUSICBOX_H

#define KEYBOARD_SIZE 17

#include <queue>
#include "Oscillator.h"
#include "AudioAPI.h"
#include "Instrument.h"
#include "Segment.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <sndfile.h>


class MusicBox {
public:

    MusicBox();
    ~MusicBox();
    bool isRunning, playbackKeys;

    bool pressedKeys[KEYBOARD_SIZE];
    Note pressedNotes[KEYBOARD_SIZE];

    SNDFILE* outputFile;

    float maxSample;
    void startPlaying();
    void stopPlaying();
    int getRootCPosition() const;
    vector<Instrument*> instruments;
    int currentInstrument = 0;
    int blockSize;
//private:
    int maxBlockCount = 4;
    std::mutex mu_blocksReadyToRead;
    std::condition_variable cv_blocksReadyToRead, cv_blocksReadyToWrite;
    std::queue<float*> blocksBuffer;
    std::atomic<int> blocksReadyToOutput;

    double globalTime;
    double timeStep;

    const int octaveSize = 12;
    int currentOctave = 4;
    std::thread readThread, writeThread;
    AudioAPI* audioApi;

    void copyBlock(const float *source, float *destination) const;

    void writePressedKeysToBuffer();
    bool readBlockFromBuffer(float *outputBlock);

    void bufferInputLoop();
    void bufferOutputLoop();

    void openFile();
    void closeFile();

    long writeBlockToFile(float *block) const;

    template <typename T>
    void zeroOutArray(T *array, int arraySize);

    void pressNoteKey(int keyPosition);

    void releaseNoteKey(int keyPosition);

    void writeBitsToBuffer(vector<Bit*> *bits);

    int keyToNoteValue(SDL_Keycode key) const;

    void tunePiano();

    void octaveUp();

    void octaveDown();
};


#endif //MUGEN_CPP_MUSICBOX_H
