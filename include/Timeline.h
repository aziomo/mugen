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
        for (int i = 0; i < 100; i++){
            allSegs.push_back(new Segment(startingColCount, i));
        }
        songSegs.push_back(allSegs.front());
    }


    bool timelineFocused = false;
    int focusedSegmentIndex = 0;
    int focusedColInd = 0;
    int focusedBitIndex = 0;
    bool editingMode = false;


    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Color black = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color grey = {0x55, 0x55, 0x55, 0xFF};
    SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Rect outline;
    string dashdash = "--";

    int width, height;
    int colWidth = 45;
    int renderedCols;
    int tempo = 150;
    vector<Segment*> songSegs;
    vector<Segment*> allSegs;

    void render(int x, int y) {


        outline = {x, y, width - renderedCols + 1, height - 3};

        Texture bitInst, bitNote;

        bool leftHalfReady = false;
        bool rightHalfReady = false;

        int middleColX = (renderedCols / 2) * colWidth - renderedCols / 2;



        if (focusedSegmentIndex == 0) {
            leftHalfReady = true;
        }
        if (focusedSegmentIndex == songSegs.size()-1) {
            rightHalfReady = true;
        }


        int renderedSegmentIndex = focusedSegmentIndex - 1;

        int leftColumnsLeftToRender = renderedCols/2 - focusedColInd;
        if (leftColumnsLeftToRender < 0) {
            leftColumnsLeftToRender = 0;
        }
        int leftColumnsRendered = 0;

        SDL_SetRenderDrawColor(renderer, 0x44, 0x44, 0x44, 0xFF);

        while (!leftHalfReady){

            if (leftColumnsRendered == leftColumnsLeftToRender) break;
            if (leftColumnsRendered && leftColumnsRendered % segColumns() == 0){
                renderedSegmentIndex--;
                if (renderedSegmentIndex < 0) {
                    break;
                }
            }

            int renderedColumnIndex = segColumns()-1 - (leftColumnsRendered % segColumns());

            SDL_Rect colOutline = {x + middleColX - (focusedColInd * (colWidth-1)) - (leftColumnsRendered+1) * (colWidth-1),   y, colWidth, height};
            for (int j = 0; j < 5; j++){
                SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 1};
                SDL_RenderDrawRect(renderer, &bitOutline);

                auto renderedBit = songSegs.at(renderedSegmentIndex)->cols.at(renderedColumnIndex)->bits[j];

                if (renderedBit != nullptr){
                    setGreyStr(&bitNote, freqToSymbol(renderedBit->note.frequency));
                    setGreyStr(&bitInst, getTwoDigitString(renderedBit->instrument->index));
                }
                else {
                    setGreyStr(&bitNote, dashdash);
                    setGreyStr(&bitInst, dashdash);
                }
                bitNote.render(bitOutline.x + bitOutline.w/2 - bitNote.w/2, bitOutline.y + bitOutline.h/2 - bitNote.h);
                bitInst.render(bitOutline.x + bitOutline.w/2 - bitInst.w/2, bitOutline.y + bitOutline.h/2);

            }

            leftColumnsRendered++;
        }


        renderedSegmentIndex = focusedSegmentIndex + 1;

        int rightColumnsLeftToRender = (renderedCols/2)+1- (segColumns() - focusedColInd);
        if (rightColumnsLeftToRender > renderedCols/2) {
            rightColumnsLeftToRender = renderedCols/2;
        }
        int rightColumnsRendered = 0;

        while (!rightHalfReady){
            if (rightColumnsRendered == rightColumnsLeftToRender) break;
            if (rightColumnsRendered && rightColumnsRendered % segColumns() == 0){
                renderedSegmentIndex++;
                if (renderedSegmentIndex == songSegs.size()) {
                    break;
                }
            }

            int renderedColumnIndex = segColumns()-1 - (rightColumnsRendered % segColumns());

            SDL_Rect colOutline = {x + middleColX + (rightColumnsRendered+1) * (colWidth-1) + (segColumns()-1) * (colWidth-1) - (focusedColInd * (colWidth-1)),   y, colWidth, height};
            for (int j = 0; j < 5; j++){
                SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 1};
                SDL_RenderDrawRect(renderer, &bitOutline);

                auto renderedBit = songSegs.at(renderedSegmentIndex)->cols.at(renderedColumnIndex)->bits[j];

                if (renderedBit != nullptr){
                    setGreyStr(&bitNote, freqToSymbol(renderedBit->note.frequency));
                    setGreyStr(&bitInst, getTwoDigitString(renderedBit->instrument->index));
                }
                else {
                    setGreyStr(&bitNote, dashdash);
                    setGreyStr(&bitInst, dashdash);
                }
                bitNote.render(bitOutline.x + bitOutline.w/2 - bitNote.w/2, bitOutline.y + bitOutline.h/2 - bitNote.h);
                bitInst.render(bitOutline.x + bitOutline.w/2 - bitInst.w/2, bitOutline.y + bitOutline.h/2);

            }

            rightColumnsRendered++;
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawRect(renderer, &outline);

        for (int i = focusedColInd; i < segColumns(); i++){

            SDL_Rect colOutline = {x + middleColX + (i * (colWidth - 1)) - (focusedColInd * (colWidth - 1)), y, colWidth, height};


            for (int j = 0; j < 5; j++){
                SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j,
                                       colWidth, colOutline.h / 5 - 1};

                bool bitFocused = (focusedColInd == i && focusedBitIndex == j);

                if (timelineFocused && bitFocused){
                    if (!editingMode){
                        SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF); // grey
                    }
                    SDL_RenderFillRect(renderer, &bitOutline);
                    if (!editingMode){
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    }
                }

                SDL_RenderDrawRect(renderer, &bitOutline);
                auto renderedBit = songSegs.at(focusedSegmentIndex)->cols.at(i)->bits[j];

                if (renderedBit != nullptr){
                    if (bitFocused && editingMode){
                        setBlackStr(&bitNote, freqToSymbol(renderedBit->note.frequency));
                        setBlackStr(&bitInst, getTwoDigitString(renderedBit->instrument->index));
                    } else {
                        setWhiteStr(&bitNote, freqToSymbol(renderedBit->note.frequency));
                        setWhiteStr(&bitInst, getTwoDigitString(renderedBit->instrument->index));
                    }
                }
                else {
                    if (bitFocused && editingMode){
                        setBlackStr(&bitNote, dashdash);
                        setBlackStr(&bitInst, dashdash);
                    } else {
                        setWhiteStr(&bitNote, dashdash);
                        setWhiteStr(&bitInst, dashdash);
                    }
                }
                bitNote.render(bitOutline.x + bitOutline.w/2 - bitNote.w/2, bitOutline.y + bitOutline.h/2 - bitNote.h);
                bitInst.render(bitOutline.x + bitOutline.w/2 - bitInst.w/2, bitOutline.y + bitOutline.h/2);
            }

        }

        for (int i = focusedColInd - 1; i >= 0; i--){

            SDL_Rect colOutline = {x + middleColX + (i * (colWidth - 1)) - (focusedColInd * (colWidth - 1)), y, colWidth, height};

            for (int j = 0; j < 5; j++){
                SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j,
                                       colWidth, colOutline.h / 5 - 1};

                bool bitFocused = (focusedColInd == i && focusedBitIndex == j);
                if (timelineFocused && bitFocused){
                    if (!editingMode){
                        SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF); // grey
                    }
                    SDL_RenderFillRect(renderer, &bitOutline);
                    if (!editingMode){
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    }
                }
                SDL_RenderDrawRect(renderer, &bitOutline);
                auto renderedBit = songSegs.at(focusedSegmentIndex)->cols.at(i)->bits[j];

                if (renderedBit != nullptr){
                    if (bitFocused && editingMode){
                        setBlackStr(&bitNote, freqToSymbol(renderedBit->note.frequency));
                        setBlackStr(&bitInst, getTwoDigitString(renderedBit->instrument->index));
                    } else {
                        setWhiteStr(&bitNote, freqToSymbol(renderedBit->note.frequency));
                        setWhiteStr(&bitInst, getTwoDigitString(renderedBit->instrument->index));
                    }
                }
                else {
                    if (bitFocused && editingMode){
                        setBlackStr(&bitNote, dashdash);
                        setBlackStr(&bitInst, dashdash);
                    } else {
                        setWhiteStr(&bitNote, dashdash);
                        setWhiteStr(&bitInst, dashdash);
                    }

                }

                bitNote.render(bitOutline.x + bitOutline.w/2 - bitNote.w/2, bitOutline.y + bitOutline.h/2 - bitNote.h);
                bitInst.render(bitOutline.x + bitOutline.w/2 - bitInst.w/2, bitOutline.y + bitOutline.h/2);
            }
        }



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
                if (focusedColInd > 0){
                    focusedColInd--;
                } else if (focusedSegmentIndex > 0) {
                    focusedSegmentIndex--;
                    focusedColInd = segColumns() - 1;
                }
                break;
            case Direction::RIGHT:
                if (focusedColInd < segColumns() - 1){
                    focusedColInd++;
                }
                else if (focusedSegmentIndex < songSegs.size() - 1){
                    focusedSegmentIndex++;
                    focusedColInd = 0;
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
