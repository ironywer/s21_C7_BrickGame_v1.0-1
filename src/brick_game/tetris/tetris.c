#include "tetris.h"

Tetromino generate_random_tetromino() {
  Tetromino tetromino;
  tetromino.type = rand() % 7;
  tetromino.rotation = 0;
  for (int i = 0; i < 4; ++i) {
    tetromino.positions[i].x =
        TETROMINO_SHAPES[tetromino.type][tetromino.rotation][i].x + WIDTH / 2 -
        1;
    tetromino.positions[i].y =
        TETROMINO_SHAPES[tetromino.type][tetromino.rotation][i].y;
  }
  return tetromino;
}

void initialize_game(Tetris *game) {
  // Инициализация игры
  game->score = 0;
  game->state = PLAYING;
  // Заполнение поля пустыми значениями
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      game->grid[i][j] = false;
    }
  }
  // Инициализация фигур
  game->current_piece = generate_random_tetromino();
  game->next_piece = generate_random_tetromino();

  // Считывание рекорда из файла score.txt
  FILE *file = fopen("score.txt", "r");
  if (file == NULL) {
    perror("Ошибка при открытии файла score.txt");
    game->max_score = 0;  // Устанавливаем рекорд в 0, если файл не найден
  } else {
    if (fscanf(file, "%d", &(game->max_score)) != 1) {
      perror("Ошибка при чтении рекорда из файла score.txt");
      game->max_score =
          0;  // Устанавливаем рекорд в 0, если произошла ошибка чтения
    }
    fclose(file);
  }
}

void rotate_piece(Tetris *game) {
  Tetromino *piece = &game->current_piece;
  int new_rotation = (piece->rotation + 1) % 4;
  Position new_positions[4];
  for (int i = 0; i < 4; ++i) {
    new_positions[i] = TETROMINO_SHAPES[piece->type][new_rotation][i];
    new_positions[i].x += piece->positions[0].x;
    new_positions[i].y += piece->positions[0].y;
  }

  // Проверка, можно ли сделать поворот (не выходит ли за границы и не
  // пересекается ли с существующими блоками)
  for (int i = 0; i < 4; ++i) {
    if (new_positions[i].x < 0 || new_positions[i].x >= HEIGHT ||
        new_positions[i].y < 0 || new_positions[i].y >= WIDTH ||
        game->grid[new_positions[i].y][new_positions[i].x]) {
      return;  // Невозможно повернуть фигуру
    }
  }

  // Обновление позиций и состояния вращения
  for (int i = 0; i < 4; ++i) {
    piece->positions[i] = new_positions[i];
  }
  piece->rotation = new_rotation;
}

void move_piece_left_right(Tetris *game, char vector) {
  // Логика перемещения фигуры влево и вправо
  Tetromino *piece = &game->current_piece;
  Position new_positions[4];
  int step = (vector == 'l') ? -1 : 1;
  for (int i = 0; i < 4; ++i) {
    new_positions[i].x = piece->positions[i].x + step;
    new_positions[i].y = piece->positions[i].y;
    if (new_positions[i].x < 0 || new_positions[i].x >= WIDTH ||
        game->grid[new_positions[i].y][new_positions[i].x])
      return;
  }

  for (int i = 0; i < 4; ++i) {
    piece->positions[i] = new_positions[i];
  }
}

int move_piece_down(Tetris *game) {
  // Логика перемещения фигуры вниз
  Tetromino *piece = &game->current_piece;
  Position new_positions[4];
  for (int i = 0; i < 4; ++i) {
    new_positions[i].x = piece->positions[i].x;
    new_positions[i].y = piece->positions[i].y + 1;
    if (new_positions[i].y >= HEIGHT ||
        game->grid[new_positions[i].y][new_positions[i].x] == true)
      return false;
  }

  for (int i = 0; i < 4; ++i) {
    piece->positions[i] = new_positions[i];
  }
  return true;
}

void lock_piece(Tetris *game) {
  Tetromino *piece = &game->current_piece;
  for (int i = 0; i < 4; ++i) {
    int x = piece->positions[i].x;
    int y = piece->positions[i].y;
    game->grid[y][x] = true;
  }
}

void clear_lines(Tetris *game) {
  int podr = 1;
  for (int i = 0; i < HEIGHT; ++i) {
    bool full_line = true;
    for (int j = 0; j < WIDTH; ++j) {
      if (!game->grid[i][j]) {
        full_line = false;
        break;
      }
    }
    if (full_line) {
      for (int k = i; k > 0; --k) {
        for (int j = 0; j < WIDTH; ++j) {
          game->grid[k][j] = game->grid[k - 1][j];
        }
      }
      for (int j = 0; j < WIDTH; ++j) {
        game->grid[0][j] = false;
      }
      switch (podr) {
        case 1:
          game->score += 100;
          break;
        case 2:
          game->score += 200;
          break;
        case 3:
          game->score += 300;
          break;
        case 4:
          game->score += 800;
          break;

        default:
          break;
      }
      podr++;
    }
  }
  game->level = game->score / 600;
  if (game->level > 10) game->level = 10;
}

void spawn_new_piece(Tetris *game) {
  game->current_piece = game->next_piece;
  game->next_piece = generate_random_tetromino();

  for (int i = 0; i < 4; ++i) {
    int x = game->current_piece.positions[i].x;
    int y = game->current_piece.positions[i].y;
    if (game->grid[y][x]) {
      game->state = GAME_OVER;
      break;
    }
  }
}

void update_game(Tetris *game) {
  if (!move_piece_down(game)) {
    lock_piece(game);
    clear_lines(game);
    spawn_new_piece(game);
  }
}