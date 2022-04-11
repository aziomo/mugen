#include <iostream>
#include "../include/AudioAPI.h"

AudioAPI::AudioAPI(const Config& config) {
    PaStreamParameters outParams;
    PaError err;
    m_blockSize = config.m_blockSize;
    err = Pa_Initialize();
    if (err != paNoError) goto error;
    outParams = getOutputStreamParams(config);
    err = Pa_OpenStream(
            &stream,
            nullptr,
            &outParams,
            config.m_sampleRate,
            config.m_blockSize,
            paNoFlag,
            nullptr,
            nullptr);
    if (err != paNoError) goto error;
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        error:
        std::cout << "An error occured while initializing PortAudio - " << Pa_GetErrorText( err );
        Pa_Terminate();
    }
}

PaStreamParameters AudioAPI::getOutputStreamParams(const Config &config)
{
    PaStreamParameters outParams;
    PaDeviceIndex outDeviceIndex = !config.m_device.empty()
            ? getDeviceId(config.m_device.c_str())
            : Pa_GetDefaultOutputDevice();

    double latency;
    if (outDeviceIndex >= 0) {
        latency = Pa_GetDeviceInfo(outDeviceIndex)->defaultLowOutputLatency;
        if (latency <= 0) latency = 0.2;
    }
    outParams.device = outDeviceIndex;
    outParams.channelCount = config.m_channels;
    outParams.sampleFormat = config.m_sampleType == 'f' ? paFloat32 : paInt16;
    outParams.suggestedLatency = latency;
    outParams.hostApiSpecificStreamInfo = nullptr;
    return outParams;
}

AudioAPI::~AudioAPI(){
    Pa_CloseStream(stream);
    stream = nullptr;
    Pa_Terminate();
}

void AudioAPI::listOutputDevices(){
    PaDeviceIndex deviceCount = Pa_GetDeviceCount();
    for (int i = 0; i < deviceCount; i++)
        printDeviceInfo(const_cast<PaDeviceInfo *>(Pa_GetDeviceInfo(i)), i);
}

void AudioAPI::printDeviceInfo(PaDeviceInfo* deviceInfo, PaDeviceIndex id){
    printf("[%d] %s\n", id, deviceInfo->name);
}

PaError AudioAPI::writeOut(float* frame) {
    return Pa_WriteStream(stream, frame, m_blockSize);
}

PaDeviceIndex AudioAPI::getDeviceId(const char* deviceName) {
    PaDeviceIndex deviceCount = Pa_GetDeviceCount();
    for (int i = 0; i < deviceCount; i++) {
        if (!strcmp(Pa_GetDeviceInfo(i)->name, deviceName))
            return i;
    }
    return -1;
}
