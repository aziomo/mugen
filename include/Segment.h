//
// Created by alberto on 12/27/21.
//

#ifndef MUGEN_SEGMENT_H
#define MUGEN_SEGMENT_H

#include "Instrument.h"
#include "Texture.h"


struct Bit {
    int holdTicks;
    int holdSection;
    Note note;
    Instrument* instrument;
    Bit(int midiNote, Instrument* instrument){
        this->instrument = instrument;
        note.frequency = midiToFreq(midiNote);
    }
};

struct Row {
    Bit* bits[5];
};

struct Segment {
    Segment(int rowsAmount){
        for (int i = 0; i < rowsAmount; i++)
            rows.emplace_back();
    }

    vector<Row> rows;
};


#endif //MUGEN_SEGMENT_H
