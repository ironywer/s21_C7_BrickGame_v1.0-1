#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define WIDTH 10
#define HEIGHT 20

typedef struct {
  int x;
  int y;
} Position;

typedef struct {
  Position positions[4];
  int type;
  int rotation;
} Tetromino;

static const Position TETROMINO_SHAPES[7][4][4] = {
    // I_SHAPE
    {{{0, -1}, {0, 0}, {0, 1}, {0, 2}},
     {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
     {{0, -1}, {0, 0}, {0, 1}, {0, 2}},
     {{-1, 0}, {0, 0}, {1, 0}, {2, 0}}},
    // O_SHAPE
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
    // T_SHAPE
    {{{0, -1}, {0, 0}, {0, 1}, {1, 0}},
     {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},
     {{0, -1}, {0, 0}, {0, 1}, {-1, 0}},
     {{-1, 0}, {0, 0}, {1, 0}, {0, -1}}},
    // S_SHAPE
    {{{0, -1}, {0, 0}, {1, 0}, {1, 1}},
     {{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
     {{0, -1}, {0, 0}, {1, 0}, {1, 1}},
     {{-1, 0}, {0, 0}, {0, 1}, {1, 1}}},
    // Z_SHAPE
    {{{1, -1}, {1, 0}, {0, 0}, {0, 1}},
     {{-1, 1}, {0, 1}, {0, 0}, {1, 0}},
     {{1, -1}, {1, 0}, {0, 0}, {0, 1}},
     {{-1, 1}, {0, 1}, {0, 0}, {1, 0}}},
    // J_SHAPE
    {{{0, -1}, {0, 0}, {0, 1}, {1, -1}},
     {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},
     {{0, -1}, {0, 0}, {0, 1}, {-1, 1}},
     {{-1, -1}, {-1, 0}, {0, 0}, {1, 0}}},
    // L_SHAPE
    {{{0, -1}, {0, 0}, {0, 1}, {1, 1}},
     {{-1, 1}, {0, 1}, {1, 1}, {1, 0}},
     {{0, -1}, {0, 0}, {0, 1}, {-1, -1}},
     {{-1, 0}, {0, 0}, {1, 0}, {1, -1}}}};
typedef enum { MENU, PLAYING, PAUSED, GAME_OVER } GameState;

typedef struct {
  bool grid[HEIGHT][WIDTH];
  Tetromino current_piece;
  Tetromino next_piece;
  int score;
  int max_score;
  int level;
  GameState state;
} Tetris;

void initialize_game(Tetris *game);
void rotate_piece(Tetris *game);
void move_piece_left_right(Tetris *game, char vector);
int move_piece_down(Tetris *game);
void lock_piece(Tetris *game);
void clear_lines(Tetris *game);
void update_game(Tetris *game);
void update_game(Tetris *game);
Tetromino generate_random_tetromino();
void spawn_new_piece(Tetris *game);
#endif
