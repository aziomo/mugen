#ifndef MUGEN_CPP_TEXTURE_H
#define MUGEN_CPP_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>


static std::string assets_dir = "/home/alberto/code/MUSOGEN/mugen_cpp/assets/";

class Texture {
public:
    Texture(){
        renderer = nullptr;
        texture = nullptr;
        width = 0;
        height = 0;
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
                width = loadedSurface->w;
                height = loadedSurface->h;
            }
            SDL_FreeSurface( loadedSurface );
        }
    }

    void loadFromText(SDL_Renderer* pRenderer, std::string textValue, SDL_Color textColor, TTF_Font* font){
        renderer = pRenderer;
        free();
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, textValue.c_str(), textColor);
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        width = textSurface->w;
        height = textSurface->h;
        SDL_FreeSurface(textSurface);
    }

    void render(int x, int y){
        SDL_Rect renderQuad = {x, y, width, height};
        SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
    }
    void free(){
        if (texture != nullptr){
            SDL_DestroyTexture(texture);
            texture = nullptr;
            width = 0;
            height = 0;
        }
    }

    int width;
    int height;
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif //MUGEN_CPP_TEXTURE_H