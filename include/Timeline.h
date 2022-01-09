#ifndef MUGEN_TIMELINE_H
#define MUGEN_TIMELINE_H

#include <stdexcept>
#include "Segment.h"
#include "Texture.h"
#include "Direction.h"

using std::string;
using std::to_string;

class Timeline {
public:
    Timeline(SDL_Renderer* renderer, TTF_Font* font, int renderedCols, int colWidth, int height, int startingColCount){
        this->renderer = renderer;
        this->font = font;
        this->renderedCols = renderedCols;
        this->colWidth = colWidth;
        this->width = renderedCols * colWidth;
        this->height = height;
        middleColX = (renderedCols/2) * colWidth - renderedCols/2;
        for (int i = 0; i < 100; i++){
            allSegs.push_back(new Segment(startingColCount, i));
        }
        songSegs.push_back(allSegs.front());
    }

    bool timelineFocused = false;
    int focusedSegmentIndex = 0;
    int focusedColIndex = 0;
    int focusedBitIndex = 0;
    bool editingMode = false;

    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Color black = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color grey = {0x77, 0x77, 0x77, 0xFF};
    SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
    string dashdash = "--";
    SDL_Rect outline;

    int width, height;
    int colWidth = 45;
    int renderedCols;
    int tempo = 150;
    int middleColX;
    vector<Segment*> songSegs;
    vector<Segment*> allSegs;

    Texture bitInstrumentLabel, bitNoteLabel;

    void render(int x, int y) {
        outline = {x, y, width - renderedCols + 1, height - 3};
        SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0xFF);
        renderLeftUnfocusedSegments(x, y);
        renderRightUnfocusedSegments(x, y);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawRect(renderer, &outline);
        renderFocusedSegmentLeftSide(x, y);
        renderFocusedSegmentRightSide(x, y);
    }

    void renderLeftUnfocusedSegments(int x, int y){
        int focusedSegmentRenderedColumns = focusedColIndex;
        if (focusedSegmentRenderedColumns > renderedCols/2){
            return;
        }
        if (focusedSegmentIndex > 0){
            int renderedSegmentIndex = focusedSegmentIndex - 1;
            int leftColumnsLeftToRender = renderedCols/2 - focusedColIndex;
            int leftColumnsRendered = 0;

            while (true){
                if (leftColumnsRendered == leftColumnsLeftToRender) break;
                if (leftColumnsRendered && leftColumnsRendered % segColumns() == 0){
                    if (--renderedSegmentIndex < 0) break;
                }
                int renderedColumnIndex = segColumns()-1 - (leftColumnsRendered % segColumns());
                SDL_Rect colOutline = {x + middleColX - (focusedColIndex * (colWidth - 1)) - (leftColumnsRendered + 1) * (colWidth - 1), y, colWidth, height};

                for (int j = 0; j < 5; j++){
                    SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 1};
                    SDL_RenderDrawRect(renderer, &bitOutline);
                    auto renderedBit = songSegs.at(renderedSegmentIndex)->cols.at(renderedColumnIndex)->bits[j];
                    setBitLabels(renderedBit, false);
                    renderBitLabels(&bitOutline);
                }

                leftColumnsRendered++;
            }
        }
    }

    void renderRightUnfocusedSegments(int x, int y){

        int focusedSegmentRenderedColumns = segColumns() - focusedColIndex;
        if (focusedSegmentRenderedColumns > renderedCols/2){
            return;
        }
        if (focusedSegmentIndex < songSegs.size()-1)
        {
            int rightColumnsLeftToRender = (renderedCols/2)+1- (focusedSegmentRenderedColumns);
            int rightColumnsRendered = 0;
            int renderedSegmentIndex = focusedSegmentIndex + 1;

            while (true){

                if (rightColumnsRendered == rightColumnsLeftToRender) break;
                if (rightColumnsRendered && rightColumnsRendered % segColumns() == 0){
                    if (++renderedSegmentIndex == songSegs.size()) break;
                }
                int renderedColumnIndex = rightColumnsRendered % segColumns();
                SDL_Rect colOutline = {x + middleColX + (rightColumnsRendered+1) * (colWidth-1) + (segColumns()-1) * (colWidth-1) - (focusedColIndex * (colWidth - 1)), y, colWidth, height};

                for (int j = 0; j < 5; j++) {
                    SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 1};
                    SDL_RenderDrawRect(renderer, &bitOutline);
                    auto renderedBit = songSegs.at(renderedSegmentIndex)->cols.at(renderedColumnIndex)->bits[j];
                    setBitLabels(renderedBit, false);
                    renderBitLabels(&bitOutline);
                }

                rightColumnsRendered++;
            }
        }
    }


    void renderFocusedSegmentLeftSide(int x, int y){
        int columnsLeftToRender = std::min(focusedColIndex+1, renderedCols/2);
        int columnsRendered = 0;

        for (int i = focusedColIndex - 1; i >= 0; i--){
            if (columnsRendered == columnsLeftToRender) break;
            SDL_Rect colOutline = {x + middleColX + (i * (colWidth - 1)) - (focusedColIndex * (colWidth - 1)), y, colWidth, height};
            for (int j = 0; j < 5; j++){
                SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 1};
                bool bitFocused = (focusedColIndex == i && focusedBitIndex == j);
                if (timelineFocused && bitFocused){
                    renderBitBackground(&bitOutline);
                }
                SDL_RenderDrawRect(renderer, &bitOutline);
                auto renderedBit = songSegs.at(focusedSegmentIndex)->cols.at(i)->bits[j];
                setBitLabels(renderedBit, true, bitFocused);
                renderBitLabels(&bitOutline);
            }
            columnsRendered++;
        }
    }

    void renderFocusedSegmentRightSide(int x, int y){
        int columnsLeftToRender = std::min((segColumns() - focusedColIndex), (renderedCols/2 + 1));
        int columnsRendered = 0;

        for (int i = focusedColIndex; i < segColumns(); i++){
            if (columnsRendered == columnsLeftToRender) break;
            SDL_Rect colOutline = {x + middleColX + (i * (colWidth - 1)) - (focusedColIndex * (colWidth - 1)), y, colWidth, height};
            for (int j = 0; j < 5; j++){
                SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 1};
                bool bitFocused = (focusedColIndex == i && focusedBitIndex == j);
                if (timelineFocused && bitFocused){
                    renderBitBackground(&bitOutline);
                }
                SDL_RenderDrawRect(renderer, &bitOutline);
                auto renderedBit = songSegs.at(focusedSegmentIndex)->cols.at(i)->bits[j];
                setBitLabels(renderedBit, true, bitFocused);
                renderBitLabels(&bitOutline);
            }
            columnsRendered++;
        }
    }

    void renderBitBackground(SDL_Rect* bitOutline) const{
        if (!editingMode){
            SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF); // grey
        }
        SDL_RenderFillRect(renderer, bitOutline);
        if (!editingMode){
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        }
    }

    void setBitLabels(Bit* renderedBit, bool segmentFocused, bool bitFocused = false){
        if (segmentFocused) {
            if (renderedBit != nullptr){
                if (bitFocused && editingMode){
                    setBlackStr(&bitNoteLabel, freqToSymbol(renderedBit->note.frequency));
                    setBlackStr(&bitInstrumentLabel, getTwoDigitString(renderedBit->instrument->index));
                } else {
                    setWhiteStr(&bitNoteLabel, freqToSymbol(renderedBit->note.frequency));
                    setWhiteStr(&bitInstrumentLabel, getTwoDigitString(renderedBit->instrument->index));
                }
            } else {
                if (bitFocused && editingMode){
                    setBlackStr(&bitNoteLabel, dashdash);
                    setBlackStr(&bitInstrumentLabel, dashdash);
                } else {
                    setWhiteStr(&bitNoteLabel, dashdash);
                    setWhiteStr(&bitInstrumentLabel, dashdash);
                }
            }
        } else {
            if (renderedBit != nullptr){
                setGreyStr(&bitNoteLabel, freqToSymbol(renderedBit->note.frequency));
                setGreyStr(&bitInstrumentLabel, getTwoDigitString(renderedBit->instrument->index));
            } else {
                setGreyStr(&bitNoteLabel, dashdash);
                setGreyStr(&bitInstrumentLabel, dashdash);
            }
        }
    }

    void renderBitLabels(SDL_Rect* bitOutline) {
        bitNoteLabel.render(bitOutline->x + bitOutline->w / 2 - bitNoteLabel.w / 2, bitOutline->y + bitOutline->h / 2 - bitNoteLabel.h);
        bitInstrumentLabel.render(bitOutline->x + bitOutline->w / 2 - bitInstrumentLabel.w / 2, bitOutline->y + bitOutline->h / 2);
    }

    void setBlackStr(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, black, font);
    }

    void setGreyStr(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, grey, font);
    }

    void setWhiteStr(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, white, font);
    }

    void move(Direction direction){
        switch (direction) {
            case Direction::UP:
                if (focusedBitIndex > 0)
                    focusedBitIndex--;
                break;
            case Direction::DOWN:
                if (focusedBitIndex < 4)
                    focusedBitIndex++;
                break;
            case Direction::LEFT:
                if (focusedColIndex > 0){
                    focusedColIndex--;
                } else if (focusedSegmentIndex > 0) {
                    focusedSegmentIndex--;
                    focusedColIndex = segColumns() - 1;
                }
                break;
            case Direction::RIGHT:
                if (focusedColIndex < segColumns() - 1){
                    focusedColIndex++;
                }
                else if (focusedSegmentIndex < songSegs.size() - 1){
                    focusedSegmentIndex++;
                    focusedColIndex = 0;
                }
                break;
        }
    }

    int segColumns(){
        return static_cast<int>(allSegs.front()->cols.size());
    }

    static string freqToSymbol(double frequency){
        int midi = freqToMidi(frequency);
        return midiToSymbol(midi);
    }

    static string midiToSymbol(int midinote){

        int octave = midinote / 12;
        int note = midinote % 12;

        switch (note){
            case 0:
                return "C" + to_string(octave);
            case 1:
                return "C#" + to_string(octave);
            case 2:
                return "D" + to_string(octave);
            case 3:
                return "D#" + to_string(octave);
            case 4:
                return "E" + to_string(octave);
            case 5:
                return "F" + to_string(octave);
            case 6:
                return "F#" + to_string(octave);
            case 7:
                return "G" + to_string(octave);
            case 8:
                return "G#" + to_string(octave);
            case 9:
                return "A" + to_string(octave);
            case 10:
                return "A#" + to_string(octave);
            case 11:
                return "B" + to_string(octave);
            default:
                return "NaN";
        }
    }

};


#endif //MUGEN_TIMELINE_H
