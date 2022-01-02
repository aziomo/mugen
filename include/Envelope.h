//
// Created by alberto on 12/19/21.
//

#ifndef MUGEN_ENVELOPE_H
#define MUGEN_ENVELOPE_H

#include <cmath>

const double SEMITONE_RATIO = pow(2.0, 1.0 / 12.0);

static double midiToFreq(int midiNote) {
    double c5, c0, a4 = 440.0;
    c5 = a4 * pow(SEMITONE_RATIO, 3.0);
    c0 = c5 * pow(0.5, 5.0);
    return c0 * pow(SEMITONE_RATIO, (double) midiNote);
}

static int freqToMidi(double frequency) {
    double c5, c0, a4 = 440.0;
    c5 = a4 * pow(SEMITONE_RATIO, 3.0);
    c0 = c5 * pow(0.5, 5.0);
    double midiNote = log(frequency / c0) / log(SEMITONE_RATIO);
    return lround(midiNote);
}

struct Note {
    bool isAudible;
    double pressedOnTime = 0.0;
    double releasedOnTime = 0.0;
    double frequency;
};

struct Envelope {
    double initialAmplitude;
    double sustainAmplitude;
    double attackDuration;
    double decayDuration;
    double releaseDuration;

    double getAmplifier(double globalTime, Note *note) const {
        if (note->pressedOnTime > note->releasedOnTime) {
            if (globalTime >= note->pressedOnTime && globalTime < note->pressedOnTime + attackDuration) {
                return ((globalTime - note->pressedOnTime) / attackDuration) * initialAmplitude;
            } else if (globalTime < note->pressedOnTime + attackDuration + decayDuration) {
                return initialAmplitude - (globalTime - note->pressedOnTime - attackDuration) / decayDuration
                                          * (initialAmplitude - sustainAmplitude);
            } else {
                return sustainAmplitude;
            }
        } else if (globalTime < note->releasedOnTime + releaseDuration) {
            if (note->releasedOnTime - note->pressedOnTime < attackDuration) {
                double ampAtRelease =
                        ((note->releasedOnTime - note->pressedOnTime) / attackDuration) * initialAmplitude;
                return ampAtRelease - ampAtRelease * ((globalTime - note->releasedOnTime) / releaseDuration);
            } else if (note->releasedOnTime - note->pressedOnTime < attackDuration + decayDuration) {
                double ampAtRelease =
                        initialAmplitude - (note->releasedOnTime - note->pressedOnTime - attackDuration) / decayDuration
                                           * (initialAmplitude - sustainAmplitude);
                return ampAtRelease - ampAtRelease * ((globalTime - note->releasedOnTime) / releaseDuration);
            } else {
                return sustainAmplitude - sustainAmplitude * ((globalTime - note->releasedOnTime) / releaseDuration);
            }
        } else {
            note->isAudible = false;
            return 0.0;
        }
    }
};

#endif //MUGEN_ENVELOPE_H
