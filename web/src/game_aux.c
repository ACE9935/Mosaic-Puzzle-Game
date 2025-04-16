#include "game_aux.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_ext.h"
#include "game_struct.h"

color default_colors[] = {
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
};

constraint default_constraints[] = {
    0,  -1, -1, 3,  -1, -1, 5,  -1, -1, -1, -1, -1, 4,
    -1, 1,  6,  -1, 6,  3,  -1, -1, -1, -1, -1, -1,
};

static color solution_default[] = {
    WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE,
    BLACK, BLACK, BLACK, BLACK, WHITE, WHITE, BLACK, BLACK, WHITE,
    WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, WHITE,
};

char* col2str[3] = {" ", "□", "■"};

char* num2str[3][10] = {
    {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"},     // empty
    {"🄋", "➀", "➁", "➂", "➃", "➄", "➅", "➆", "➇", "➈"},  // white
    {"⓿", "❶", "❷", "❸", "❹", "❺", "❻", "❼", "❽", "❾"},     // black
};

char* _square2str(constraint n, color c) {
  if (n == UNCONSTRAINED)
    return col2str[c];
  else
    return num2str[c][n];
}

void game_print(cgame g) {
  assert(g);
  printf("     ");
  for (uint j = 0; j < game_nb_cols(g); j++) printf("%d ", j);
  printf("\n");
  printf("     ");
  for (uint j = 0; j < 2 * game_nb_cols(g); j++) printf("-");
  printf("\n");
  for (uint i = 0; i < game_nb_rows(g); i++) {
    printf("  %d |", i);
    for (uint j = 0; j < game_nb_cols(g); j++) {
      constraint n = game_get_constraint(g, i, j);
      color c = game_get_color(g, i, j);
      char* ch = _square2str(n, c);
      printf("%s ", ch);
    }
    printf("|\n");
  }
  printf("     ");
  for (uint j = 0; j < 2 * game_nb_cols(g); j++) printf("-");
  printf("\n");
}

game game_default() {
  game g = game_new_ext(5, 5, default_constraints, default_colors, false, FULL);

  return g;
}

game game_default_solution(void) {
  game g =
      game_new_ext(5, 5, default_constraints, solution_default, false, FULL);
  return g;
}
