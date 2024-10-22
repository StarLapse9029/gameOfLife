#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

const int size = 10;
const int screenWidth = 1080;
const int screenHeight = 720;
bool quit = false;
SDL_Window *window;
SDL_Renderer *renderer;

const int ROWS = screenHeight / size;
const int COLUMNS = screenWidth / size;

void nextGen(int grid[ROWS][COLUMNS]);
void input();
void doRender();
void drawCells(int grid[ROWS][COLUMNS]);

int main(int argc, char **argv){
    srand(time(NULL));
    int grid[ROWS][COLUMNS];

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            grid[i][j] = rand() % 2;
        }
    }
    if(SDL_Init(SDL_INIT_VIDEO) > 0){
        printf("INIT ERROR: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow(
        "GameOfLife",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth,
        screenHeight, 0);

    if(!window){
        printf("WINDOW ERROR: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("RENDERER ERROR: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    doRender();

    while(!quit){
        input();
        drawCells(grid);
        nextGen(grid);
        SDL_Delay(100);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void input() {
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
            quit = true;
        }
    }
}

void doRender(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void drawCells(int grid[ROWS][COLUMNS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            SDL_SetRenderDrawColor(renderer, grid[i][j] ? 255 : 0, grid[i][j] ? 255 : 0,  grid[i][j] ? 255 : 0, 255);
            SDL_Rect rect = {j * size, i * size, size, size};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

void nextGen(int grid[ROWS][COLUMNS]){
    int next[ROWS][COLUMNS];
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            int count = 0;
            for(int x = -1; x <= 1; x++){
                for(int y = -1; y <= 1; y++){
                    if(x == 0 && y == 0){
                      continue;
                    }
                    int ni = i + x, nj = j + y;
                    if(ni >= 0 && ni < ROWS && nj >= 0 && nj < COLUMNS){
                        count += grid[ni][nj];
                    }
                }
            }
            if(grid[i][j] == 1 && (count < 2 || count > 3)){
                next[i][j] = 0; 
            }else if(grid[i][j] == 0 && count == 3) {
                next[i][j] = 1;
            }else{
                next[i][j] = grid[i][j];
            }
        }
    }
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            grid[i][j] = next[i][j];
        }
    }
}

