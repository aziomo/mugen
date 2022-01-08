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

struct Column {
    Column() : bits(){}
    Bit* bits[5];
};

struct Segment {
    Segment(int rowsAmount, int index){
        for (int i = 0; i < rowsAmount; i++){
            cols.push_back(new Column());
        }
        this->index = index;
    }
    int index;
    vector<Column*> cols;
};


#endif //MUGEN_SEGMENT_H
