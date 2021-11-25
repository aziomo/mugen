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

    ListOutputDevices();
    GetPulseDeviceId();
//    printf("choose output device: ");
//    scanf("%d", &out_dev_ndx);

    out_dev_ndx = GetPulseDeviceId();
//    out_dev_ndx = GetDeviceId("pipewire");


    in_dev_ndx = Pa_GetDefaultInputDevice();
    if (in_dev_ndx >= 0 && inLatency < 0) {/* suggested latency unselected, using default */
        inLatency = Pa_GetDeviceInfo(in_dev_ndx )->defaultLowInputLatency;
        if (inLatency <= 0) inLatency = 0.2;
    }
    inParams.device = in_dev_ndx;
    inParams.channelCount = channels;       /* stereo output */
    inParams.sampleFormat = SAMPLE_TYPE; /* 32 bit floating point output */
    inParams.suggestedLatency = inLatency;
    inParams.hostApiSpecificStreamInfo = nullptr;

//    outPortID = SET_PORT;
//    outPortID = Pa_GetDefaultOutputDevice();
//    outPortID = 10;
//    out_dev_ndx = outPortID;
    if (out_dev_ndx >=0 && outLatency < 0) {/* suggested latency unselected, using default */
        outLatency = Pa_GetDeviceInfo(out_dev_ndx )->defaultLowOutputLatency;
        if (outLatency <= 0) outLatency = 0.2;
    }
    outParams.device = out_dev_ndx;
    outParams.channelCount = channels;       /* stereo output */
    outParams.sampleFormat = SAMPLE_TYPE; /* 32 bit floating point output */
    outParams.suggestedLatency = outLatency;
    outParams.hostApiSpecificStreamInfo = nullptr;


    err = Pa_OpenStream(
            &stream,
            &inParams,
            &outParams,
            sampleRate,
            FRAMES_PER_BUFFER,
            paNoFlag,
            nullptr, /* no callback, use blocking API */
            nullptr); /* no callback, so no callback userData */
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

void AudioAPI::ListOutputDevices(){
    PaDeviceIndex deviceCount = Pa_GetDeviceCount();
    PaDeviceIndex defaultOutputDevice = Pa_GetDefaultOutputDevice();

    const PaDeviceInfo* devices[deviceCount];
    const PaDeviceInfo* defaultOutputInfo = Pa_GetDeviceInfo(defaultOutputDevice);

    printf("default output: \n");
    PrintDeviceInfo(const_cast<PaDeviceInfo *>(defaultOutputInfo), defaultOutputDevice);

    for (int i = 0; i < deviceCount; i++) {
        devices[i] = Pa_GetDeviceInfo(i);
        PrintDeviceInfo(const_cast<PaDeviceInfo *>(devices[i]), i);
    }
}

void AudioAPI::PrintDeviceInfo(PaDeviceInfo* deviceInfo, PaDeviceIndex id){
    printf("[%d] %s\n", id, deviceInfo->name);
}

PaError AudioAPI::writeOut(float* frame) {
    Pa_WriteStream(stream, frame, bufferSize);
}

PaDeviceIndex AudioAPI::GetPulseDeviceId() {
    PaDeviceIndex deviceCount = Pa_GetDeviceCount();
//    const PaDeviceInfo* devices[deviceCount];
//    const char* deviceNames[deviceCount];
    for (int i = 0; i < deviceCount; i++) {
        if (!strcmp(Pa_GetDeviceInfo(i)->name, "pulse")){
            return i;
        }
//        deviceNames[i] = Pa_GetDeviceInfo(i)->name;
//        PrintDeviceInfo(const_cast<PaDeviceInfo *>(devices[i]), i);
    }
}

PaDeviceIndex AudioAPI::GetDeviceId(const char* deviceName) {
    PaDeviceIndex deviceCount = Pa_GetDeviceCount();

//    const PaDeviceInfo* devices[deviceCount];
//    const char* deviceNames[deviceCount];
    for (int i = 0; i < deviceCount; i++) {
        if (!strcmp(Pa_GetDeviceInfo(i)->name, deviceName)){
            return i;
        }
//        deviceNames[i] = Pa_GetDeviceInfo(i)->name;
//        PrintDeviceInfo(const_cast<PaDeviceInfo *>(devices[i]), i);
    }
}
