#include <iostream>
#include "../include/AudioAPI.h"

PaError AudioAPI::init(int bufferSize, double sampleRate)
{
    PaStreamParameters inParams, outParams;
    PaDeviceIndex in_dev_ndx, out_dev_ndx;
    PaError  err;

    this->bufferSize = bufferSize;
    this->sampleRate = sampleRate;

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    out_dev_ndx = getPulseDeviceId();

    in_dev_ndx = Pa_GetDefaultInputDevice();
    if (in_dev_ndx >= 0 && inLatency < 0) {
        inLatency = Pa_GetDeviceInfo(in_dev_ndx )->defaultLowInputLatency;
        if (inLatency <= 0) inLatency = 0.2;
    }
    inParams.device = in_dev_ndx;
    inParams.channelCount = channels;
    inParams.sampleFormat = SAMPLE_TYPE;
    inParams.suggestedLatency = inLatency;
    inParams.hostApiSpecificStreamInfo = nullptr;

    if (out_dev_ndx >=0 && outLatency < 0) {
        outLatency = Pa_GetDeviceInfo(out_dev_ndx )->defaultLowOutputLatency;
        if (outLatency <= 0) outLatency = 0.2;
    }
    outParams.device = out_dev_ndx;
    outParams.channelCount = channels;
    outParams.sampleFormat = SAMPLE_TYPE;
    outParams.suggestedLatency = outLatency;
    outParams.hostApiSpecificStreamInfo = nullptr;


    err = Pa_OpenStream(
            &stream,
            &inParams,
            &outParams,
            sampleRate,
            FRAMES_PER_BUFFER,
            paNoFlag,
            nullptr,
            nullptr);
    if( err != paNoError ) goto error;
    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    return err;

    error:
    std::cout << "An error occured while initializing PortAudio - " << Pa_GetErrorText( err );
    Pa_Terminate();
    return err;
}

AudioAPI::AudioAPI(int bufferSize, double sampleRate) {
    init(bufferSize, sampleRate);
}

AudioAPI::~AudioAPI(){
    Pa_CloseStream(stream);
    stream = nullptr;
    Pa_Terminate();
}

void AudioAPI::listOutputDevices(){
    PaDeviceIndex deviceCount = Pa_GetDeviceCount();
    PaDeviceIndex defaultOutputDevice = Pa_GetDefaultOutputDevice();

    const PaDeviceInfo* devices[deviceCount];
    const PaDeviceInfo* defaultOutputInfo = Pa_GetDeviceInfo(defaultOutputDevice);

    printf("default output: \n");
    printDeviceInfo(const_cast<PaDeviceInfo *>(defaultOutputInfo), defaultOutputDevice);

    for (int i = 0; i < deviceCount; i++) {
        devices[i] = Pa_GetDeviceInfo(i);
        printDeviceInfo(const_cast<PaDeviceInfo *>(devices[i]), i);
    }
}

void AudioAPI::printDeviceInfo(PaDeviceInfo* deviceInfo, PaDeviceIndex id){
    printf("[%d] %s\n", id, deviceInfo->name);
}

PaError AudioAPI::writeOut(float* frame) {
    Pa_WriteStream(stream, frame, bufferSize);
}

PaDeviceIndex AudioAPI::getPulseDeviceId() {
    PaDeviceIndex deviceCount = Pa_GetDeviceCount();
    for (int i = 0; i < deviceCount; i++) {
        if (!strcmp(Pa_GetDeviceInfo(i)->name, "pulse")){
            return i;
        }
    }
}

PaDeviceIndex AudioAPI::getDeviceId(const char* deviceName) {
    PaDeviceIndex deviceCount = Pa_GetDeviceCount();
    for (int i = 0; i < deviceCount; i++) {
        if (!strcmp(Pa_GetDeviceInfo(i)->name, deviceName)){
            return i;
        }
    }
}
