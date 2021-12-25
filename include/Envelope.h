//
// Created by alberto on 12/19/21.
//

#ifndef MUGEN_ENVELOPE_H
#define MUGEN_ENVELOPE_H

struct Note {
    bool isPressed;
    bool isPlaying;
    double pressedOnTime;
    double releasedOnTime;
    double frequency;
};

struct Envelope {
    double initialAmplitude;
    double sustainAmplitude;
    double attackDuration;
    double decayDuration;
    double releaseDuration;

    double getAmplifier(double globalTime, Note *note) const {
        if (note->isPressed) {
            if (globalTime >= note->pressedOnTime && globalTime < note->pressedOnTime + attackDuration) {
                note->isPlaying = true;
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
            note->isPlaying = false;
            return 0.0;
        }
    }
};

#endif //MUGEN_ENVELOPE_H
