# Projeto 1 - Processamento Digital de Imagem (SDL3)

Este projeto foi desenvolvido para a disciplina de Computação Visual. O software realiza a manipulação de pixels em tempo real para converter imagens coloridas, analisar seus dados estatísticos e aplicar melhorias de contraste.

## O que é o projeto
O programa é uma ferramenta de processamento de imagem que utiliza a biblioteca SDL3. Ele permite carregar arquivos de imagem via linha de comando, transformá-los para tons de cinza, visualizar o histograma de intensidades e aplicar a técnica de Equalização de Histograma para melhorar a qualidade visual da imagem.

## Como o projeto funciona
1. Conversão de Luminância: O programa percorre cada pixel e aplica a fórmula $Y = 0.2125 \cdot R + 0.7154 \cdot G + 0.0721 \cdot B$ para garantir uma conversão precisa para cinza.
2. Análise Estatística: Calcula a Média e o Desvio Padrão dos pixels para classificar a imagem (ex: Clara/Escura, Baixo/Alto contraste).
3. Histograma: Gera um gráfico de barras na janela secundária representando a frequência de cada tom de cinza (0-255).
4. Equalização (CDF): Implementa a Função de Distribuição Acumulada para redistribuir as intensidades e expandir o contraste.

## Contribuições do Grupo
* Eduardo Losco: Setup do projeto, estruturação do repositório Git, configuração do arquivo `.gitignore` e documentação inicial.
* Bruno Roveri: Implementação da lógica de conversão para escala de cinza e salvamento de arquivos (tecla S).
* Ian Merlini:Desenvolvimento da interface multi-janela (SDL_Window) e cálculo das estatísticas (Média/Desvio).
* Otto Mooca: Implementação do algoritmo de equalização de histograma e lógica do botão interativo na GUI.

## Compilação e Execução

### Pré-requisitos
* Ambiente MSYS2 com o subsistema UCRT64 instalado.
* Bibliotecas `SDL3` e `SDL3_image` configuradas.

### Passo 1: Compilação
No terminal UCRT64, navegue até a raiz do projeto e execute:
g++ src/*.cpp -I include -o programa -lSDL3 -lSDL3_image

### Passo 2: Execução 
./programa assets/teste.jpg
