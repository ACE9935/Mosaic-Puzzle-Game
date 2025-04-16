#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"

typedef struct move_s {
  uint i, j;             // Coordinates of the move
  color previous_color;  // Color before the move was made
  color applied_color;
} move_t;

typedef struct game_s {
  uint row;
  uint column;
  constraint *constraints;
  color *colors;
  bool wrapping;
  neighbourhood neigh;
  move_t *history;       // Array to store history of moves
  int history_size;      // Size of the history array
  int current_move;      // Index of the current move in the history
  int history_capacity;  // Capacity of the history array
  queue *played_moves;   // Queue for moves that have been played
  queue *undone_moves;   // Queue for moves that have been undone

} * game;

#endif