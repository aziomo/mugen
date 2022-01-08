#ifndef MUGEN_SEGMENTMANAGER_H
#define MUGEN_SEGMENTMANAGER_H

#include "Timeline.h"

class SegmentManager {
public:
    SegmentManager(Timeline* timeline, int width, int height, int visibleItems){
        this->timeline = timeline;
        this->renderer = timeline->renderer;
        this->width = width;
        this->height = height;
        this->visibleItems = visibleItems;
        topDisplayedItemIndex = 0;
    }

    SDL_Renderer* renderer;
    SDL_Rect outline;
    Timeline* timeline;
    int width, height, padding = 5;
    int topDisplayedItemIndex, visibleItems;

    void render(int x, int y){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        outline = {x, y, width, height};
        SDL_RenderDrawRect(renderer, &outline);

        Texture segIndex, segNumber;


        for (int i = topDisplayedItemIndex; i < topDisplayedItemIndex + visibleItems; ++i) {
            if (i == timeline->songSegs.size()) break;

            setWhiteStr(&segIndex, getTwoDigitString(i));
            setWhiteStr(&segNumber, getTwoDigitString(timeline->songSegs.at(i)->index));

            SDL_Rect container = {x, y + padding + segIndex.h * (i - topDisplayedItemIndex),
                                  width, segIndex.h};

            segIndex.render(container.x + container.w/4 - segIndex.w/2, container.y);
            segNumber.render(container.x + container.w*3/4 - segNumber.w/2, container.y);

        }
    }

    void setBlackStr(Texture* texture, const string& text) const{
        timeline->setBlackStr(texture, text);
    }

    void setWhiteStr(Texture* texture, const string& text) const{
        timeline->setWhiteStr(texture, text);
    }

};


#endif //MUGEN_SEGMENTMANAGER_H
