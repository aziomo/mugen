


#include "../include/MusicBox.h"



MusicBox::MusicBox() {
    audioApi = new AudioAPI(512, 44100.0);
    blockSize = audioApi->bufferSize;
    isRunning = false;
    instruments.push_back(new Instrument(blockSize));
    blocksLeft = 0;
}

MusicBox::~MusicBox() {
    while (!instruments.empty()){
        instruments.pop_back();
    }
    delete audioApi;
}

void MusicBox::startPlaying(){
    isRunning = true;
    mainThread = std::thread(&MusicBox::mainLoop, this);
}

void MusicBox::stopPlaying(){
    isRunning = false;
    mainThread.join();
}

void MusicBox::playMidiNote(int offset){
    int midiNote = getRootCPosition() + offset;
    for (int i = 0; i < 16; i++) {
        float frame[blockSize];
        instruments.front()->fillSampleBlock(frame, midiToFrequency(midiNote));
        audioApi->writeOut(frame);
    }
}

void MusicBox::putMidiNoteInQueue(int offset){
    std::unique_lock<std::mutex> lock(blocksQueueMutex);

    int midiNote = getRootCPosition() + offset;
    float* newBlock = new float[blockSize];
    instruments.front()->fillSampleBlock(newBlock, midiToFrequency(midiNote));
    if (blocksLeft < 8){
        blocksQueue.push(newBlock);
        blocksLeft++;
    }
    else delete[] newBlock;
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

    float mainOutputBlock[blockSize];

    while (isRunning){

        if (blocksLeft > 0) {
            readBlockOfSamples(mainOutputBlock);
            audioApi->writeOut(mainOutputBlock);
        }

        /*           STEADY SIGNAL OUTPUT  */
//        instruments.front()->fillSampleBlock(mainOutputBlock, 440);
//        audioApi->writeOut(mainOutputBlock);
    }
}

void MusicBox::loadBlockOfSamples(float* frame) {

    std::unique_lock<std::mutex> lock(blocksQueueMutex);
    if (blocksLeft == 8){
        queueFull.wait(lock);
    }
    blocksQueue.push(frame);
    blocksLeft++;
    queueFull.notify_one();
}


void MusicBox::readBlockOfSamples(float* outputBlock) {

    // new version
    std::unique_lock<std::mutex> lock(blocksQueueMutex);
    copyBlock(blocksQueue.front(), outputBlock);
    blocksQueue.pop();
    blocksLeft--;

    // old version

    /*
    std::unique_lock<std::mutex> lock(blocksQueueMutex);
    if (blocksLeft == 0){
        queueFull.wait(lock);
    }
    queueFull.notify_one();

    return blocksQueue.front();
//    blocksQueue.pop();
    */
}

int MusicBox::getRootCPosition() {
    return octaveSize * currentOctave;
}

/*
void MusicBox::playTwoNotes(int midiNote1, int midiNote2){
    float maxSample;
    for (int i = 0; i < 16; i++) {
        float frame1[blockSize], frame2[blockSize];
        instruments.front()->fillSampleFrame(frame1, midiToFrequency(midiNote1));
        instruments.front()->fillSampleBlock(frame2, midiToFrequency(midiNote2));
//        mixSampleFrames(frame1, frame2); // DELETED
        audioApi->writeOut(frame1);
    }
}
*/


