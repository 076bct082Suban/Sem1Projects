#include<stdio.h>
#include<SDL2/SDL.h>
#include<string.h>
#include<SDL2/SDL_timer.h>
#include<SDL_image.h>
#include<stdlib.h>

#include "stuff.h"

#include <SDL2/SDL.h>
#include "stuff.h"

const SDL_Color GRID_COLOR = { .r = 0, .g = 0, .b = 0 };
const SDL_Color PLAYER_X_COLOR = { .r = 255, .g = 0, .b = 0};
const SDL_Color PLAYER_O_COLOR = { .r = 0, .g = 0, .b = 255};
const SDL_Color TIE_COLOR  = { .r = 0, .g = 255, .b = 0};
int gameover = 0;

/* ------------------------- RENDER BEGIN -------------------------------- */
void draw_X(SDL_Renderer *Renderer, int index, SDL_Color *color){
    int celli, cellj, centerX, centerY;
    int cellCenterX = CELL_WIDTH/ 2, cellCenterY = CELL_HEIGHT / 2;
    //for(int i = 0; i < N * N; i++){
        // a00, a01 .. a10
        // a11, a12 ... a21
    celli = index / N;
    cellj = index % N;

    // Swaped i and j
    centerX = cellCenterX + CELL_WIDTH * cellj;
    centerY = cellCenterY + CELL_HEIGHT * celli;

    SDL_SetRenderDrawColor(Renderer, color->r, color->g, color->b, 255);


    // if you are wondering why so many render lines, it is to
    // MAKE THE LINES THICCCCC!!!
    for(int i = 0; i < 5; i++){
            SDL_RenderDrawLine(Renderer, centerX - CELL_WIDTH / 4 , centerY - CELL_HEIGHT / 4 + i,
                       centerX + CELL_WIDTH / 4, centerY + CELL_HEIGHT / 4 + i);

            SDL_RenderDrawLine(Renderer, centerX - CELL_WIDTH / 4, centerY + CELL_HEIGHT / 4 + i,
                       centerX + CELL_WIDTH / 4, centerY - CELL_HEIGHT / 4 + i);

            SDL_RenderDrawLine(Renderer, centerX - CELL_WIDTH / 4 , centerY - CELL_HEIGHT / 4 - i,
                       centerX + CELL_WIDTH / 4, centerY + CELL_HEIGHT / 4 - i);

            SDL_RenderDrawLine(Renderer, centerX - CELL_WIDTH / 4, centerY + CELL_HEIGHT / 4 - i,
                       centerX + CELL_WIDTH / 4, centerY - CELL_HEIGHT / 4 - i);
    }


}

void draw_O(SDL_Renderer *Renderer, int index, SDL_Color *color){
        int celli, cellj, centerX, centerY;
    int cellCenterX = CELL_WIDTH/ 2, cellCenterY = CELL_HEIGHT / 2;
    //for(int i = 0; i < N * N; i++){
        // a00, a01 .. a10
        // a11, a12 ... a21
    celli = index / N;
    cellj = index % N;

    centerX = cellCenterX + CELL_WIDTH * cellj;
    centerY = cellCenterY + CELL_HEIGHT * celli;

    SDL_SetRenderDrawColor(Renderer, color->r, color->g, color->b, 255);
    for(int i = 0; i < 5; i++){
        SDL_RenderDrawLine(Renderer, centerX - CELL_WIDTH / 4 , centerY + i, centerX + CELL_WIDTH / 4, centerY + i);
    }
}

void render_blocks(SDL_Renderer *Renderer, struct game_t *game){
    int celli, cellj;
    for(int i = 0; i < N * N; i++){
        // a00, a01 .. a10
        // a11, a12 ... a21
        celli = i / N;
        cellj = i % N;

        switch(game->board[i]){
            case EMPTY:
                break;
            case PLAYER_X:
                draw_X(Renderer, i, &PLAYER_X_COLOR);
                break;
            case PLAYER_O:
                draw_O(Renderer, i, &PLAYER_O_COLOR);
                break;
        }
    }
}

void render_grid(SDL_Renderer *Renderer, SDL_Color *color){

    SDL_SetRenderDrawColor(Renderer, color->r, color->g, color->b, 255);

    // Vertical lines and horizontal lines
    for(int i = 0; i < N; i++){
        SDL_RenderDrawLine(Renderer, CELL_WIDTH * i, 0, CELL_WIDTH * i, SCREEN_HEIGHT );

        SDL_RenderDrawLine(Renderer, 0, CELL_HEIGHT * i, SCREEN_WIDTH, CELL_HEIGHT * i);
    }
}

void render_game(SDL_Renderer *Renderer, const struct game_t *game){

    switch(game->state){
        case PLAY_STATE:
            render_running_game(Renderer, game);
            break;

        case PLAYER_X_WON:
            render_end_of_game(Renderer, game, &PLAYER_X_COLOR);
            break;

        case PLAYER_O_WON:
            render_end_of_game(Renderer, game, &PLAYER_O_COLOR);
            break;

        case TIE_STATE:
            render_end_of_game(Renderer, game, &TIE_COLOR);
            break;
    }

}

void render_running_game(SDL_Renderer *Renderer, const struct game_t *game){
    render_grid(Renderer, &GRID_COLOR);
    render_blocks(Renderer, game);
}

void render_end_of_game(SDL_Renderer *Renderer, const struct game_t *game, SDL_Color *color){
    int celli, cellj;
    render_grid(Renderer, color);
    for(int i = 0; i < N * N; i++){
        // a00, a01 .. a10
        // a11, a12 ... a21
        celli = i / N;
        cellj = i % N;

        switch(game->board[i]){
            case EMPTY:
                break;
            case PLAYER_X:
                draw_X(Renderer, i, color);
                break;
            case PLAYER_O:
                draw_O(Renderer, i, color);
                break;
        }
    }
}
/* ------------------------- RENDER END-------------------------------- */

/* ------------------------- LOGIC SECTION  -----------------------------*/
void clickBlock(struct game_t *game, int index){
    if(game->board[index] == EMPTY){
        game->board[index] = game->player;
        if(checkIfWon(game) == 1){
            printf("yay");
            game->state = game->player;
        }else if(checkIfTie() == 0){ // tie
            game->state = TIE_STATE;
        }else{
            switchPlayer(game);
        }
    }

}

void onClick(struct game_t* game, int index){
    if(game->state == PLAY_STATE){
        clickBlock(game, index);
    }else if(game->state == TIE_STATE){
        resetBoard(game);
    }
    else{
        resetBoard(game);
    }
}

void switchPlayer(struct game_t *game){
    if(game->player == PLAYER_O){
        game->player = PLAYER_X;
    }else if(game->player == PLAYER_X){
        game->player = PLAYER_O;
    }
}

int checkIfWon(const struct game_t *game){
    // Returns 0 if won, 1 if not won
    for(int i = 0; i < N; i++){
        if(game->board[i]!= EMPTY){
            if(game->board[i] == game->board[i + N] && game->board[i] == game->board[i + 2 * N]){
                return 1;
            }
        }

        if(game->board[i * N] != EMPTY){
            if(game->board[i * 3] == game->board[i * 3 + 1] && game->board[i * 3] == game->board[i * 3 + 2]){
                return 1;
            }
        }
    }

    if(game->board[4] != EMPTY){
        if(game->board[0] == game->board[4] && game->board[4] == game->board[8]){
            return 1;
        }else if(game ->board[2] == game->board[4] && game->board[4] == game->board[6]){
            return 1;
        }
    }


    return 0;
}

int checkIfTie(const struct game_t *game){
    // returns 1 if not tie
    // returns 0 if tie

    for(int i = 0; i < N * N ; i++){
        if(game->board[i] == EMPTY){
            return 1;
        }
    }
    return 0;
}


void resetBoard(struct game_t *game){
    int i;
    game->player = PLAYER_X;
    game->state = PLAY_STATE;
    for(i = 0; i < N * N; i++){
        game->board[i] = EMPTY;
    }
}


/* ------------------------- LOGIC END -------------------------------- */
int main(int argc, char* argv[]){
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event e; // handles events
        int click_index;
        /* Initialize SDL. */
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                return 1;

        /* Create the window where we will draw. */
        window = SDL_CreateWindow("TIC TAC TOE",
                                          100, 100,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);


        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);


        SDL_SetRenderDrawColor(renderer, 190, 240, 240, 255);

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        struct game_t game = {
        .board = { EMPTY, EMPTY, EMPTY,
                   EMPTY, EMPTY, EMPTY,
                   EMPTY, EMPTY, EMPTY },
        .player = PLAYER_X,
        .state = PLAY_STATE
        };

        while (game.state != QUIT_STATE) {
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                    case SDL_QUIT:
                        game.state = QUIT_STATE;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        // For some reason CELL_WIDTH AND CELL_HEIGHT DID NOT WORK
                        // SO, hard coded it, don't change the resolution
                        click_index = e.button.x / 213 + (e.button.y / 160) * 3;
                        onClick(&game, click_index);

                        // X / CELL_WIDTH , Y / CELL_HEIGHT


                }
            }
        SDL_SetRenderDrawColor(renderer, 190, 240, 240, 255);
        SDL_RenderClear(renderer);
        render_game(renderer, &game);
        SDL_RenderPresent(renderer);
        }
        SDL_Quit();
        return 0;
}

