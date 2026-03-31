#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "processamento.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <caminho_da_imagem>" << std::endl;
        return -1;
    }

    // Inicialização da SDL3 e SDL_image
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Erro ao iniciar SDL: %s", SDL_GetError());
        return -1;
    }

    AppData data;
    SDL_Surface* carregada = IMG_Load(argv[1]);
    if (!carregada) {
        SDL_Log("Erro ao carregar imagem: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    converterParaCinza(carregada);
    data.originalGray = carregada;
    
    calcularDados(data.originalGray, data);
    
    data.equalizedGray = criarEqualizada(data.originalGray, data.histograma);

    // --- CONFIGURAÇÃO DAS JANELAS ---
    SDL_Window* winMain = SDL_CreateWindow("Principal (Pressione S para Salvar)", data.originalGray->w, data.originalGray->h, 0);
    SDL_SetWindowPosition(winMain, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_Renderer* renMain = SDL_CreateRenderer(winMain, NULL);

    int mx, my; 
    SDL_GetWindowPosition(winMain, &mx, &my);
    SDL_Window* winSec = SDL_CreateWindow("Histograma e Controles", 400, 450, 0);
    SDL_SetWindowPosition(winSec, mx + data.originalGray->w + 10, my);
    SDL_SetWindowParent(winSec, winMain); 
    SDL_Renderer* renSec = SDL_CreateRenderer(winSec, NULL);

    bool running = true;
    SDL_Event ev;
    SDL_FRect btn = {100, 380, 200, 40};
    int btnState = 0;

    // --- LOOP PRINCIPAL ---
    while (running) {
        float mouseX, mouseY;
        SDL_GetGlobalMouseState(&mouseX, &mouseY);
        int winX, winY;
        SDL_GetWindowPosition(winSec, &winX, &winY);
        float relX = mouseX - winX;
        float relY = mouseY - winY;
        bool hover = (relX >= btn.x && relX <= btn.x + btn.w && relY >= btn.y && relY <= btn.y + btn.h);

        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) running = false;

            if (ev.type == SDL_EVENT_KEY_DOWN && ev.key.key == SDLK_S) {
                SDL_Surface* pSalvar = data.exibindoEqualizada ? data.equalizedGray : data.originalGray;
                if (IMG_SavePNG(pSalvar, "output_image.png")) {
                    std::cout << "Imagem salva como output_image.png!" << std::endl;
                }
            }

            if (hover) {
                if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    btnState = 2;
                } else if (ev.type == SDL_EVENT_MOUSE_BUTTON_UP && btnState == 2) {
                    data.exibindoEqualizada = !data.exibindoEqualizada;
                    calcularDados(data.exibindoEqualizada ? data.equalizedGray : data.originalGray, data);
                    btnState = 1;
                } else if (btnState != 2) {
                    btnState = 1;
                }
            } else {
                btnState = 0;
            }
        }

        // --- RENDERIZAÇÃO JANELA PRINCIPAL ---
        SDL_Surface* atual = data.exibindoEqualizada ? data.equalizedGray : data.originalGray;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renMain, atual);
        SDL_RenderClear(renMain);
        SDL_RenderTexture(renMain, tex, NULL, NULL);
        SDL_RenderPresent(renMain);
        SDL_DestroyTexture(tex);

        // --- RENDERIZAÇÃO JANELA SECUNDÁRIA ---
        SDL_SetRenderDrawColor(renSec, 35, 35, 35, 255);
        SDL_RenderClear(renSec);
        
        // Desenha o Histograma
        SDL_SetRenderDrawColor(renSec, 255, 255, 255, 255);
        for(int i = 0; i < 256; i++) {
            if(data.maxHist > 0) {
                int h = (int)((float)data.histograma[i] / data.maxHist * 300);
                SDL_RenderLine(renSec, 72 + i, 350, 72 + i, 350 - h);
            }
        }

        // Desenha o Botão conforme o estado
        if(btnState == 0) SDL_SetRenderDrawColor(renSec, 0, 0, 255, 255);
        else if(btnState == 1) SDL_SetRenderDrawColor(renSec, 100, 100, 255, 255);
        else SDL_SetRenderDrawColor(renSec, 0, 0, 150, 255);
        
        SDL_RenderFillRect(renSec, &btn);
        SDL_RenderPresent(renSec);
    }

    // --- LIMPEZA FINAL ---
    SDL_DestroySurface(data.originalGray);
    SDL_DestroySurface(data.equalizedGray);
    SDL_DestroyRenderer(renMain);
    SDL_DestroyRenderer(renSec);
    SDL_DestroyWindow(winMain);
    SDL_DestroyWindow(winSec);
    SDL_Quit();

    return 0;
}