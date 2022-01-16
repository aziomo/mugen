#include "../include/Serializer.h"



JSON Serializer::extractProject(MusicBox* musicBox, Timeline* timeline){
    JSON projectJson;
    projectJson["instruments"] = extractInstruments(musicBox);
    projectJson["composition"] = extractComposition(timeline);

    return projectJson;
}

JSON Serializer::extractComposition(Timeline* timeline){
    JSON compositionJson;
    compositionJson["tempo"] = timeline->tempo;
    compositionJson["columns"] = timeline->segColumns();
    compositionJson["song_segments"] = extractSongSegmentsIndices(timeline);
    compositionJson["all_segments"] = extractAllSegments(timeline);
    return compositionJson;
}

JSON Serializer::extractSongSegmentsIndices(Timeline* timeline){
    JSON segIndicesJson;
    for (auto* songSeg : timeline->songSegs){
        segIndicesJson.push_back(songSeg->index);
    }
    return segIndicesJson;
}

JSON Serializer::extractAllSegments(Timeline* timeline){
    JSON allSegsJson;
    for (auto* segment : timeline->allSegs){
        allSegsJson.push_back(extractSegment(segment));
    }
    return allSegsJson;
}

JSON Serializer::extractSegment(Segment* segment){
    JSON segmentJson;
    bool onlyNulls = true;
    for (auto* col : segment->cols){
        auto extractedColumn = extractColumn(col);
        if (extractedColumn != nullptr)
            onlyNulls = false;
        segmentJson["cols"].push_back(extractColumn(col));
    }
    if (onlyNulls)
        segmentJson = nullptr;
    return segmentJson;
}

JSON Serializer::extractColumn(Column* column){
    for (int i = 0; i < 5; i++){
        if (column->bits[i] != nullptr){
            break;
        }
        if (i == 4)
            return nullptr;
    }
    JSON columnJson;
    for (auto* bit : column->bits){
        if (bit == nullptr) {
            columnJson.push_back(nullptr);
        }
        else {
            JSON bitJson;
            bitJson["instrument"] = bit->instrument->index;
            bitJson["note_value"] = freqToMidi(bit->note.frequency);
            bitJson["hold_duration"] = bit->holdDuration;
            bitJson["hold_section"] = bit->holdSection;
            columnJson.push_back(bitJson);
        }
    }
    return columnJson;
}



JSON Serializer::extractInstruments(MusicBox* musicBox) {
    JSON instrumentsJson;
    for (auto* instrument : musicBox->instruments){
        JSON instJson;
        instJson["index"] = instrument->index;
        instJson["envelope"] = extractEnvelope(instrument);
        instJson["oscillators"] = extractOscillators(instrument);
        instrumentsJson.push_back(instJson);
    }
    return instrumentsJson;
}

JSON Serializer::extractEnvelope(Instrument* instrument){
    JSON envJson;
    envJson["initial"] = instrument->env.initialAmplitude;
    envJson["sustain"] = instrument->env.sustainAmplitude;
    envJson["attack"] = instrument->env.attackDuration;
    envJson["decay"] = instrument->env.decayDuration;
    envJson["release"] = instrument->env.releaseDuration;
    return envJson;
}

JSON Serializer::extractOscillators(Instrument *pInstrument) {
    JSON allOscsJson;
    for (auto* osc : pInstrument->oscillators){
        JSON singleOscJson;
        singleOscJson["type"] = wavetypeToString(osc->waveType);
        singleOscJson["freq_mod"] = osc->freqModifier;
        singleOscJson["amp_mod"] = osc->ampModifier;
        singleOscJson["lfo"] = extractLFO(osc->lfo);
        allOscsJson.push_back(singleOscJson);
    }
    return allOscsJson;
}

JSON Serializer::extractLFO(Oscillator* osc){
    if (osc == nullptr){
        return nullptr;
    }
    JSON lfoJson;
    lfoJson["type"] = wavetypeToString(osc->waveType);
    lfoJson["freq_mod"] = osc->freqModifier;
    lfoJson["amp_mod"] = osc->ampModifier;
    return lfoJson;
}