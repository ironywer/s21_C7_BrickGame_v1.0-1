#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../brick_game/tetris/tetris.h"

void initialize_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(
      500);  // Увеличенный таймаут для getch, чтобы можно было обновлять экран
}

void terminate_ncurses() { endwin(); }
void save_max_score(int max_score) {
  FILE *file = fopen("score.txt", "w");
  if (file == NULL) {
    perror("Ошибка при открытии файла score.txt для записи");
    return;
  }
  fprintf(file, "%d", max_score);
  fclose(file);
}
void draw_game(const Tetris *game) {
  clear();

  // Рисуем границы поля
  for (int i = 0; i < HEIGHT + 2; ++i) {
    mvprintw(i, 0, "|");
    mvprintw(i, WIDTH + 1, "|");
  }
  for (int j = 0; j < WIDTH + 2; ++j) {
    mvprintw(0, j, "-");
    mvprintw(HEIGHT + 1, j, "-");
  }

  // Рисуем фигуры на поле
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if (game->grid[i][j]) {
        mvprintw(i + 1, j + 1, "X");
      }
    }
  }

  // Рисуем текущую фигуру
  for (int i = 0; i < 4; ++i) {
    int x = game->current_piece.positions[i].x;
    int y = game->current_piece.positions[i].y;
    mvprintw(y + 1, x + 1, "X");
  }
  mvprintw(0, WIDTH + 3, "Max score: %d", game->max_score);
  mvprintw(1, WIDTH + 3, "Score: %d", game->score);

  // Рисуем следующую фигуру
  mvprintw(2, WIDTH + 3, "Next:");
  for (int i = 0; i < 4; ++i) {
    int x = game->next_piece.positions[i].x;
    int y = game->next_piece.positions[i].y;
    mvprintw(y + 4, x + WIDTH + 3, "X");
  }

  refresh();
}

int main() {
  Tetris game;
  initialize_game(&game);

  initialize_ncurses();

  while (true) {
    int ch = getch();
    switch (ch) {
      case 'q':
        terminate_ncurses();
        exit(0);
        break;
      case 'p':
        if (game.state == PLAYING) {
          game.state = PAUSED;
        } else if (game.state == PAUSED) {
          game.state = PLAYING;
        }
        break;
      case KEY_LEFT:
        move_piece_left_right(&game, 'l');
        break;
      case KEY_RIGHT:
        move_piece_left_right(&game, 'r');
        break;
      case KEY_DOWN:
        move_piece_down(&game);
        break;
      case ' ':
        rotate_piece(&game);
        break;
    }

    update_game(&game);
    draw_game(&game);

    if (game.state == GAME_OVER) {
      if (game.score > game.max_score) {
        game.max_score = game.score;
        save_max_score(game.max_score);
      }
      mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "GAME OVER");
      refresh();
      sleep(3);
      terminate_ncurses();
      exit(0);
    }

    timeout(1100 -
            game.level *
                100);  // Уменьшенная задержка для регулировки скорости игры
  }

  terminate_ncurses();
  return 0;
}
