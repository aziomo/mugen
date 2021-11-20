//
// Created by alberto on 11/11/21.
//

#include <thread>
#include <mutex>
#include <condition_variable>
#include "MusicBox.h"

std::mutex mu;
std::queue<float*> blocksQueue;
std::condition_variable queueFull;
int blocksLeft;

MusicBox::MusicBox(int oscillators) {
    audioApi = new AudioAPI(512, 44100.0);
    blockSize = audioApi->bufferSize;
    isRunning = false;
    for (int i = 0; i < oscillators; i++){
        auto* osc = new Oscillator(44100);
        this->oscillators.push_back(osc);
    }
}

MusicBox::~MusicBox() {
    for (Oscillator* osc : this->oscillators) {
        delete osc;
    }
}

void MusicBox::addOscillator(WaveformType wavetype){
    auto* osc = new Oscillator(44100, wavetype);
    this->oscillators.push_back(osc);
}

void MusicBox::popOscillator(){
    if (!oscillators.empty())
        oscillators.pop_back();
}

//void MusicBox::createBlockOfSamples(){
//    float block[blockSize];
//    float singleSample = 0;
//    for (int i = 0; i < blockSize; i++){
//        for (int j = 0; i < KEYBOARD_SIZE; j++){
//            if (pressedKeys[j]){
//                singleSample += oscillators[0]->getSample()
//            }
//
//        }
//    }
//
//}

void MusicBox::startPlaying(){
    isRunning = true;
    mainThread = std::thread(&MusicBox::mainLoop, this);
}

void MusicBox::stopPlaying(){
    isRunning = false;
    mainThread.join();
}

void MusicBox::fillSampleFrame(float* frame, double frequency, double amplitude) {
    for (int i = 0; i < blockSize; i++) {
        frame[i] = getSampleAllOscs(frequency, amplitude);
    }
}

float MusicBox::getSample(double frequency, double amplitude) {
    this->oscillators[0]->setFrequency(frequency);
    return (float) this->oscillators[0]->getSample(amplitude);
}

float MusicBox::getSampleAllOscs(double frequency, double amplitude) {
    float sample = 0;
    int nOscs = oscillators.size();
    for (int i = 1; i < nOscs + 1; i++){
        auto* osc = oscillators[i-1];
        osc->setFrequency(frequency * i);
        sample += (float) osc->getSample(amplitude * (i * 0.5));
    }
    if (nOscs > 1)
        sample /= nOscs;
    return sample;
}

void MusicBox::playMidiNote(int offset){
    int midiNote = getRootCPosition() + offset;
    for (int i = 0; i < 16; i++) {
        float frame[blockSize];
        fillSampleFrame(frame, midiToFrequency(midiNote), 0.1);
        audioApi->writeOut(frame);
    }
}

void MusicBox::playTwoNotes(int midiNote1, int midiNote2){
    float maxSample;
    for (int i = 0; i < 16; i++) {
        float frame1[blockSize], frame2[blockSize];
        fillSampleFrame(frame1, midiToFrequency(midiNote1), 0.3);
        fillSampleFrame(frame2, midiToFrequency(midiNote2), 0.3);
//        mixSampleFrames(frame1, frame2); // DELETED
        audioApi->writeOut(frame1);
    }
}


double MusicBox::midiToFrequency(int midiNote){
    double c5, c0, a4 = 440.0;
    c5 = a4 * pow(SEMITONE_RATIO, 3.0);
    c0 = c5 * pow(0.5, 5.0);
    return c0 * pow(SEMITONE_RATIO, (double) midiNote);
}

void MusicBox::mainLoop(){

    float frame[blockSize];

    while (isRunning){

//        frame = readBlockOfSamples();

        fillSampleFrame(frame, 220, 0.1);
//        audioApi->writeOut(readBlockOfSamples());
        audioApi->writeOut(frame);
//        blocksQueue.pop();
//        blocksLeft--;
    }
}


void MusicBox::loadBlockOfSamples(float* frame) {

    std::unique_lock<std::mutex> lock(mu);
    if (blocksLeft == 8){
        queueFull.wait(lock);
    }
    blocksQueue.push(frame);
    blocksLeft++;
    queueFull.notify_one();
}

float* MusicBox::readBlockOfSamples() {
    std::unique_lock<std::mutex> lock(mu);
    if (blocksLeft == 0){
        queueFull.wait(lock);
    }
    queueFull.notify_one();

    return blocksQueue.front();
//    blocksQueue.pop();
}

void MusicBox::playSound(){
    for (int i = 0; i < 8; i++) {
        float frame[audioApi->bufferSize];
        fillSampleFrame(frame, 220.0, 0.3);
        audioApi->writeOut(frame);
    }
}

int MusicBox::getRootCPosition() {
    return octaveSize * currentOctave;
}



