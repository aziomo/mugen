#include <iostream>

#include "../include/MusicBox.h"

MusicBox::MusicBox() {
    audioApi = new AudioAPI(FRAMES_PER_BUFFER, 44100.0);
    blockSize = audioApi->bufferSize;
    isRunning = false;
    playbackKeys = true;
    instruments.push_back(new Instrument(blockSize));
    blocksReadyToOutput = 0;
    outputFile = nullptr;
    timeStep = 1.0 / 44100.0;
    globalTime = 0.0;
    tunePiano();
}

MusicBox::~MusicBox() {
    while (!instruments.empty()) {
        instruments.pop_back();
    }
    delete audioApi;
}

void MusicBox::octaveUp(){
    if (currentOctave < 8)
        currentOctave++;
    tunePiano();
}

void MusicBox::octaveDown(){
    if (currentOctave > 0)
        currentOctave--;
    tunePiano();
}

void MusicBox::tunePiano(){
    for (int i = 0; i < KEYBOARD_SIZE; i++) {
        pressedNotes[i].frequency = midiToFreq(getRootCPosition() + i);
    }
}

void MusicBox::startPlaying() {
    isRunning = true;
    readThread = std::thread(&MusicBox::bufferOutputLoop, this);
    if (playbackKeys){
        writeThread = std::thread(&MusicBox::bufferInputLoop, this);
    }
}

void MusicBox::stopPlaying() {
    isRunning = false;
    cv_blocksReadyToRead.notify_one();
    readThread.join();
    cv_blocksReadyToWrite.notify_one();
    if (writeThread.joinable())
        writeThread.join();
}

template<typename T>
void MusicBox::zeroOutArray(T *array, int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        array[i] = 0;
    }
}

void MusicBox::writePressedKeysToBuffer() {
    if (blocksReadyToOutput < maxBlockCount) {
        std::unique_lock<std::mutex> locker(mu_blocksReadyToRead);
        if (blocksReadyToOutput == 0)
            cv_blocksReadyToWrite.wait(locker);
        if (!isRunning){
            return;
        }
        bool anyPlaying = false;
        for (auto note: pressedNotes)
            if (note.isAudible) {
                anyPlaying = true;
                break;
            }

        if (anyPlaying) {

            float *newBlock = new float[blockSize];
            zeroOutArray(newBlock, blockSize);
            for (int i = 0; i < KEYBOARD_SIZE; i++) {
                if (pressedNotes[i].isAudible) {
                    instruments.at(currentInstrument)->addToBufferBlock(newBlock, &pressedNotes[i], globalTime);
                }
            }
            globalTime += timeStep * blockSize;
            blocksBuffer.push(newBlock);

            blocksReadyToOutput++;
            if (blocksReadyToOutput == 1)
                cv_blocksReadyToRead.notify_one();
        }
    }
}

void MusicBox::writeBitsToBuffer(vector<Bit*> *bits){
    auto *newBlock = new float[blockSize];
    zeroOutArray(newBlock, blockSize);
    for (auto bit : *bits) {
        bit->instrument->addToBufferBlock(newBlock, &bit->note, globalTime);
    }
    globalTime += timeStep * blockSize;
    blocksBuffer.push(newBlock);
    blocksReadyToOutput++;
    if (blocksReadyToOutput == 1){
        cv_blocksReadyToRead.notify_one();
    }
}



void MusicBox::copyBlock(const float *source, float *destination) const {
    for (int i = 0; i < blockSize; i++) {
        destination[i] = source[i];
    }
}

void MusicBox::bufferInputLoop() {
    while (isRunning) {
        writePressedKeysToBuffer();
    }
}

void MusicBox::bufferOutputLoop() {
    float outputBlock[blockSize];
    while (isRunning) {
        if (!readBlockFromBuffer(outputBlock)) break;
        if (outputFile != nullptr) writeBlockToFile(outputBlock);
        else audioApi->writeOut(outputBlock);
    }
}

bool MusicBox::readBlockFromBuffer(float *outputBlock) {
    std::unique_lock<std::mutex> locker(mu_blocksReadyToRead);
    if (blocksReadyToOutput == 0)
        cv_blocksReadyToRead.wait(locker);
    if (!isRunning)
        return false;

    copyBlock(blocksBuffer.front(), outputBlock);
    blocksBuffer.pop();
    blocksReadyToOutput--;
    if (blocksReadyToOutput == maxBlockCount - 1){
        cv_blocksReadyToWrite.notify_one();
    }
    return true;
}

int MusicBox::getRootCPosition() const {
    return octaveSize * currentOctave;
}

void MusicBox::openFile(const string& filepath) {
    if (outputFile == nullptr) {
        SF_INFO fileInfo;
        fileInfo.samplerate = 44100;
        fileInfo.channels = 1;
        fileInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
        outputFile = sf_open(filepath.c_str(), SFM_WRITE, &fileInfo);
        if (outputFile == nullptr) {
            auto errorMessage = sf_strerror(nullptr);
            printf("%s\n", errorMessage);
        }
    }
}

long MusicBox::writeBlockToFile(float *block) const {
    return sf_write_float(outputFile, block, blockSize);
}

void MusicBox::closeFile() {
    auto errorMessage = sf_strerror(nullptr);
    printf("%s\n", errorMessage);
    sf_close(outputFile);
    outputFile = nullptr;
}

void MusicBox::pressNoteKey(int keyPosition) {
    cv_blocksReadyToWrite.notify_one();
    pressedNotes[keyPosition].isAudible = true;
    pressedNotes[keyPosition].pressedOnTime = globalTime;
}

void MusicBox::releaseNoteKey(int keyPosition) {
    pressedNotes[keyPosition].releasedOnTime = globalTime;
}

int MusicBox::keyToNoteValue(SDL_Keycode key) const{
    switch (key){
        case SDLK_z:
            return getRootCPosition() + 0;
        case SDLK_s:
            return getRootCPosition() + 1;
        case SDLK_x:
            return getRootCPosition() + 2;
        case SDLK_d:
            return getRootCPosition() + 3;
        case SDLK_c:
            return getRootCPosition() + 4;
        case SDLK_v:
            return getRootCPosition() + 5;
        case SDLK_g:
            return getRootCPosition() + 6;
        case SDLK_b:
            return getRootCPosition() + 7;
        case SDLK_h:
            return getRootCPosition() + 8;
        case SDLK_n:
            return getRootCPosition() + 9;
        case SDLK_j:
            return getRootCPosition() + 10;
        case SDLK_m:
            return getRootCPosition() + 11;
        case SDLK_COMMA:
            return getRootCPosition() + 12;
        case SDLK_l:
            return getRootCPosition() + 13;
        case SDLK_PERIOD:
            return getRootCPosition() + 14;
        case SDLK_SEMICOLON:
            return getRootCPosition() + 15;
        case SDLK_SLASH:
            return getRootCPosition() + 16;
        default:
            return 0;
    }
}