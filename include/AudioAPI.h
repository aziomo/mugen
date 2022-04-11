#ifndef MUGEN_CPP_AUDIOAPI_H
#define MUGEN_CPP_AUDIOAPI_H
#include "../include/Config.h"
#include <portaudio.h>
#include <alsa/asoundlib.h>

class AudioAPI
{
public:
    explicit AudioAPI(const Config& config);
    ~AudioAPI();
    PaError writeOut(float* frame);
    static void listOutputDevices();

private:
    int m_blockSize;
    PaStream* stream;
    static void printDeviceInfo(PaDeviceInfo *deviceInfo, PaDeviceIndex id);
    static PaDeviceIndex getDeviceId(const char* deviceName);
    static PaStreamParameters getOutputStreamParams(const Config &config) ;
};

#endif //MUGEN_CPP_AUDIOAPI_H
