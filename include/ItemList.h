//
// Created by alberto on 12/25/21.
//

#ifndef MUGEN_ITEMLIST_H
#define MUGEN_ITEMLIST_H

#include "Texture.h"
#include <vector>
#include <string>

using std::string;
using std::to_string;
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
        selectedIndex = -1;
        topDisplayedItemIndex = 0;
    };

    void setArrowTexture(Texture* texture){
        arrowTexture = texture;
    }


    int w, h, itemWidth, itemHeight;

    bool enumerate = false;

    int selectedIndex, topDisplayedItemIndex;
    int visibleItems = 4;

    TTF_Font* font;
    SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Color black = {0x00, 0x00, 0x00, 0xFF};
    SDL_Renderer* renderer;
    Texture* arrowTexture;

    vector<string> items;
    vector<Texture*> renderedStrings;
    vector<Texture*> renderedIndices;
    vector<SDL_Rect> itemContainers;

    void setBlackStr(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, black, font);
    }

    void setWhiteStr(Texture* texture, string text) const{
        texture->loadFromText(renderer, text, white, font);
    }



    void addItem(string item){
        items.push_back(item);
        auto* strTexture = new Texture();
        auto* indexTexture = new Texture();
        if (items.size() == 1){
            strTexture->loadFromText(renderer, item, black, font);
            indexTexture->loadFromText(renderer, getTwoDigitString(items.size()), black, font);
            selectedIndex = 0;
        } else {
            strTexture->loadFromText(renderer, item, white, font);
            indexTexture->loadFromText(renderer, getTwoDigitString(items.size()), white, font);
        }
        renderedStrings.push_back(strTexture);
        renderedIndices.push_back(indexTexture);
        updateItemContainers();
    }

    void removeItem(int index){
        if (index < items.size()){
            items.erase(items.begin() + index);
            renderedStrings.at(index)->free();
            renderedStrings.erase(renderedStrings.begin() + index);
            renderedIndices.at(index)->free();
            renderedIndices.erase(renderedIndices.begin() + index);
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
        if (selectedIndex > 0){
            setWhiteStr(renderedStrings.at(selectedIndex), items.at(selectedIndex));
            setWhiteStr(renderedIndices.at(selectedIndex), items.at(selectedIndex));
            selectedIndex--;
            setBlackStr(renderedStrings.at(selectedIndex), items.at(selectedIndex));
            setBlackStr(renderedIndices.at(selectedIndex), items.at(selectedIndex));
        }

        if (selectedIndex < topDisplayedItemIndex)
            topDisplayedItemIndex--;
    }

    void moveDown(){
        if (selectedIndex < items.size()-1){
            setWhiteStr(renderedStrings.at(selectedIndex), items.at(selectedIndex));
            setWhiteStr(renderedIndices.at(selectedIndex), items.at(selectedIndex));
            selectedIndex++;
            setBlackStr(renderedStrings.at(selectedIndex), items.at(selectedIndex));
            setBlackStr(renderedIndices.at(selectedIndex), items.at(selectedIndex));
        }

        if (selectedIndex == topDisplayedItemIndex + visibleItems)
            topDisplayedItemIndex++;
    }

    void setSelectedIndex(int newSelectedIndex){
        if (!items.empty()){
            setWhiteStr(renderedStrings.at(selectedIndex), items.at(selectedIndex));
        }
        this->selectedIndex = newSelectedIndex;
        setBlackStr(renderedStrings.at(selectedIndex), items.at(selectedIndex));
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
            } else {
                SDL_RenderDrawRect(renderer, itemContainer);
            }
            auto itemString = renderedStrings.at(i);
            if (enumerate){
                auto itemIndex = renderedIndices.at(i);
                itemIndex->render(itemContainer->x + itemIndex->w/2,
                                  itemContainer->y + itemContainer->h/2 - itemString->h/2);

                itemString->render(itemContainer->x + itemIndex->w + (itemContainer->w - itemIndex->w) / 2 - itemString->w/2,
                                   itemContainer->y + itemContainer->h/2 - itemString->h/2);
            }
            else {
                itemString->render(itemContainer->x + itemContainer->w/2 - itemString->w/2,
                                   itemContainer->y + itemContainer->h/2 - itemString->h/2);
            }

        }
    }



};


#endif //MUGEN_ITEMLIST_H
