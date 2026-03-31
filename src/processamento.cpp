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


 // 3. Cálculo da Variância e Desvio Padrão 
    //variancia em zero
    double variancia = 0;
    for(int i=0; i<256; i++) {
        if(data.histograma[i] > data.maxHist) data.maxHist = data.histograma[i];
        variancia += data.histograma[i] * pow(i - data.media, 2);
    }
    data.desvio = sqrt(variancia / total);

    std::string brilho = (data.media < 85) ? "Escura" : (data.media > 170) ? "Clara" : "Media";
    std::string contraste = (data.desvio < 30) ? "Baixo" : (data.desvio > 70) ? "Alto" : "Medio";

    // EXIBIÇÃO NO TERMINAL 
    std::cout << "\n========================================" << std::endl;
    std::cout << " ANALISE DA IMAGEM ATUAL " << std::endl;
    std::cout << "========================================" << std::endl;
    //media
    std::cout << "Media de Intensidade: " << data.media << " -> " << brilho << std::endl;
    //desvio
    std::cout << "Desvio Padrao:        " << data.desvio << " -> Contraste " << contraste << std::endl;
    std::cout << "========================================\n" << std::endl;
}

SDL_Surface* criarEqualizada(SDL_Surface* original, uint32_t* hist) {
    SDL_Surface* surf = SDL_DuplicateSurface(original);
    //total de pixel
    int total = surf->w * surf->h;
    //veto CDF
    float cdf[256], somaAcumulada = 0;
    
    // CDF com as frequências normalizadas do histograma
    for(int i=0; i<256; i++) {
        somaAcumulada += (float)hist[i] / total;
        cdf[i] = somaAcumulada;
    }

    //perciorre a nova imagem 
    for (int y = 0; y < surf->h; y++) {
        for (int x = 0; x < surf->w; x++) {
            uint8_t r, g, b, a;
            SDL_ReadSurfacePixel(surf, x, y, &r, &g, &b, &a);
            uint8_t novoTom = (uint8_t)(cdf[r] * 255);
            SDL_WriteSurfacePixel(surf, x, y, novoTom, novoTom, novoTom, a);
        }
    }
    return surf;
}
