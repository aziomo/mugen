//
// Created by alberto on 12/19/21.
//

#ifndef MUGEN_ENVELOPE_H
#define MUGEN_ENVELOPE_H


struct Envelope {
    double attackDuration, decayDuration, releaseDuration,
            startingAmplitude, sustainAmplitude;
};

struct Note {
    bool isPressed;
    bool isPlaying;
    double pressedOnTime;
    double releasedOnTime;
    double frequency;
};


#endif //MUGEN_ENVELOPE_H
