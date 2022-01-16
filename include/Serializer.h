#ifndef MUGEN_SERIALIZER_H
#define MUGEN_SERIALIZER_H
#include "json.h"
#include "MusicBox.h"
#include "Timeline.h"

using JSON = nlohmann::json;

class Serializer {
public:

    JSON extractProject(MusicBox *musicBox, Timeline *timeline);

    JSON extractComposition(Timeline *timeline);
    JSON extractSongSegmentsIndices(Timeline *timeline);
    JSON extractAllSegments(Timeline *timeline);
    JSON extractSegment(Segment *segment);
    JSON extractColumn(Column *column);

    JSON extractInstruments(MusicBox *musicBox);
    JSON extractEnvelope(Instrument *instrument);
    JSON extractOscillators(Instrument *pInstrument);
    JSON extractLFO(Oscillator *osc);
};

#endif //MUGEN_SERIALIZER_H