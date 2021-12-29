//
// Created by alberto on 12/27/21.
//

#ifndef MUGEN_SEGMENT_H
#define MUGEN_SEGMENT_H

#include "Instrument.h"


struct Bit {
    Note note;
    Instrument* instrument;
};

struct Row {

    Bit bits[5];
};

struct Segment {
    Segment(){
        rows.push_back(Row());
        rows.push_back(Row());
        rows.push_back(Row());
        rows.push_back(Row());
        rows.push_back(Row());
        rows.push_back(Row());
    }

    vector<Row> rows;
};


#endif //MUGEN_SEGMENT_H
