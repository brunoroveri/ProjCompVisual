#include "processamento.hpp"
#include <iostream>
#include <string>

void converterParaCinza(SDL_Surface* surface) {
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            uint8_t r, g, b, a;
            SDL_ReadSurfacePixel(surface, x, y, &r, &g, &b, &a);
            uint8_t gray = (uint8_t)(0.2125f * r + 0.7154f * g + 0.0721f * b);
            SDL_WriteSurfacePixel(surface, x, y, gray, gray, gray, a);
        }
    }


