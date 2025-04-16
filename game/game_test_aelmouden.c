#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"

struct game_s {
  int row;
  int column;
  constraint *constraints;
  color *colors;
};

bool test_game_new() {
  uint size = DEFAULT_SIZE;
  constraint constraints[size * size];
  color colors[size * size];

  for (uint i = 0; i < size * size; i++) {
    constraints[i] = UNCONSTRAINED;
    colors[i] = EMPTY;
  }

  game g = game_new(constraints, colors);
  bool success = (g != NULL);
  game_delete(g);
  return success;
}

bool test_game_new_empty() {
  game g = game_new_empty();
  bool success = (g != NULL);

  if (success) {
    uint size = g->row * g->column;
    for (uint i = 0; i < size; i++) {
      if (g->constraints[i] != UNCONSTRAINED || g->colors[i] != EMPTY) {
        success = false;
        break;
      }
    }
  }

  game_delete(g);
  return success;
}

bool test_game_copy() {
  uint size = DEFAULT_SIZE;
  constraint constraints[size * size];
  color colors[size * size];

  for (uint i = 0; i < size * size; i++) {
    constraints[i] = UNCONSTRAINED;
    colors[i] = EMPTY;
  }

  game original = game_new(constraints, colors);
  game copy = game_copy(original);
  bool success = game_equal(original, copy);
  game_delete(original);
  game_delete(copy);
  return success;
}

bool test_game_equal() {
  game g = game_default();

  game copy = game_copy(g);

  if (game_equal(g, copy)) {
    return true;
  } else {
    return false;
  }
}

int test_game_delete() {
  game g = game_default();

  game_delete(g);

  if (g != NULL) {
    fprintf(stderr, "Test \"test_aelmouden_game_delete\" finished: FAILURE\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// void test_game_solve() {
//     game g= game_default();

//     if (game_solve(sample_game)) {
//         printf("Test passed: Game solved!\n");
//     } else {
//         printf("Test failed: Unable to solve the game.\n");
//     }
// }

int test_dummy() { return EXIT_SUCCESS; }

int main(int argc, char *argv[]) {
  bool ok = false;
  if (argc != 2) {
    printf("Invalid argument\n");
    return EXIT_FAILURE;
  }
  char *nom = argv[1];
  if (strcmp(nom, "dummy") == 0) {
    int res = test_dummy();
    ok = (res == EXIT_SUCCESS);

  } else if (strcmp(nom, "test_game_new") == 0) {
    int res = test_game_new();
    ok = res;
  } else if (strcmp(nom, "test_game_new_empty") == 0) {
    int res = test_game_new_empty();
    ok = res;
  } else if (strcmp(nom, "test_game_copy") == 0) {
    int res = test_game_copy();
    ok = res;
  } else if (strcmp(nom, "test_game_equal") == 0) {
    int res = test_game_equal();
    ok = res;
  } else if (strcmp(nom, "test_game_delete") == 0) {
    int res = test_game_delete();
    ok = res;
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
