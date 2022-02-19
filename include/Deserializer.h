#ifndef MUGEN_DESERIALIZER_H
#define MUGEN_DESERIALIZER_H
#include "json.h"
#include "MusicBox.h"
#include "Timeline.h"

using JSON = nlohmann::json;


class Deserializer {
public:
    void deserializeInstrument(JSON instJson, Instrument* instrument);
    void deserializeEnvelope(JSON envJson, Envelope* envelope);
    void deserializeOscillator(JSON oscJson, Oscillator* osc);
    void deserializeLFO(JSON lfoJson, Oscillator* lfo);

};


#endif //MUGEN_DESERIALIZER_H
