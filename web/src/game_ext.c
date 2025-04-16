
#include "game_ext.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"
#include "queue.h"

/**
 * @brief Creates a new game with extended options and initializes it.
 * @param nb_rows number of rows in game
 * @param nb_cols number of columns in game
 * @param constraints an array describing the initial constraint of each square
 * using row-major storage (of size nb_rows*nb_cols)
 * @param colors an array describing the initial color of each square using
 * row-major storage  (of size nb_rows*nb_cols) or NULL to set all colors as
 * EMPTY
 * @param wrapping wrapping option
 * @param neigh neighbourhood option
 * @pre @p constraints must be an initialized array of default size squared
 * @pre @p colors must be an initialized array of default size squared or NULL
 * @return the created game**/
game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints,
                  color *colors, bool wrapping, neighbourhood neigh) {
  game g = malloc(sizeof(struct game_s));

  if (g != NULL) {
    if (colors == NULL) {
      g->row = nb_rows;
      g->column = nb_cols;
      g->wrapping = wrapping;
      g->neigh = neigh;
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
    g->row = nb_rows;
    g->column = nb_cols;
    g->wrapping = wrapping;
    g->neigh = neigh;
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

/**
 * @brief Creates a new empty game with extended options.
 * @details All squares are initialized with empty squares.
 * @param nb_rows number of rows in game
 * @param nb_cols number of columns in game
 * @param wrapping wrapping option
 * @param neigh neighbourhood option
 * @return the created game
 **/
game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        neighbourhood neigh) {
  game g = malloc(sizeof(struct game_s));
  if (g != NULL) {
    g->row = nb_rows;
    g->column = nb_cols;
    g->wrapping = wrapping;
    g->neigh = neigh;
    g->history = NULL;  // Initially no history
    g->history_size = 0;
    g->current_move = -1;           // No moves made yet
    g->played_moves = queue_new();  // Create the played moves queue
    g->undone_moves = queue_new();  // Create the undone moves queue

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

/**
 * @brief Gets the number of rows (or height).
 * @param g the game
 * @return the number of rows on this game
 * @pre @p g is a valid pointer toward a cgame structure
 **/
uint game_nb_rows(cgame g) {
  if (g != NULL) {
    return g->row;
  } else {
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Gets the number of columns (or width).
 * @param g the game
 * @return the the number of columns on this game
 * @pre @p g is a valid pointer toward a cgame structure
 **/
uint game_nb_cols(cgame g) {
  if (g != NULL) {
    return g->column;
  } else {
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Checks if the game has the wrapping option
 * @return true, if wrapping option is enabled, false otherwise
 * @pre @p g is a valid pointer toward a cgame structure
 **/
bool game_is_wrapping(cgame g) {
  assert(g != NULL);
  return g->wrapping;
}

/**
 * @brief Gets the neighbourhood option
 * @return the neighbourhood option
 * @pre @p g is a valid pointer toward a cgame structure
 **/
neighbourhood game_get_neighbourhood(cgame g) {
  assert(g != NULL);
  return g->neigh;
}

/**
 * @brief Undoes the last move.
 * @details Searches in the history the last move played (by calling
 * @ref game_play_move or @ref game_redo), and restores the state of the game
 * before that move. If no moves have been played, this function does nothing.
 * The @ref game_restart function clears the history.
 * @param g the game
 * @pre @p g is a valid pointer toward a cgame structure
 **/
void game_undo(game g) {
  if (!queue_is_empty(g->played_moves)) {
    move_t *move = queue_pop_tail(g->played_moves);

    // Undo the move in the game
    g->colors[move->i * g->column + move->j] = move->previous_color;

    // Add the move to the start of the undone_moves queue for potential redo
    queue_push_head(g->undone_moves, move);
  }
}

/**
 * @brief Redoes the last move.
 * @details Searches in the history the last cancelled move (by calling @ref
 * game_undo), and replays it. If there are no more moves to be replayed, this
 * function does nothing. After playing a new move with @ref game_play_move, it
 * is no longer possible to redo an old cancelled move.
 * @param g the game
 * @pre @p g is a valid pointer toward a cgame structure
 **/
void game_redo(game g) {
  if (!queue_is_empty(g->undone_moves)) {
    move_t *move = queue_pop_head(g->undone_moves);

    // Redo the move in the game
    g->colors[move->i * g->column + move->j] = move->applied_color;

    // Add the move back to the end of the played_moves queue
    queue_push_tail(g->played_moves, move);
  }
}