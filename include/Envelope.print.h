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
    double pressedAt = 0.0;
    double releasedAt = 0.0;
    double frequency;
};

struct Envelope {
  double iAmp, sAmp, aDur, dDur, rDur;

  double getAmplifier(double gTime, Note *note) const {
    if (note->pressedAt > note->releasedAt) {
      if (gTime >= note->pressedAt && gTime < note->pressedAt + aDur) {
        return ((gTime - note->pressedAt) / aDur) * iAmp;
      } else if (gTime < note->pressedAt + aDur + dDur) {
        return iAmp - (gTime - note->pressedAt - aDur) / dDur
               * (iAmp - sAmp);
      } else {
        return sAmp;
      }
    } else if (gTime < note->releasedAt + rDur) {
      if (note->releasedAt - note->pressedAt < aDur) {
        double ampAtRelease =
          ((note->releasedAt - note->pressedAt) / aDur) * iAmp;
        return ampAtRelease - ampAtRelease * ((gTime - note->releasedAt) / rDur);
      } else if (note->releasedAt - note->pressedAt < aDur + dDur) {
        double ampAtRelease =
          iAmp - (note->releasedAt - note->pressedAt - aDur) / dDur
          * (iAmp - sAmp);
        return ampAtRelease - ampAtRelease * ((gTime - note->releasedAt) / rDur);
      } else {
        return sAmp - sAmp * ((gTime - note->releasedAt) / rDur);
      }
    } else {
      note->isAudible = false;
      return 0.0;
    }
  }
};

#endif //MUGEN_ENVELOPE_H
