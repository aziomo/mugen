//
// Created by alberto on 12/25/21.
//

#ifndef MUGEN_ITEMLIST_H
#define MUGEN_ITEMLIST_H

#include "Texture.h"
#include <vector>
#include <string>

using std::string;
using std::vector;


class ItemList {

public:
    ItemList(SDL_Renderer* renderer, TTF_Font* font, int width, int height, int visibleItems)
    {
        this->renderer = renderer;
        this->w = width;
        this->h = height;
        this->font = font;
        this->visibleItems = visibleItems;
        itemWidth = width;
        itemHeight = height / visibleItems;
        selectedIndex = 0;
        topDisplayedItemIndex = 0;
    };

    void setArrowTexture(Texture* texture){
        arrowTexture = texture;
    }


    int w, h, itemWidth, itemHeight;

    int selectedIndex, topDisplayedItemIndex;
    int visibleItems = 4;

    TTF_Font* font;
    SDL_Color whiteColor = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Color blackColor = {0x00, 0x00, 0x00, 0xFF};
    SDL_Renderer* renderer;
    Texture* arrowTexture;

    vector<string> items;
    vector<Texture*> renderedStrings;
    vector<Texture*> renderedBlackStrings;
    vector<SDL_Rect> itemContainers;


    void addItem(string item){
        items.push_back(item);
        auto* textureW = new Texture();
        auto* textureB = new Texture();

        textureW->loadFromText(renderer, item, whiteColor, font);
        textureB->loadFromText(renderer, item, blackColor, font);

        renderedStrings.push_back(textureW);
        renderedBlackStrings.push_back(textureB);
        updateItemContainers();
    }

    void removeItem(int index){
        if (index < items.size()){
            items.erase(items.begin() + index);
            renderedStrings.at(index)->free();
            renderedStrings.erase(renderedStrings.begin() + index);
            renderedBlackStrings.at(index)->free();
            renderedBlackStrings.erase(renderedBlackStrings.begin() + index);
        }

        updateItemContainers();
    }

    void updateItemContainers(){
        while (!itemContainers.empty()){
            itemContainers.pop_back();
        }
        for (int i = 0; i < items.size(); i++){
            SDL_Rect container={0,0, itemWidth, itemHeight};
            itemContainers.push_back(container);
        }
    }

    void moveUp(){
        if (selectedIndex > 0)
            selectedIndex--;

        if (selectedIndex < topDisplayedItemIndex)
            topDisplayedItemIndex--;

    }

    void moveDown(){
        if (selectedIndex < items.size()-1)
            selectedIndex++;

        if (selectedIndex == topDisplayedItemIndex + visibleItems)
            topDisplayedItemIndex++;
    }


    void render(int x, int y){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        SDL_Rect containerBorder = {x, y, w, h};
        SDL_RenderDrawRect(renderer, &containerBorder);
        for (int i = topDisplayedItemIndex; i < topDisplayedItemIndex + visibleItems; i++){
            if (i == items.size()){
                break;
            }
            auto itemContainer = &itemContainers.at(i);
            itemContainer->x = x;
            itemContainer->y = y + itemHeight * (i - topDisplayedItemIndex);
            if (i == selectedIndex){
                SDL_RenderFillRect(renderer, itemContainer);
                auto blackItemString = renderedBlackStrings.at(i);
                blackItemString->render(itemContainer->x + itemContainer->w/2 - blackItemString->w/2,
                                   itemContainer->y + itemContainer->h/2 - blackItemString->h/2);
            } else {
                SDL_RenderDrawRect(renderer, itemContainer);
                auto itemString = renderedStrings.at(i);
                itemString->render(itemContainer->x + itemContainer->w/2 - itemString->w/2,
                                   itemContainer->y + itemContainer->h/2 - itemString->h/2);
            }

        }
    }






};


#endif //MUGEN_ITEMLIST_H
