#ifndef MUGEN_SDL_UTILS_H
#define MUGEN_SDL_UTILS_H

#include <SDL2/SDL.h>

struct ColorTheme {
    SDL_Color background, foreground, inactiveText, inactiveBorder, focusedBit;
};

static void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

#endif //MUGEN_SDL_UTILS_H
