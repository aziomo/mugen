#ifndef MUGEN_CPP_WAVEFORMTYPE_H
#define MUGEN_CPP_WAVEFORMTYPE_H
#include <string>
using std::string;

enum class WaveformType {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTHDOWN,
    SAWTOOTHUP,
    NOISE
};

string static wavetypeToString(WaveformType type){
    switch (type) {
        case WaveformType::SINE:
            return "sine";
        case WaveformType::SQUARE:
            return "square";
        case WaveformType::TRIANGLE:
            return "triangle";
        case WaveformType::SAWTOOTHDOWN:
            return "sawdown";
        case WaveformType::SAWTOOTHUP:
            return "sawup";
        case WaveformType::NOISE:
            return "noise";
    }
    return "-";
}

WaveformType static stringToWavetype(const std::string& type){
    if (type == "sine")
        return WaveformType::SINE;
    if (type == "square")
        return WaveformType::SQUARE;
    if (type == "triangle")
        return WaveformType::TRIANGLE;
    if (type == "sawdown")
        return WaveformType::SAWTOOTHDOWN;
    if (type == "sawup")
        return WaveformType::SAWTOOTHUP;
    if (type == "noise")
        return WaveformType::NOISE;
    return WaveformType::SINE;
}


#endif //MUGEN_CPP_WAVEFORMTYPE_H
