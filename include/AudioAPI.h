//
// Created by alberto on 11/11/21.
//

#ifndef MUGEN_CPP_AUDIOAPI_H
#define MUGEN_CPP_AUDIOAPI_H

// ustawienia
#define SET_PORT             (0)
#define SET_VOLUME           (20)

#define SAMPLES_PER_FRAME   (2)
#define FRAMES_PER_BUFFER   (512)

#if 1
#define SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#else
#define SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#endif

#include <portaudio.h>
#include <alsa/asoundlib.h>


class AudioAPI {

public:
    AudioAPI(int bufferSize, double sampleRate);
    ~AudioAPI();
    PaError init(int bufferSize, double sampleRate);
    PaError writeOut(float* frame);

    int bufferSize;

    void ListOutputDevices();

private:
    PaStream* stream;
    double inLatency = -1;
    double outLatency = -1;
    int channels = 1;
    double sampleRate = 44100.0;
    PaDeviceIndex outPortID;


    void PrintDeviceInfo(PaDeviceInfo *deviceInfo, PaDeviceIndex deviceId);
    PaDeviceIndex GetDeviceId(const char* deviceName);
    PaDeviceIndex GetPulseDeviceId();
    PaDeviceIndex GetDefaultDeviceId();


    PaTime getSuggestedOutLatency(PaDeviceIndex deviceId);

    void setOutDeviceParameters(PaStreamParameters *params);
};


#endif //MUGEN_CPP_AUDIOAPI_H
