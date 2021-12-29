//
// Created by alberto on 12/27/21.
//

#ifndef MUGEN_TIMELINE_H
#define MUGEN_TIMELINE_H

#include "Segment.h"
#include "Texture.h"


class Timeline {
public:
    Timeline(SDL_Renderer* renderer, int width, int height){
        this->renderer = renderer;
        this->width = width;
        this->height = height;
        segments.push_back(Segment());
    }

    SDL_Renderer* renderer;

    int width, height;

    int rows, cols;

    int rowWidth = 40;

    int tempo = 150;

    vector<Segment> segments;

    void render(int x, int y){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_Rect outline = {x, y, width, height};
        SDL_RenderDrawRect(renderer, &outline);

        for (int i = 0; i < segments.front().rows.size(); i++){
            SDL_Rect rowOutline = {x + i * (rowWidth - 1) , y, rowWidth, height};
//            SDL_RenderDrawRect(renderer, &rowOutline);
            for (int j = 0; j < 5; j++){
                SDL_Rect bitOutline = {rowOutline.x, rowOutline.y + (rowOutline.h / 5) * j,
                                       rowWidth, rowOutline.h / 5 - 1};
                SDL_RenderDrawRect(renderer, &bitOutline);
            }
        }


    };






};


#endif //MUGEN_TIMELINE_H
