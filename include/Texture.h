#ifndef MUGEN_CPP_TEXTURE_H
#define MUGEN_CPP_TEXTURE_H



#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>

typedef int Alignment;

constexpr Alignment CENTER = 1, TO_LEFT = 0, TO_RIGHT = 2, TO_TOP = 0, TO_BOTTOM = 2;

static std::string assets_dir = "../assets/";

static std::string getTwoDigitString(int number){
    if (number < 0 || number > 99)
        return "XX";
    if (number < 10)
        return "0"+ std::to_string(number);
    return std::to_string(number);
}

class Texture {
public:
    Texture(){
        renderer = nullptr;
        texture = nullptr;
        w = 0;
        h = 0;
    }

    ~Texture(){
        free();
    }

    void loadFromFile(SDL_Renderer* pRenderer, std::string path){
        renderer = pRenderer;
        free();
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if( loadedSurface == nullptr ){
            printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        }
        else
        {
            texture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
            if( texture == nullptr ) {
                printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }
            else {
                w = loadedSurface->w;
                h = loadedSurface->h;
            }
            SDL_FreeSurface( loadedSurface );
        }
    }

    void loadFromText(SDL_Renderer* pRenderer, const std::string& textValue, SDL_Color textColor, TTF_Font* font){
        renderer = pRenderer;
        free();
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, textValue.c_str(), textColor);
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        w = textSurface->w;
        h = textSurface->h;
        SDL_FreeSurface(textSurface);
    }

    void render(int x, int y){
        SDL_Rect renderQuad = {x, y, w, h};
        SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
    }

    void renderRotated(int x, int y, double angle){
        SDL_Rect renderQuad = {x, y, w, h};
        SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, angle, nullptr, SDL_FLIP_NONE);
    }

    void free(){
        if (texture != nullptr){
            SDL_DestroyTexture(texture);
            texture = nullptr;
            w = 0;
            h = 0;
        }
    }

    int w;
    int h;
    SDL_Texture* texture;
private:
    SDL_Renderer* renderer;
};

#endif //MUGEN_CPP_TEXTURE_H