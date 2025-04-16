#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_tools.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <option> <input> [<output>]\n", argv[0]);
    return EXIT_FAILURE;
  }

  char *option = argv[1];
  char *input_file = argv[2];
  char *output_file = (argc > 3) ? argv[3] : NULL;

  game g = game_load(input_file);
  if (g == NULL) {
    fprintf(stderr, "error loading game from file %s\n", input_file);
    return EXIT_FAILURE;
  }

  if (option[0] == '-' && option[1] == 's') {
    if (game_solve(g)) {
      if (output_file) {
        game_save(g, output_file);
      } else {
        game_print(g);
      }
    } else {
      fprintf(stderr, "no solution found for game.\n");
      game_delete(g);
      return EXIT_FAILURE;
    }
  } else if (option[0] == '-' && option[1] == 'c') {
    int num_solutions = game_nb_solutions(g);
    if (output_file) {
      FILE *f = fopen(output_file, "w");
      if (f == NULL) {
        fprintf(stderr, "error opening output file %s\n", output_file);
        game_delete(g);
        return EXIT_FAILURE;
      }
      fprintf(f, "%d\n", num_solutions);
      fclose(f);
    } else {
      printf("%d\n", num_solutions);
    }
  } else {
    fprintf(stderr, "invalid option %s\n", option);
    game_delete(g);
    return EXIT_FAILURE;
  }

  game_delete(g);
  return EXIT_SUCCESS;
}
