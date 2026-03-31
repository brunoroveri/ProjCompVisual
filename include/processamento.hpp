#ifndef PROCESSAMENTO_HPP
#define PROCESSAMENTO_HPP

#include <SDL3/SDL.h>
#include <vector>
#include <cmath>

struct AppData {
    SDL_Surface *originalGray = nullptr;
    SDL_Surface *equalizedGray = nullptr;
    bool exibindoEqualizada = false;
    uint32_t histograma[256] = {0};
    uint32_t maxHist = 0;
    float media = 0;
    float desvio = 0;
};

void converterParaCinza(SDL_Surface* surface);
void calcularDados(SDL_Surface* surface, AppData& data);
SDL_Surface* criarEqualizada(SDL_Surface* original, uint32_t* hist);

#endif