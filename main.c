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
int x = 0;
int y = 0;
const int ROWS = screenWidth/size;
const int COLUMNS = screenHeight/size;

void nextGen();
void input();
void doRender();
void drawCells();
int neighbor();

int main(int argc, char **argv){
    srand(time(NULL));
    x = screenWidth/2;
    y = screenHeight/2;
    int grid[ROWS][COLUMNS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            grid[i][j] = rand() % 20; 
        }
    }

    if(SDL_Init(SDL_INIT_VIDEO) > 0){
        printf("INIT ERROR: %s", SDL_GetError());
        return -1;
    }
    window = SDL_CreateWindow(
        "GameOfLife",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth,
        screenHeight,
        0);

    if(window < 0){
        printf("WINDOW ERROR: %s\n", SDL_GetError());
        SDL_Quit();
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer < 0){
        printf("RENDERER ERROR: %s", SDL_GetError());
        return -1;
    }
    doRender();

    while(!quit){
        input();
        drawCells(grid);
        nextGen(grid,grid);
        SDL_Delay(100);
    }
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_Quit();

    return 0;
}

void input(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT_CLOSE:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }
}

void doRender(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);    
}
void drawCells(int (*grid)[COLUMNS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            if(grid[i][j] == 0){
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect rect = {(i*size), (j*size), size, size};
                SDL_RenderDrawRect(renderer, &rect);
                SDL_RenderFillRect(renderer, &rect);
            }else{
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect rect = {(i*size), (j*size), size, size};
                SDL_RenderDrawRect(renderer, &rect);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}


void nextGen(int (*grid)[COLUMNS], int (*next)[COLUMNS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            int count = 0;
            if(!(i-1 < 0 || i + 1 == ROWS || j - 1 < 0 || j + 1 == COLUMNS)){
                if(grid[i-1][j] == 0  && !(i-1 < 0 || i + 1 > ROWS || j - 1 < 0 || j + 1 > COLUMNS)) count++;
                if(grid[i+1][j] == 0 && !(i-1 < 0 || i + 1 > ROWS || j - 1 < 0 || j + 1 > COLUMNS)) count++;
                if(grid[i-1][j+1] == 0 && !(i-1 < 0 || i + 1 > ROWS || j - 1 < 0 || j + 1 > COLUMNS)) count++;
                if(grid[i+1][j-1] == 0 && !(i-1 < 0 || i + 1 > ROWS || j - 1 < 0 || j + 1 > COLUMNS)) count++;
                if(grid[i-1][j-1] == 0 && !(i-1 < 0 || i + 1 > ROWS || j - 1 < 0 || j + 1 > COLUMNS)) count++;
                if(grid[i+1][j+1] == 0 && !(i-1 < 0 || i + 1 > ROWS || j - 1 < 0 || j + 1 > COLUMNS)) count++;
                if(grid[i][j+1] == 0 && !(i-1 < 0 || i + 1 > ROWS || j - 1 < 0 || j + 1 > COLUMNS)) count++;
                if(grid[i][j-1] == 0 && !(i-1 < 0 || i + 1 > ROWS || j - 1 < 0 || j + 1 > COLUMNS)) count++;
            }
            if(grid[i][j] == 0 && count < 2){
                next[i][j] = 1;//underpopulation
            }else if(grid[i][j] == 0 && count > 3){
                next[i][j] = 1;//overpopulation
            }else if(grid[i][j] != 0 && count == 3){
                next[i][j] = 0;//reproduction
            }else if(grid[i][j] == 0 && (count == 3 || count == 2)){
                next[i][j] = 0;//stays
            }
        }
    }
}

