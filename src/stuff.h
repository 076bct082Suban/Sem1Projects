#ifndef GAME_H_
#define GAME_H_

#define N 3
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define CELL_HEIGHT SCREEN_HEIGHT / 3
#define CELL_WIDTH SCREEN_WIDTH / 3

#define EMPTY 0
#define PLAYER_X 1
#define PLAYER_O 2

#define PLAY_STATE 0
#define PLAYER_X_WON 1
#define PLAYER_O_WON 2
#define TIE_STATE 3
#define QUIT_STATE 4

struct game_t{
    int board [N * N];
    int player;
    int state;
};

#endif
