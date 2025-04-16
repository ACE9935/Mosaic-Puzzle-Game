#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"
#include "queue.h"

bool test_game_set_color() {
  game g = game_default();
  game_set_color(g, 1, 0, WHITE);
  if (game_get_color(g, 1, 0) == WHITE) {
    game_delete(g);
    return true;
  }
  return false;
}

bool test_game_get_status() {
  game g = game_default_solution();
  bool test1 = (game_get_status(g, 0, 0) == SATISFIED);

  game_set_color(g, 0, 0, BLACK);
  bool test2 = (game_get_status(g, 0, 0) == ERROR);
  game_set_color(g, 2, 0, EMPTY);
  bool test3 = (game_get_status(g, 1, 1) == UNSATISFIED);

  g->wrapping = true;
  bool test4 = (game_get_status(g, 0, 0) == ERROR);

  game_delete(g);
  return (test1 && test2 && test3 && test4);
}

bool test_game_nb_neighbors() { return true; }

bool test_game_play_move() {
  bool result = true;
  game g = game_default();  // Initialize the game

  // Test 1: Valid move with BLACK color
  game_play_move(g, 1, 2, BLACK);
  result &= (game_get_color(g, 1, 2) == BLACK);

  // Test 2: Valid move with WHITE color
  game_play_move(g, 2, 3, WHITE);
  result &= (game_get_color(g, 2, 3) == WHITE);

  // Test 3: Valid move with EMPTY color
  game_play_move(g, 4, 3, EMPTY);
  result &= (game_get_color(g, 4, 3) == EMPTY);

  game_delete(g);
  return result;
}

bool test_game_won() {
  game g =
      game_default_solution();  // Initialize the game with an appropriate size
  if (game_won(g)) {
    return true;
  } else {
    return false;
  }

  game_set_color(g, 0, 0, BLACK);

  if (game_won(g)) {
    return false;
  } else {
    return true;
  }
}

bool test_game_restart() {
  game g = game_default_solution();
  game_restart(g);
  if (game_get_color(g, 0, 2) == BLACK) {
    return false;
  }
  bool allEmpty = true;
  for (uint i = 0; i < g->row; i++) {
    for (uint j = 0; j < g->column; j++) {
      if (game_get_color(g, i, j) != EMPTY) {
        allEmpty = false;
      }
    }
  }
  game_delete(g);
  return allEmpty;
}

bool test_game_new_ext() {
  uint rows = 6, cols = 5;
  constraint constraints[30] = {
      0, -1, -1, 3, -1, -1, 5,  -1, -1, -1, -1, -1, 4, -1, 1,
      6, -1, 6,  3, -1, -1, -1, -1, -1, -1, 2,  3,  5, 1,  2,
  };
  color colors[30] = {
      EMPTY, WHITE, BLACK, WHITE, BLACK, EMPTY, BLACK, EMPTY, WHITE, BLACK,
      EMPTY, WHITE, WHITE, BLACK, EMPTY, WHITE, BLACK, BLACK, BLACK, WHITE,
      BLACK, EMPTY, WHITE, BLACK, EMPTY, BLACK, WHITE, BLACK, WHITE, EMPTY,
  };
  game g = game_new_ext(rows, cols, constraints, colors, true, FULL);

  if (g == NULL || game_nb_rows(g) != rows || game_nb_cols(g) != cols ||
      game_is_wrapping(g) != true || game_get_neighbourhood(g) != FULL) {
    game_delete(g);
    return false;
  }

  game_delete(g);
  return true;
}

bool test_game_new_empty_ext() {
  uint rows = 6, cols = 5;
  game g = game_new_empty_ext(rows, cols, false, ORTHO);

  if (g == NULL || game_nb_rows(g) != rows || game_nb_cols(g) != cols ||
      game_is_wrapping(g) != false || game_get_neighbourhood(g) != ORTHO) {
    game_delete(g);
    return false;
  }

  // Additional checks to ensure all cells are initialized as EMPTY
  for (uint i = 0; i < rows; i++) {
    for (uint j = 0; j < cols; j++) {
      if (game_get_color(g, i, j) != EMPTY) {
        game_delete(g);
        return false;
      }
    }
  }

  game_delete(g);
  return true;
}

bool test_game_nb_rows() {
  uint rows = 5, cols = 6;
  game g = game_new_empty_ext(rows, cols, false, ORTHO);

  bool result = (game_nb_rows(g) == rows);
  game_delete(g);
  return result;
}

bool test_game_nb_cols() {
  uint rows = 5, cols = 6;
  game g = game_new_empty_ext(rows, cols, false, ORTHO);

  bool result = (game_nb_cols(g) == cols);
  game_delete(g);
  return result;
}

bool test_game_is_wrapping() {
  game g = game_new_empty_ext(5, 5, true, ORTHO);
  bool result = game_is_wrapping(g);
  game_delete(g);

  game g2 = game_new_empty_ext(5, 5, false, ORTHO);
  bool result2 = !game_is_wrapping(g2);
  game_delete(g2);

  return result && result2;
}

bool test_game_get_neighbourhood() {
  game g = game_new_empty_ext(5, 5, false, FULL);
  bool result = (game_get_neighbourhood(g) == FULL);
  game_delete(g);

  game g2 = game_new_empty_ext(5, 5, false, ORTHO);
  bool result2 = (game_get_neighbourhood(g2) == ORTHO);
  game_delete(g2);
  game g3 = game_new_empty_ext(5, 5, false, FULL_EXCLUDE);
  bool result3 = (game_get_neighbourhood(g3) == FULL_EXCLUDE);
  game_delete(g3);
  game g4 = game_new_empty_ext(5, 5, false, ORTHO_EXCLUDE);
  bool result4 = (game_get_neighbourhood(g4) == ORTHO_EXCLUDE);
  game_delete(g4);

  return result && result2 && result3 && result4;
}

bool test_game_undo() {
  game g = game_default_solution();
  game_play_move(g, 0, 0, BLACK);  // Make a move
  game_undo(g);                    // Undo the move

  bool result =
      (game_get_color(g, 0, 0) == WHITE);  // Check if the move was undone
  game_delete(g);
  return result;
}

bool test_game_redo() {
  game g = game_new_empty_ext(5, 5, false, ORTHO);
  game_play_move(g, 2, 2, BLACK);  // Make a move
  game_undo(g);                    // Undo the move
  game_redo(g);                    // Redo the move

  bool result =
      (game_get_color(g, 2, 2) == BLACK);  // Check if the move was redone
  game_delete(g);

  return result;
}
bool test_game_load() {
  // Create a new game and save it to a file
  game g = game_new_empty_ext(7, 6, false, FULL);
  game_save(g, "test.txt");

  // Load the game from the saved file
  game g2 = game_load("test.txt");

  // Check if the loaded game is equal to the original
  if (game_equal(g, g2)) {
    // If equal, play a move, save the game to a new file
    game_play_move(g, 0, 0, WHITE);
    game_save(g, "test1.txt");

    // Check if the two games are no longer equal after the move
    return !game_equal(g, g2);
  } else {
    // If not equal initially, return false
    return false;
  }
}

int test_dummy() { return EXIT_SUCCESS; }

int main(int argc, char *argv[]) {
  bool ok = false;
  if (argc != 2) {
    printf("invalid argument\n");
    return EXIT_FAILURE;
  }
  char *nom = argv[1];
  if (strcmp(nom, "dummy") == 0) {
    int res = test_dummy();
    return res;
  } else if (strcmp(nom, "test_game_set_color") == 0) {
    ok = test_game_set_color();

  } else if (strcmp(nom, "test_game_get_status") == 0) {
    ok = test_game_get_status();

  } else if (strcmp(nom, "test_game_nb_neighbors") == 0) {
    ok = test_game_nb_neighbors();

  } else if (strcmp(nom, "test_game_play_move") == 0) {
    ok = test_game_play_move();

  } else if (strcmp(nom, "test_game_won") == 0) {
    ok = test_game_won();

  } else if (strcmp(nom, "test_game_restart") == 0) {
    ok = test_game_restart();

  } else if (strcmp(nom, "test_game_new_ext") == 0) {
    ok = test_game_new_ext();

  } else if (strcmp(nom, "test_game_new_empty_ext") == 0) {
    ok = test_game_new_empty_ext();

  } else if (strcmp(nom, "test_game_nb_rows") == 0) {
    ok = test_game_nb_rows();

  } else if (strcmp(nom, "test_game_nb_cols") == 0) {
    ok = test_game_nb_cols();

  } else if (strcmp(nom, "test_game_is_wrapping") == 0) {
    ok = test_game_is_wrapping();

  } else if (strcmp(nom, "test_game_get_neighbourhood") == 0) {
    ok = test_game_get_neighbourhood();

  } else if (strcmp(nom, "test_game_undo") == 0) {
    ok = test_game_undo();

  } else if (strcmp(nom, "test_game_redo") == 0) {
    ok = test_game_redo();

  } else if (strcmp(nom, "test_game_load") == 0) {
    ok = test_game_load();
  } else {
    printf("Invalid argument or test name unknown\n");
    return EXIT_FAILURE;
  }

  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
