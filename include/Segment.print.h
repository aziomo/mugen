struct Bit {
    int holdDuration, holdSection;
    Note note;
    Instrument* instrument;
};

struct Column {
    Bit* bits[5];
};

struct Segment {
    int index;
    vector<Column*> cols;
};

