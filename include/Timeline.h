//
// Created by alberto on 12/27/21.
//

#ifndef MUGEN_TIMELINE_H
#define MUGEN_TIMELINE_H

#include "Segment.h"
#include "Texture.h"

using std::string;
using std::to_string;

class Timeline {
public:
    Timeline(SDL_Renderer* renderer, TTF_Font* font, int width, int height, int startingSegments){
        this->renderer = renderer;
        this->bitFont = font;
        this->width = width;
        this->height = height;
        segments.emplace_back(startingSegments);
    }

    TTF_Font* bitFont;

    SDL_Color fontColor = {255,255,255};

    SDL_Renderer* renderer;

    int width, height;

    int rows, cols;

    int rowWidth = 40;

    int tempo = 150;

    vector<Segment> segments;

    void setTextTexture(Texture* texture, string text) const {
        texture->loadFromText(renderer, text, fontColor, bitFont);
    }


    void render(int x, int y){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_Rect outline = {x, y, width, height};
        SDL_RenderDrawRect(renderer, &outline);

        for (int i = 0; i < segments.front().rows.size(); i++){

            SDL_Rect rowOutline = {x + i * (rowWidth - 1) , y, rowWidth, height};

            for (int j = 0; j < 5; j++){
                SDL_Rect bitOutline = {rowOutline.x, rowOutline.y + (rowOutline.h / 5) * j,
                                       rowWidth, rowOutline.h / 5 - 1};
                SDL_RenderDrawRect(renderer, &bitOutline);

                Texture bitInst, bitNote;

                if (segments.front().rows.at(i).bits[j] != nullptr){
                    setTextTexture(&bitNote, to_string(freqToMidi(segments.front().rows.at(i).bits[j]->note.frequency)));
                    setTextTexture(&bitInst, getTwoDigitString(segments.front().rows.at(i).bits[j]->instrument->index));
                }
                else {
                    setTextTexture(&bitNote, "--");
                    setTextTexture(&bitInst, "--");
                }
                bitNote.render(bitOutline.x, bitOutline.y);
                bitInst.render(bitOutline.x, bitOutline.y + bitNote.h);

            }
        }

    };

};


#endif //MUGEN_TIMELINE_H
