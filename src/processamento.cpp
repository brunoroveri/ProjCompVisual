#include "processamento.hpp"
#include <iostream>
#include <string>

void converterParaCinza(SDL_Surface* surface) {
    //linhas
    for (int y = 0; y < surface->h; y++) {
        //colunas
    for (int x = 0; x < surface->w; x++) {
            uint8_t r, g, b, a;
            SDL_ReadSurfacePixel(surface, x, y, &r, &g, &b, &a);
            uint8_t gray = (uint8_t)(0.2125f * r + 0.7154f * g + 0.0721f * b);
            SDL_WriteSurfacePixel(surface, x, y, gray, gray, gray, a);
        }
    }
}
// calcula histograma
void calcularDados(SDL_Surface* surface, AppData& data) {
    for(int i=0; i<256; i++) data.histograma[i] = 0;
    data.maxHist = 0;
    double soma = 0;
    //total de pixel
    int total = surface->w * surface->h;

 // 1. Contagem do Histograma e Soma para Média
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            uint8_t r, g, b, a;
            SDL_ReadSurfacePixel(surface, x, y, &r, &g, &b, &a);
            data.histograma[r]++;
            soma += r;
        }
    }

    // 2. Cálculo da Média 
    data.media = (float)(soma / total);

