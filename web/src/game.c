#include "game.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_aux.h"
#include "game_struct.h"

game game_new(constraint *constraints, color *colors) {
  game g = malloc(sizeof(struct game_s));

  if (g != NULL) {
    if (colors == NULL) {
      g->row = DEFAULT_SIZE;
      g->column = DEFAULT_SIZE;
      g->wrapping = false;
      g->neigh = FULL;
      g->history = NULL;  // Initially no history
      g->history_size = 0;
      g->current_move = -1;           // No moves made yet
      g->played_moves = queue_new();  // Create the played moves queue
      g->undone_moves = queue_new();  // Create the undone moves queue
      g->constraints = malloc(g->row * g->column * sizeof(constraint));
      g->colors = malloc(g->row * g->column * sizeof(color));
      if (g->colors != NULL && g->constraints != NULL) {
        for (int i = 0; i < g->row * g->column; i++) {
          g->constraints[i] = constraints[i];
          g->colors[i] = EMPTY;
        }
        return g;
      }

      else {
        exit(EXIT_FAILURE);
      }
    }
    g->row = DEFAULT_SIZE;
    g->column = DEFAULT_SIZE;
    g->wrapping = false;
    g->neigh = FULL;
    g->history = NULL;  // Initially no history
    g->history_size = 0;
    g->current_move = -1;           // No moves made yet
    g->played_moves = queue_new();  // Create the played moves queue
    g->undone_moves = queue_new();  // Create the undone moves queue

    g->constraints = malloc(g->row * g->column * sizeof(constraint));
    g->colors = malloc(g->row * g->column * sizeof(color));
    if (g->colors != NULL && g->constraints != NULL) {
      for (int i = 0; i < g->row * g->column; i++) {
        g->constraints[i] = constraints[i];
        g->colors[i] = colors[i];
      }
      return g;
    }

    else {
      exit(EXIT_FAILURE);
    }
  } else {
    fprintf(stderr, "allocation mémoire achouée\n");
    exit(EXIT_FAILURE);
  }
}

game game_new_empty(void) {
  game g = malloc(sizeof(struct game_s));
  if (g != NULL) {
    g->row = DEFAULT_SIZE;
    g->column = DEFAULT_SIZE;
    g->wrapping = false;
    g->neigh = FULL;
    g->history = NULL;  // Initially no history
    g->history_size = 0;
    g->current_move = -1;           // No moves made yet
    g->played_moves = queue_new();  // Create the played moves queue
    g->undone_moves = queue_new();
    g->constraints = malloc(g->row * g->column * sizeof(constraint));
    g->colors = malloc(g->row * g->column * sizeof(color));
    if (g->colors != NULL && g->constraints != NULL) {
      for (int i = 0; i < g->row * g->column; i++) {
        g->constraints[i] = UNCONSTRAINED;
        g->colors[i] = EMPTY;
      }
      return g;  // Return the created game struct
    } else {
      // Free the allocated memory if one of the allocations failed
      free(g->constraints);
      free(g->colors);
      free(g);
      return NULL;  // Return NULL to indicate allocation failure
    }
  } else {
    exit(EXIT_FAILURE);  // Exit the program if the initial allocation fails
  }
}

game game_copy(cgame g) {
  if (g == NULL) {
    return NULL;
  }

  game copy = malloc(sizeof(struct game_s));
  if (copy != NULL) {
    copy->row = g->row;
    copy->column = g->column;
    copy->wrapping = false;
    copy->neigh = FULL;
    copy->history = NULL;  // Initially no history
    copy->history_size = 0;
    copy->current_move = -1;           // No moves made yet
    copy->played_moves = queue_new();  // Create the played moves queue
    copy->undone_moves = queue_new();
    copy->constraints = malloc(copy->row * copy->column * sizeof(constraint));
    copy->colors = malloc(copy->row * copy->column * sizeof(color));
    if (copy->colors != NULL && copy->constraints != NULL) {
      for (int i = 0; i < copy->row * copy->column; i++) {
        copy->constraints[i] = g->constraints[i];
        copy->colors[i] = g->colors[i];
      }
      copy->wrapping = g->wrapping;
      copy->neigh = g->neigh;

      return copy;
    } else {
      fprintf(stderr, "Allocation mémoire échouée\n");
      exit(EXIT_FAILURE);
    }
  } else {
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(EXIT_FAILURE);
  }
}

bool game_equal(cgame g1, cgame g2) {
  if (g1 == NULL || g2 == NULL) {
    return false;
  }
  if (g1->row != g2->row || g1->column != g2->column ||
      g1->wrapping != g2->wrapping || g1->neigh != g2->neigh) {
    return false;
  }

  for (int i = 0; i < g1->row * g1->column; i++) {
    if (g1->constraints[i] != g2->constraints[i] ||
        g1->colors[i] != g2->colors[i]) {
      return false;
    }
  }

  return true;
}

void game_delete(game g) {
  if (g != NULL) {
    free(g->constraints);
    free(g->colors);
    free(g->history);
    queue_free(g->played_moves);
    queue_free(g->undone_moves);
    free(g);
  }
}

void game_set_constraint(game g, uint i, uint j, constraint n) {
  if (g != NULL && i < g->row && j < g->column) {
    g->constraints[i * g->column + j] = n;
  } else {
    exit(EXIT_FAILURE);
  }
}

void game_set_color(game g, uint i, uint j, color c) {
  if (g != NULL && i < g->row && j < g->column) {
    g->colors[i * g->column + j] = c;
  } else {
    exit(EXIT_FAILURE);
  }
}

constraint game_get_constraint(cgame g, uint i, uint j) {
  if (g != NULL && i < g->row && j < g->column) {
    return g->constraints[i * g->column + j];
  } else {
    exit(EXIT_FAILURE);
  }
}

color game_get_color(cgame g, uint i, uint j) {
  if (g != NULL && i < g->row && j < g->column) {
    return g->colors[i * g->column + j];
  } else {
    exit(EXIT_FAILURE);
  }
}

bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint *pi_next,
                          uint *pj_next) {
  assert(pi_next != NULL && pj_next != NULL);

  if (g == NULL || i >= g->row || j >= g->column) {
    exit(EXIT_FAILURE);
  }

  int ni = i, nj = j;

  switch (dir) {
    case UP:
      ni = i > 0 ? i - 1 : (g->wrapping ? g->row - 1 : -1);
      break;
    case DOWN:
      ni = i < g->row - 1 ? i + 1 : (g->wrapping ? 0 : -1);
      break;
    case LEFT:
      nj = j > 0 ? j - 1 : (g->wrapping ? g->column - 1 : -1);
      break;
    case RIGHT:
      nj = j < g->column - 1 ? j + 1 : (g->wrapping ? 0 : -1);
      break;
    case UP_LEFT:
    case UP_RIGHT:
    case DOWN_LEFT:
    case DOWN_RIGHT:
    case HERE:
    default:
      break;
  }

  if (ni < 0 || nj < 0 || ni >= g->row || nj >= g->column) {
    return false;
  }

  *pi_next = ni;
  *pj_next = nj;
  return true;
}

uint game_rows(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "Invalid game pointer\n");
    return 0;
  }
  return g->row;
}

uint game_cols(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "Invalid game pointer\n");
    return 0;
  }
  return g->column;
}

int game_nb_neighbors(cgame g, uint i, uint j, color c) {
  int count = 0;
  neighbourhood nh_type = g->neigh;
  bool wrapping = g->wrapping;  // Check if wrapping is enabled

  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      if ((nh_type == FULL_EXCLUDE || nh_type == ORTHO_EXCLUDE) && x == 0 &&
          y == 0)
        continue;
      if ((nh_type == ORTHO || nh_type == ORTHO_EXCLUDE) && x != 0 && y != 0)
        continue;

      int ni = i + x;
      int nj = j + y;

      // Apply wrapping if enabled
      if (wrapping) {
        ni = (ni + g->row) % g->row;
        nj = (nj + g->column) % g->column;
      } else {
        // Skip neighbors outside the grid if wrapping is not enabled
        if (ni < 0 || ni >= g->row || nj < 0 || nj >= g->column) continue;
      }

      if (game_get_color(g, ni, nj) == c) {
        count++;
      }
    }
  }
  return count;
}

int game_max_neighbors(cgame g, uint i, uint j) {
  bool wrapping = g->wrapping;
  int max_neigh;

  if (wrapping) {
    // All cells have 9 neighbors when wrapping is enabled
    max_neigh = 9;
  } else {
    // Count neighbors for cells at edges or corners
    int row = g->row - 1;
    int column = g->column - 1;

    if ((i == 0 || i == row) && (j == 0 || j == column)) {
      // Corner cells
      max_neigh = 4;
    } else if (i == 0 || i == row || j == 0 || j == column) {
      // Cells at the edges
      max_neigh = 6;
    } else {
      // Cells not at the edges or corners
      max_neigh = 9;
    }
  }
  return max_neigh;
}
int count_neighbors_of_color(cgame g, uint i, uint j, color c) {
  int count = 0;
  bool wrapping = g->wrapping;  // Check if wrapping is enabled

  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      int ni = i + x;
      int nj = j + y;

      if (wrapping) {
        // Adjust indices for wrapping
        ni = (ni + g->row) % g->row;
        nj = (nj + g->column) % g->column;
      } else {
        // Check for boundaries of the grid
        if (ni < 0 || ni >= g->row || nj < 0 || nj >= g->column) continue;
      }

      if (game_get_color(g, ni, nj) == c) count++;
    }
  }

  return count;
}

status game_get_status(cgame g, uint i, uint j) {
  constraint cnt = game_get_constraint(g, i, j);

  if (cnt == UNCONSTRAINED) {
    if (game_nb_neighbors(g, i, j, EMPTY) == 0) {
      return SATISFIED;
    }
    return UNSATISFIED;
  }

  int blackNeighbors = game_nb_neighbors(g, i, j, BLACK);

  if (blackNeighbors > cnt) {
    return ERROR;
  } else if (blackNeighbors < cnt && game_nb_neighbors(g, i, j, EMPTY) == 0) {
    return ERROR;
  } else if (blackNeighbors < cnt && game_nb_neighbors(g, i, j, EMPTY) != 0) {
    return UNSATISFIED;
  } else if (blackNeighbors == cnt && game_nb_neighbors(g, i, j, EMPTY) == 0) {
    return SATISFIED;
  } else
    return SATISFIED;
}

void game_play_move(game g, uint i, uint j, color c) {
  if (g == NULL || i >= g->row || j >= g->column) {
    fprintf(stderr, "Invalid move: Out of bounds\n");
    return;
  }

  if (c != BLACK && c != WHITE && c != EMPTY) {
    fprintf(stderr, "Invalid move: Unknown color\n");
    return;
  }

  move_t *move = malloc(sizeof(move_t));
  move->i = i;
  move->j = j;
  move->previous_color =
      g->colors[i * g->column + j];  // Assuming this accesses the current color
  move->applied_color = c;

  // Apply the move to the game
  g->colors[i * g->column + j] = c;

  // Add the move to the end of the played_moves queue
  queue_push_tail(g->played_moves, move);

  // Clear the undone_moves queue as a new move invalidates the redo history
  queue_clear_full(g->undone_moves, free);
}

bool game_won(cgame g) {
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  for (uint i = 0; i < g->row; i++) {
    for (uint j = 0; j < g->column; j++) {
      if (game_get_color(g, i, j) == EMPTY) {
        return false;
      }
      if (game_get_status(g, i, j) != SATISFIED) {
        return false;
      }
    }
  }

  return true;
}

void game_restart(game g) {
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  for (uint i = 0; i < g->row; i++) {
    for (uint j = 0; j < g->column; j++) {
      g->colors[i * g->column + j] = EMPTY;
    }
  }
  queue_clear(g->played_moves);
  queue_clear(g->undone_moves);
}
