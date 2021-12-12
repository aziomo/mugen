#include <iostream>

#include "../include/MusicBox.h"

MusicBox::MusicBox() {
    audioApi = new AudioAPI(FRAMES_PER_BUFFER, 44100.0);
    blockSize = audioApi->bufferSize;
    isRunning = false;
    instruments.push_back(new Instrument(blockSize));
    blocksAvailable = 0;
    mainBuffer = new float[blockSize * maxBlockCount];
    outputFile = nullptr;
}

MusicBox::~MusicBox() {
    while (!instruments.empty()){
        instruments.pop_back();
    }
    delete audioApi;
}

void MusicBox::startPlaying(){
    isRunning = true;
//    mainThread = std::thread(&MusicBox::mainLoop, this);
    readThread = std::thread(&MusicBox::bufferReadLoop, this);
    writeThread = std::thread(&MusicBox::bufferWriteLoop, this);
}

void MusicBox::stopPlaying(){
    isRunning = false;
//    mainThread.join();
    readThread.join();
    writeThread.join();
}

void MusicBox::playMidiNote(int offset){
    int midiNote = getRootCPosition() + offset;
    for (int i = 0; i < 16; i++) {
        float frame[blockSize];
        instruments.front()->fillSampleBlock(frame, midiToFrequency(midiNote));
        audioApi->writeOut(frame);
    }
}

void MusicBox::putMidiNoteInQueue(int rootCOffset){
    std::unique_lock<std::mutex> lm(mutexBlocksReadyToRead);

    int midiNote = getRootCPosition() + rootCOffset;
    float* newBlock = new float[blockSize];
    instruments.front()->fillSampleBlock(newBlock, midiToFrequency(midiNote));

    // fillMainBlockAt(mainBuffer, rootCOffset, blockLength);

    if (blocksAvailable < maxBlockCount){
        loadBlockToQueue(newBlock);
    }
    else delete[] newBlock;
}


void MusicBox::putMidiNoteInMainBuffer(int rootCOffset){
//    std::unique_lock<std::mutex> lock(blocksQueueMutex);
    int midiNote = getRootCPosition() + rootCOffset;

    if (blocksAvailable < maxBlockCount){
        instruments.front()->fillMainBufferSegment(mainBuffer, (currentWriteBlockIndex * blockSize),
                                                   midiToFrequency(midiNote));
        currentWriteBlockIndex++;
        if (currentWriteBlockIndex == maxBlockCount) currentWriteBlockIndex = 0;
        blocksAvailable++;
//        std::unique_lock<std::mutex> lm(mutexBlocksReadyToRead);
        if (blocksAvailable > 3){
            std::cout <<"retard" << std::endl;
            std::unique_lock<std::mutex> lm(mutexBlocksReadyToRead);
            blocksReadyToRead.notify_one();
        }

    }
}

void MusicBox::writePressedKeysToMainBuffer(){
    if (blocksAvailable < maxBlockCount){
        bool anyPressed = false;
        for (bool keyPress : pressedKeys){
            if (keyPress) {
                anyPressed = true;
                break;
            }
        }

        if (anyPressed){
            int offset = currentWriteBlockIndex * blockSize;
            for (int i = 0; i < blockSize; i++) {
                mainBuffer[offset + i] = 0;
            }
            int scaleFactor = 0;
            for (int i = 0; i < KEYBOARD_SIZE; i++){
                if (pressedKeys[i]){
                    int midiNote = getRootCPosition() + i;
                    instruments.front()->addToMainBufferSegment(mainBuffer, (currentWriteBlockIndex * blockSize),
                                                                midiToFrequency(midiNote));
//                    instruments.front()->testAddTwoNotesToMainBufferSegment(mainBuffer, (currentWriteBlockIndex * blockSize),
//                                                                midiToFrequency(midiNote));
                    scaleFactor++;
                }
            }
            for (int i = 0; i < blockSize; i++) {
                mainBuffer[offset + i] /= scaleFactor;
                if (maxSample < mainBuffer[offset + i]){
                    maxSample = mainBuffer[offset + i];
                }
            }

            currentWriteBlockIndex++;
            if (currentWriteBlockIndex == maxBlockCount) currentWriteBlockIndex = 0;
            blocksAvailable++;
        }
    }
}


void MusicBox::writePressedKeysToQueue(){
    if (blocksAvailable < maxBlockCount){
        bool anyPressed = false;
        for (bool keyPress : pressedKeys){
            if (keyPress) {
                anyPressed = true;
                break;
            }
        }

        if (anyPressed){
            float* newBlock = new float[blockSize];
            for (int i = 0; i < blockSize; i++) {
                newBlock[i] = 0;
            }
            int scaleFactor = 0;
            for (int i = 0; i < KEYBOARD_SIZE; i++){
                if (pressedKeys[i]){
                    int midiNote = getRootCPosition() + i;
                    instruments.front()->addToMainBufferSegment(newBlock, 0,
                                                                midiToFrequency(midiNote));
//                    instruments.front()->testAddTwoNotesToMainBufferSegment(mainBuffer, (currentWriteBlockIndex * blockSize),
//                                                                midiToFrequency(midiNote));
                    scaleFactor++;
                }
            }
            for (int i = 0; i < blockSize; i++) {
                newBlock[i] /= scaleFactor;
                if (maxSample < newBlock[i]){
                    maxSample = newBlock[i];
                }
            }
            blocksQueue.push(newBlock);
            blocksAvailable++;
        }
    }
}


bool MusicBox::readFromMainBuffer(float* outputBlock) {

    if (blocksAvailable == 0) {
//        std::unique_lock<std::mutex> lm(mutexBlocksReadyToRead);
//        blocksReadyToRead.wait(lm);
        return false;
    } else {
        for(int i = 0; i < blockSize; i++){
            outputBlock[i] = mainBuffer[(currentReadBlockIndex * blockSize) + i];
        }
        currentReadBlockIndex++;
        if (currentReadBlockIndex == maxBlockCount) currentReadBlockIndex = 0;
        blocksAvailable--;
        return true;
    }

}



double MusicBox::midiToFrequency(int midiNote){
    double c5, c0, a4 = 440.0;
    c5 = a4 * pow(SEMITONE_RATIO, 3.0);
    c0 = c5 * pow(0.5, 5.0);
    return c0 * pow(SEMITONE_RATIO, (double) midiNote);
}

void MusicBox::copyBlock(float* source, float* destination){
    for (int i = 0; i < blockSize; i++){
        destination[i] = source[i];
    }
}

void MusicBox::mainLoop(){

    float outputBlock[blockSize];

    while (isRunning){

        readFromMainBuffer(outputBlock);
        audioApi->writeOut(outputBlock);

        /*           STEADY SIGNAL OUTPUT  */
//        instruments.front()->fillSampleBlock(outputBlock, 440);
//        audioApi->writeOut(outputBlock);
    }
}

void MusicBox::bufferWriteLoop(){
    while (isRunning) {
//        writePressedKeysToMainBuffer();
        writePressedKeysToQueue();
    }
}

void MusicBox::bufferReadLoop(){
    float outputBlock[blockSize];
    while (isRunning){

//      if (readFromMainBuffer(outputBlock)){
        if (readBlockFromQueue(outputBlock)){
            writeBlockToFile(outputBlock);
            audioApi->writeOut(outputBlock);
        }

        /*           STEADY SIGNAL OUTPUT  */
//        instruments.front()->fillSampleBlock(outputBlock, 440);
//        audioApi->writeOut(outputBlock);
    }
}

void MusicBox::loadBlockToQueue(float* frame) {
    blocksQueue.push(frame);
    blocksAvailable++;
//    blocksReadyToRead.notify_one();
}


bool MusicBox::readBlockFromQueue(float* outputBlock) {
    if (blocksAvailable == 0){
        return false;
    } else {
        copyBlock(blocksQueue.front(), outputBlock);
        blocksQueue.pop();
        blocksAvailable--;
        return true;
    }

}



int MusicBox::getRootCPosition() {
    return octaveSize * currentOctave;
}

void MusicBox::openFile() {
    if (outputFile == nullptr){
        SF_INFO fileInfo;
        fileInfo.samplerate = 44100;
        fileInfo.channels = 1;
        fileInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
        outputFile = sf_open("retard.wav", SFM_WRITE, &fileInfo);
        if (outputFile == nullptr){
            auto errorMessage = sf_strerror(nullptr);
            printf("%s\n", errorMessage);
        }
    }
}

long MusicBox::writeBlockToFile(float* block){
    return sf_write_float(outputFile, block, blockSize);
}

void MusicBox::closeFile(){
    sf_close(outputFile);
    outputFile = nullptr;
}