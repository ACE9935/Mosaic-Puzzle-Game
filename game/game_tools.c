#include "game_tools.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"

game game_load(char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    return NULL;
  }

  int nb_rows, nb_cols, wrapping, neigh;
  if (fscanf(file, "%d %d %d %d\n", &nb_rows, &nb_cols, &wrapping, &neigh) !=
      4) {
    fprintf(stderr, "Failed to read the game configuration properly.\n");
    fclose(file);
    return NULL;
  }

  constraint *constraints = malloc(nb_rows * nb_cols * sizeof(constraint));
  color *colors = malloc(nb_rows * nb_cols * sizeof(color));

  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      char constraint_char = fgetc(file);  // Read character from file
      char color_char = ' ';  // Initialize color_char to a default value

      // Check if reading from the file is successful
      if (constraint_char == EOF) {
        fprintf(stderr, "Error reading from file\n");
        // Handle the error appropriately, e.g., return NULL
        fclose(file);
        free(constraints);
        free(colors);
        return NULL;
      }

      // Check if the color character is read successfully
      if (fscanf(file, " %c", &color_char) != 1) {
        fprintf(stderr, "Error reading color character\n");
        // Handle the error appropriately, e.g., return NULL
        fclose(file);
        free(constraints);
        free(colors);
        return NULL;
      }

      constraints[i * nb_cols + j] =
          (constraint_char == '-') ? UNCONSTRAINED : constraint_char - '0';
      switch (color_char) {
        case 'e':
          colors[i * nb_cols + j] = EMPTY;
          break;
        case 'w':
          colors[i * nb_cols + j] = WHITE;
          break;
        case 'b':
          colors[i * nb_cols + j] = BLACK;
          break;
        default:
          fprintf(stderr, "Unexpected color character: %c\n", color_char);
          // Handle unexpected character appropriately
          break;
      }
    }
    fgetc(file);  // To read the newline character
  }

  fclose(file);

  game g = game_new_ext(nb_rows, nb_cols, constraints, colors, wrapping,
                        (neighbourhood)neigh);
  free(constraints);
  free(colors);

  return g;
}

void game_save(cgame g, char *filename) {
  FILE *file = fopen(filename, "w");
  if (!file) {
    fprintf(stderr, "Cannot open file %s for writing\n", filename);
    return;
  }

  fprintf(file, "%d %d %d %d\n", game_nb_rows(g), game_nb_cols(g),
          game_is_wrapping(g), game_get_neighbourhood(g));

  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      char constraint_char = game_get_constraint(g, i, j) == UNCONSTRAINED
                                 ? '-'
                                 : '0' + game_get_constraint(g, i, j);
      char color_char = '\0';  // Initialize color_char to a default value
      switch (game_get_color(g, i, j)) {
        case EMPTY:
          color_char = 'e';
          break;
        case WHITE:
          color_char = 'w';
          break;
        case BLACK:
          color_char = 'b';
          break;
      }
      fprintf(file, "%c%c", constraint_char, color_char);
    }
    fprintf(file, "\n");
  }

  fclose(file);
}

bool set_and_check_game(game g, const char *word) {
  int nb_rows = game_nb_rows(g);
  int nb_cols = game_nb_cols(g);
  int total_cells = nb_rows * nb_cols;

  for (int i = 0; i < total_cells; ++i) {
    if (g->constraints[i] != 0 || word[i] != '1') {
      int row = i / nb_cols;
      int col = i % nb_cols;
      color c = (word[i] == '1') ? BLACK : WHITE;
      game_set_color(g, row, col, c);
    }
  }

  return game_won(g);
}

bool game_check_no_solution(cgame g) {
  for (uint i = 0; i < game_nb_rows(g); ++i) {
    for (uint j = 0; j < game_nb_cols(g); ++j) {
      if (game_get_constraint(g, i, j) != UNCONSTRAINED &&
          game_get_color(g, i, j) == EMPTY) {
        return true;
      }
    }
  }
  return false;
}

bool game_solve(game g) {
  int total_cells = game_nb_rows(g) * game_nb_cols(g);
  unsigned int max_number = 1 << total_cells;  // 2^total_cells
  char binary_word[total_cells + 1];

  // Check if the game  and has no solution
  if (game_nb_solutions(g) == 0) {
    return false;  // No solution for the game
  } else {
    for (unsigned int i = 0; i < max_number; ++i) {
      for (int j = 0; j < total_cells; ++j) {
        binary_word[j] = (i & (1 << j)) ? '1' : '0';
      }
      binary_word[total_cells] = '\0';  // Null-terminate the string

      if (set_and_check_game(g, binary_word)) {
        return true;  // Found a winning configuration
      }
    }
  }
  return false;
}

uint game_nb_solutions_helper(game g, uint start_row, uint start_col) {
  uint count = 0;
  if (game_won(g)) return 1;

  for (uint i = start_row; i < game_nb_rows(g); i++) {
    for (uint j = (i == start_row ? start_col : 0); j < game_nb_cols(g); j++) {
      if (game_get_color(g, i, j) == EMPTY) {
        game_set_color(g, i, j, BLACK);
        count += game_nb_solutions_helper(g, i, j);
        game_set_color(g, i, j, EMPTY);
      }
    }
  }
  return count;
}

uint game_nb_solutions(cgame g) {
  game g2 = game_copy(g);
  uint counter = 0;
  int total_cells = game_nb_rows(g2) * game_nb_cols(g2);
  unsigned int max_number = 1 << total_cells;  // 2^total_cells
  char binary_word[total_cells + 1];

  for (unsigned int i = 0; i < max_number; ++i) {
    for (int j = 0; j < total_cells; ++j) {
      binary_word[j] = (i & (1 << j)) ? '1' : '0';
    }
    binary_word[total_cells] = '\0';  // Null-terminate the string

    if (set_and_check_game(g2, binary_word)) {
      counter++;  // Found a winning configuration
    }
  }
  return counter;
}

game game_random(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh,
                 bool with_solution, float black_rate, float constraint_rate) {
  assert(black_rate >= 0.0f && black_rate <= 1.0f);
  assert(constraint_rate >= 0.0f && constraint_rate <= 1.0f);
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);
  assert(g);

  // fill the grid with random colors
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      color c = (rand() < black_rate * (float)RAND_MAX) ? BLACK : WHITE;
      game_set_color(g, i, j, c);
    }
  }

  // fill the grid with actual constraint at random positions
  uint nb_squares = nb_rows * nb_cols;
  uint nb_constraints = constraint_rate * nb_squares;
  for (uint i = 0; i < nb_constraints; i++) {
    uint row = rand() % nb_rows;
    uint col = rand() % nb_cols;
    int nb_blacks = game_nb_neighbors(g, row, col, BLACK);
    game_set_constraint(g, row, col, nb_blacks);
  }

  // check solution
  if (!game_won(g)) {
    game_delete(g);
    return NULL;
  }

  if (!with_solution) game_restart(g);
  return g;
}