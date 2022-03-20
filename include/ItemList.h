#ifndef MUGEN_ITEMLIST_H
#define MUGEN_ITEMLIST_H

#include "Texture.h"
#include <vector>
#include <string>

using std::vector;

class ItemList {

public:
    ItemList(SDL_Renderer* renderer, TTF_Font* font, int width, int height, int visibleItems, bool enumerate = false)
    {
        this->renderer = renderer;
        this->width = width;
        this->height = height;
        this->font = font;
        this->visibleItems = visibleItems;
        this->itemWidth = width;
        this->itemHeight = height / visibleItems;
        this->selectedIndex = -1;
        this->topDisplayedItemIndex = 0;
        this->isFocused = false;
        this->enumerate = enumerate;
    };

    ~ItemList(){
        renderer = nullptr;
        font = nullptr;
        items.clear();
        indexLabel.free();
        itemLabel.free();
    };

    bool enumerate;
    int width, height, itemWidth, itemHeight;
    int selectedIndex, topDisplayedItemIndex;
    int visibleItems = 4;

    TTF_Font* font;
    SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Color black = {0x00, 0x00, 0x00, 0xFF};
    SDL_Renderer* renderer;
    SDL_Rect outline;
    bool isFocused;

    vector<string> items;
    Texture indexLabel, itemLabel;

    void setBlackStr(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, black, font);
    }

    void setWhiteStr(Texture* texture, string text) const{
        texture->loadFromText(renderer, text, white, font);
    }

    void addItem(const string& item){
        items.push_back(item);
        auto* strTexture = new Texture();
        auto* indexTexture = new Texture();
        if (items.size() == 1){
            setBlackStr(strTexture, item);
            setBlackStr(indexTexture, getTwoDigitString(items.size()));
            selectedIndex = 0;
        } else {
            setWhiteStr(strTexture, item);
            setWhiteStr(indexTexture, getTwoDigitString(items.size()));
        }
    }

    void removeItem(int index){
        if (index < items.size())
            items.erase(items.begin() + index);
    }

    void moveUp(){
        if (!items.empty()){
            if (selectedIndex > 0)
                selectedIndex--;
            if (selectedIndex < topDisplayedItemIndex)
                topDisplayedItemIndex--;
        }
    }

    void moveDown(){
        if (!items.empty()) {
            if (selectedIndex < items.size() - 1)
                selectedIndex++;
            if (selectedIndex == topDisplayedItemIndex + visibleItems)
                topDisplayedItemIndex++;
        }
    }

    void setSelectedIndex(int newSelectedIndex){
        this->selectedIndex = newSelectedIndex;
    }

    string getSelectedItem(){
        return items.at(selectedIndex);
    }

    void render(int x, int y){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        outline = {x, y, width, height};
        SDL_RenderDrawRect(renderer, &outline);

        for (int i = topDisplayedItemIndex; i < topDisplayedItemIndex + visibleItems; i++){
            if (i == items.size()){
                break;
            }

            SDL_Rect itemContainer = {x, y + itemHeight * (i - topDisplayedItemIndex), itemWidth, itemHeight};

            if (i == selectedIndex){
                SDL_RenderFillRect(renderer, &itemContainer);
                if (isFocused){
                    setBlackStr(&itemLabel, items.at(i));
                    if (enumerate) setBlackStr(&indexLabel, getTwoDigitString(i));
                } else{
                    SDL_Rect containerFilling = {itemContainer.x+3, itemContainer.y+3, itemContainer.w-6, itemContainer.h-6};
                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                    SDL_RenderFillRect(renderer, &containerFilling);
                    setWhiteStr(&itemLabel, items.at(i));
                    if (enumerate) setWhiteStr(&indexLabel, getTwoDigitString(i));
                }
            }
            else {
                setWhiteStr(&itemLabel, items.at(i));
                if (enumerate) setWhiteStr(&indexLabel, getTwoDigitString(i));
            }

            if (enumerate){
                indexLabel.render(itemContainer.x + itemContainer.w * 0.1 - indexLabel.w/2,
                                  itemContainer.y + itemContainer.h/2 - itemLabel.h / 2 + 1);
                itemLabel.render(itemContainer.x + itemContainer.w * 0.55 - itemLabel.w/2,
                                 itemContainer.y + itemContainer.h/2 - itemLabel.h / 2 + 1);
            }
            else {
                itemLabel.render(itemContainer.x + itemContainer.w / 2 - itemLabel.w / 2,
                                 itemContainer.y + itemContainer.h/2 - itemLabel.h / 2 + 1);
            }

        }
    }
};


#endif //MUGEN_ITEMLIST_H
