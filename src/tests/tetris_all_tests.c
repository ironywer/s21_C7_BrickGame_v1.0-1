#include <check.h>

#include "../brick_game/tetris/tetris.h"

static Tetris game;

START_TEST(test_initialize_game) {
  initialize_game(&game);
  ck_assert_int_eq(game.level, 0);
  ck_assert_int_eq(game.score, 0);
  ck_assert_ptr_nonnull(game.grid);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(game.grid[i][j], 0);
    }
  }
}
END_TEST

START_TEST(test_generate_random_tetromino) {
  Tetromino tetromino = generate_random_tetromino();
  ck_assert(tetromino.type >= 0 && tetromino.type < 7);
  ck_assert_int_eq(tetromino.rotation, 0);
}
END_TEST

START_TEST(test_rotate_piece) {
  initialize_game(&game);
  Tetromino original_piece = game.current_piece;
  rotate_piece(&game);
  ck_assert_int_ne(original_piece.rotation, game.current_piece.rotation);
}
END_TEST

START_TEST(test_move_piece_left) {
  initialize_game(&game);
  Tetromino original_piece = game.current_piece;
  move_piece_left_right(&game, 'l');
  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(game.current_piece.positions[i].x,
                     original_piece.positions[i].x);
    ck_assert_int_eq(game.current_piece.positions[i].y,
                     original_piece.positions[i].y);
  }
}
END_TEST

START_TEST(test_move_piece_right) {
  initialize_game(&game);
  Tetromino original_piece = game.current_piece;
  move_piece_left_right(&game, 'r');
  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(game.current_piece.positions[i].x,
                     original_piece.positions[i].x + 1);
    ck_assert_int_eq(game.current_piece.positions[i].y,
                     original_piece.positions[i].y);
  }
}
END_TEST

START_TEST(test_move_piece_down) {
  initialize_game(&game);
  Tetromino original_piece = game.current_piece;
  move_piece_down(&game);
  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(game.current_piece.positions[i].x,
                     original_piece.positions[i].x);
    ck_assert_int_eq(game.current_piece.positions[i].y,
                     original_piece.positions[i].y + 1);
  }
}
END_TEST

START_TEST(test_lock_piece) {
  initialize_game(&game);
  Tetromino piece = game.current_piece;
  lock_piece(&game);
  for (int i = 0; i < 4; ++i) {
    int x = piece.positions[i].x;
    int y = piece.positions[i].y;
    ck_assert_int_eq(game.grid[y][x], 1);
  }
}
END_TEST

START_TEST(test_clear_lines) {
  initialize_game(&game);
  for (int j = 0; j < WIDTH; ++j) {
    game.grid[HEIGHT - 1][j] = 1;
  }
  clear_lines(&game);
  for (int j = 0; j < WIDTH; ++j) {
    ck_assert_int_eq(game.grid[HEIGHT - 1][j], 0);
  }
  ck_assert_int_eq(game.score, 100);
}
END_TEST

START_TEST(test_spawn_new_piece) {
  initialize_game(&game);
  Tetromino cur_piece = game.next_piece;
  spawn_new_piece(&game);
  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(game.current_piece.positions[i].x,
                     cur_piece.positions[i].x);
    ck_assert_int_eq(game.current_piece.positions[i].y,
                     cur_piece.positions[i].y);
  }
  ck_assert_int_ne(cur_piece.type, game.next_piece.type);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Tetris");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_initialize_game);
  tcase_add_test(tc_core, test_generate_random_tetromino);
  tcase_add_test(tc_core, test_rotate_piece);
  tcase_add_test(tc_core, test_move_piece_left);
  tcase_add_test(tc_core, test_move_piece_right);
  tcase_add_test(tc_core, test_move_piece_down);
  tcase_add_test(tc_core, test_lock_piece);
  tcase_add_test(tc_core, test_clear_lines);
  tcase_add_test(tc_core, test_spawn_new_piece);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = tetris_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
