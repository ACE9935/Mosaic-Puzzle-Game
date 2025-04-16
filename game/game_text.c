#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char *argv[]) {
  game g;

  if (argc > 1) {
    g = game_load(argv[1]);
    if (g == NULL) {
      fprintf(stderr, "Failed to load game from file: %s\n", argv[1]);
      exit(EXIT_FAILURE);
    }
  } else {
    g = game_default();
  }

  while (!(game_won(g))) {
    game_print(g);
    for (int i = 0; i < game_nb_rows(g); i++) {
      for (int j = 0; j < game_nb_cols(g); j++) {
        status myStatus = game_get_status(g, i, j);
        if (game_get_constraint(g, i, j) != -1 && myStatus == ERROR) {
          printf("errors:%d \n", game_get_constraint(g, i, j));
        }
      }
    }
    printf("Type a command ([h] for help, [s <filename>] to save): \n");

    char command[256];
    if (scanf("%255s", command) != 1) {
      fprintf(stderr, "Failed to read command\n");
      continue;
    }

    // Check if scanf successfully reads the input filename
    else if (command[0] == 's') {
      char filename[256];
      if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Failed to read filename\n");
        continue;
      }
      game_save(g, filename);
      printf("Game saved to %s\n", filename);
    } else if (command[0] == 'q') {
      printf("shame\n");
      break;
    } else if (command[0] == 'h') {
      printf("action : help \n");
      printf("- press 'w <i> <j> ' to set square (i,j) white\n");
      printf("- press 'b <i> <j> ' to set square (i,j) black\n");
      printf("- press 'e <i> <j> ' to set square (i,j) empty\n");
      printf("- press 'z' to undo\n");
      printf("- press 'y' to redo\n");
      printf("- press 'r' to restart\n");
      printf("- press 'q' to quit\n");
    } else if (command[0] == 'r') {
      game_restart(g);
    } else if (command[0] == 'z') {
      game_undo(g);
    } else if (command[0] == 'y') {
      game_redo(g);
    } else if (command[0] == 'w' || command[0] == 'b' || command[0] == 'e') {
      int i, j;
      // Check if scanf successfully reads the input row and column indices
      if (scanf("%d %d", &i, &j) != 2) {
        fprintf(stderr, "Failed to read row and column indices\n");
        continue;
      }

      if (i >= 0 && i < game_nb_rows(g) && j >= 0 && j < game_nb_cols(g)) {
        if (command[0] == 'w') {
          game_play_move(g, i, j, WHITE);

        } else if (command[0] == 'b') {
          game_play_move(g, i, j, BLACK);

        } else if (command[0] == 'e') {
          game_play_move(g, i, j, EMPTY);
        }
      }
    }
    game_print(g);
  }
  if (game_won(g)) {
    printf("congrats");
  }
  game_print(g);
  return EXIT_SUCCESS;
}
