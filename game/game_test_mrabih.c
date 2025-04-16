#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"
#include "game_tools.h"

// game_aux.h testing functions

int test_game_print() {
  game g = game_default();

  assert(g != NULL);

  game_print(g);

  game_delete(g);

  return EXIT_SUCCESS;
}

int test_game_default() {
  game g = game_default();

  if (g != NULL && g->row == 5 && g->column == 5) {
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
}

int test_game_default_solution() {
  game g = game_default_solution();

  assert(g != NULL);
  assert(game_won(g));

  game_print(g);

  game_delete(g);

  return EXIT_SUCCESS;
}

// game.h testing functions

int test_game_set_constraint() {
  game g = game_new_empty();

  game_set_constraint(g, 0, 0, 2);
  assert(game_get_constraint(g, 0, 0) == 2);

  game_set_constraint(g, 1, 1, 1);
  assert(game_get_constraint(g, 1, 1) == 1);

  game_set_constraint(g, 2, 2, UNCONSTRAINED);
  assert(game_get_constraint(g, 2, 2) == UNCONSTRAINED);

  game_delete(g);

  return 0;
}

bool test_game_set_color() {
  game g = game_default();
  game_set_color(g, 1, 0, WHITE);
  if (game_get_color(g, 1, 0) == WHITE) {
    game_delete(g);
    return true;
  }
  return false;
}

int test_game_get_constraint() {
  game g = game_new_empty();
  game_set_constraint(g, 1, 2, 1);

  assert(game_get_constraint(g, 1, 2) == 1);

  game_delete(g);

  return EXIT_SUCCESS;
}

int test_game_get_color() {
  game g = game_new_empty();

  assert(game_get_color(g, 0, 0) == EMPTY);
  assert(game_get_color(g, 1, 1) == EMPTY);

  game_set_color(g, 0, 0, WHITE);
  assert(game_get_color(g, 0, 0) == WHITE);

  game_set_color(g, 1, 1, BLACK);
  assert(game_get_color(g, 1, 1) == BLACK);

  game_delete(g);

  return 0;
}

int test_game_get_next_square() {
  game g = game_new_empty();

  assert(game_get_next_square(g, 2, 2, UP, NULL, NULL) == true);
  assert(game_get_next_square(g, 2, 2, RIGHT, NULL, NULL) == true);
  assert(game_get_next_square(g, 0, 2, UP, NULL, NULL) == false);

  game_delete(g);

  return EXIT_SUCCESS;
}

// game_tools
int test_game_save() {
  game g = game_new_empty();
  game_save(g, "test_game_save.txt");
  game g_loaded = game_load("test_game_save.txt");
  game_delete(g);
  game_delete(g_loaded);
  return 0;
}

int test_dummy() { return EXIT_SUCCESS; }

int main(int argc, char *argv[]) {
  bool ok = false;
  if (argc != 2) {
    printf("Invalid argument\n");
    return EXIT_FAILURE;
  }
  char *nom = argv[1];
  int res = EXIT_FAILURE;  // inialise the result var
  if (strcmp(nom, "dummy") == 0) {
    res = test_dummy();
    ok = (res == EXIT_SUCCESS);
  } else if (strcmp(nom, "test_game_default") == 0) {
    res = test_game_default();
    ok = (res == EXIT_SUCCESS);
  } else if (strcmp(nom, "test_game_default_solution") == 0) {
    res = test_game_default_solution();
    ok = (res == EXIT_SUCCESS);
  } else if (strcmp(nom, "test_game_print") == 0) {
    res = test_game_print();
    ok = (res == EXIT_SUCCESS);
  } else if (strcmp(nom, "test_game_set_constraint") == 0) {
    res = test_game_set_constraint();
    ok = (res == EXIT_SUCCESS);
  } else if (strcmp(nom, "test_game_set_color") == 0) {
    if (test_game_set_color()) {
      res = EXIT_SUCCESS;
      ok = true;
    }
  } else if (strcmp(nom, "test_game_get_constraint") == 0) {
    res = test_game_get_constraint();
    ok = (res == EXIT_SUCCESS);
  } else if (strcmp(nom, "test_game_get_color") == 0) {
    res = test_game_get_color();
    ok = (res == EXIT_SUCCESS);
  } else if (strcmp(nom, "test_game_get_next_square") == 0) {
    res = test_game_get_next_square();
    ok = (res == EXIT_SUCCESS);
  } else if (strcmp(nom, "test_game_save") == 0) {
    res = test_game_save();
    ok = (res == EXIT_SUCCESS);
  } else {
    printf("Invalid argument or test name unknown\n");
    return EXIT_FAILURE;
  }

  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
  }

  return res;
}
