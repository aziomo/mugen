#include <iostream>

#include "../include/MusicBox.h"

MusicBox::MusicBox() {
    audioApi = new AudioAPI(FRAMES_PER_BUFFER, 44100.0);
    blockSize = audioApi->bufferSize;
    isRunning = false;
    instruments.push_back(new Instrument(blockSize));
    blocksAvailable = 0;
    outputFile = nullptr;
    timeStep = 1.0 / 44100.0;
    globalTime = 0.0;
    for (int i = 0; i < KEYBOARD_SIZE; i++) {
        pressedNotes[i].frequency = midiToFrequency(getRootCPosition() + i);
    }
}

MusicBox::~MusicBox() {
    while (!instruments.empty()) {
        instruments.pop_back();
    }
    delete audioApi;
}

void MusicBox::startPlaying() {
    isRunning = true;
    readThread = std::thread(&MusicBox::bufferReadLoop, this);
    writeThread = std::thread(&MusicBox::bufferWriteLoop, this);
}

void MusicBox::stopPlaying() {
    isRunning = false;
    readThread.join();
    writeThread.join();
}

template<typename T>
void MusicBox::zeroOutArray(T *array, int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        array[i] = 0;
    }
}

void MusicBox::writePressedKeysToBuffer() {
    if (blocksAvailable < maxBlockCount) {
        bool anyPlaying = false;
        for (auto note: pressedNotes)
            if (note.isPlaying) {
                anyPlaying = true;
                break;
            }

        if (anyPlaying) {
            float *newBlock = new float[blockSize];
            zeroOutArray(newBlock, blockSize);
            for (int i = 0; i < KEYBOARD_SIZE; i++) {
                if (pressedNotes[i].isPlaying) {
                    instruments.at(currentInstrument)->addToBufferBlock(newBlock, &pressedNotes[i], globalTime);
                }
            }
            globalTime += timeStep * blockSize;
            blocksBuffer.push(newBlock);
            blocksAvailable++;
        }
    }
}


double MusicBox::midiToFrequency(int midiNote) {
    double c5, c0, a4 = 440.0;
    c5 = a4 * pow(SEMITONE_RATIO, 3.0);
    c0 = c5 * pow(0.5, 5.0);
    return c0 * pow(SEMITONE_RATIO, (double) midiNote);
}

void MusicBox::copyBlock(float *source, float *destination) {
    for (int i = 0; i < blockSize; i++) {
        destination[i] = source[i];
    }
}

void MusicBox::bufferWriteLoop() {
    while (isRunning) {
        writePressedKeysToBuffer();
    }
}

void MusicBox::bufferReadLoop() {
    float outputBlock[blockSize];
    while (isRunning) {
        if (readBlockFromBuffer(outputBlock)) {
            writeBlockToFile(outputBlock);
            audioApi->writeOut(outputBlock);
        }
    }
}

bool MusicBox::readBlockFromBuffer(float *outputBlock) {
    if (blocksAvailable == 0) {
        return false;
    } else {
        copyBlock(blocksBuffer.front(), outputBlock);
        blocksBuffer.pop();
        blocksAvailable--;
        return true;
    }
}

int MusicBox::getRootCPosition() const {
    return octaveSize * currentOctave;
}

void MusicBox::openFile() {
    if (outputFile == nullptr) {
        SF_INFO fileInfo;
        fileInfo.samplerate = 44100;
        fileInfo.channels = 1;
        fileInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
        outputFile = sf_open("retard.wav", SFM_WRITE, &fileInfo);
        if (outputFile == nullptr) {
            auto errorMessage = sf_strerror(nullptr);
            printf("%s\n", errorMessage);
        }
    }
}

long MusicBox::writeBlockToFile(float *block) {
    return sf_write_float(outputFile, block, blockSize);
}

void MusicBox::closeFile() {
    sf_close(outputFile);
    outputFile = nullptr;
}

void MusicBox::pressNoteKey(int keyPosition) {
    pressedNotes[keyPosition].isPlaying = true;
    pressedNotes[keyPosition].isPressed = true;
    pressedNotes[keyPosition].pressedOnTime = globalTime;
}

void MusicBox::releaseNoteKey(int keyPosition) {
    pressedNotes[keyPosition].isPressed = false;
    pressedNotes[keyPosition].releasedOnTime = globalTime;
}