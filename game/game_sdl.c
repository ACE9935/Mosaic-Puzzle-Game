// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "game_sdl.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"
#include "queue.h"

/* **************************************************************** */

#define MIN_CELL_SIZE 30  // Minimum size for each cell in the grid
#define FONT_PATH "res/arial.ttf"
#define FONT_SIZE 14
#define STATE_NONE 0
#define STATE_BLACK 1
#define STATE_WHITE 2

#define NUM_BUTTONS 5

typedef struct {
  SDL_Rect rect;  // x, y, w, h
  int id;         // An identifier for the button
  char text[50];  // Text displayed on the button
} Button;

struct Env_t {
  TTF_Font *font;
  SDL_Color textColor;
  int cellSize;       // Size of each grid cell
  bool cellSelected;  // Indicates if a cell is selected
  int selectedRow;    // Row of the selected cell
  int selectedCol;    // Column of the selected cell

  game game_instance;
  Button buttons[NUM_BUTTONS];
};

void afficherTexte(SDL_Renderer *renderer, int x, int y, const char *texte,
                   TTF_Font *police, SDL_Color couleur) {

  SDL_Surface *surface = TTF_RenderText_Solid(police, texte, couleur);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect destRect = {x, y, surface->w, surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &destRect);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);

}

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
    // Allocation de mémoire pour l'environnement
    Env *env = malloc(sizeof(struct Env_t));
    if (!env) {
        printf("Échec de l'allocation de l'environnement.\n");
        exit(EXIT_FAILURE);
    }

    // Chargement du jeu à partir d'un fichier si un fichier est spécifié en argument
    if (argc > 1) {
        env->game_instance = game_load(argv[1]);
        if (env->game_instance == NULL) {
            fprintf(stderr, "Échec du chargement du jeu à partir du fichier : %s\n", argv[1]);
        }
    } else {
        env->game_instance = game_default();
    }

    // Initialisation de la couleur du texte et de la police
    env->textColor = (SDL_Color){0, 128, 128, 255};  // Bleu
    env->font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!env->font) {
        printf("Échec du chargement de la police : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    // Définition de la taille initiale des cellules
    env->cellSize = MIN_CELL_SIZE;

    // Initialisation de la sélection de la cellule
    env->cellSelected = false;
    env->selectedRow = -1;
    env->selectedCol = -1;

    // Labels des boutons
    const char *buttonLabels[NUM_BUTTONS] = {
        "Recommencer", "Annuler Mouvement", "Refaire Mouvement", "Résoudre", "Jeu Aléatoire"};

    int winWidth, winHeight;
    SDL_GetWindowSize(win, &winWidth, &winHeight);

    int buttonWidth = 100;
    int buttonHeight = 30;
    int spacing = 20;  // Espacement entre les boutons, à ajuster selon besoin

    // Calcul de la largeur totale de tous les boutons et de l'espacement
    int totalButtonWidth = NUM_BUTTONS * buttonWidth + (NUM_BUTTONS - 1) * spacing;

    // Calcul de la position X de départ pour le premier bouton pour les centrer
    int startX = (winWidth - totalButtonWidth) / 2;

    // Définition de la position Y pour tous les boutons (en haut, centré)
    int buttonY = 10;  // À ajuster selon besoin

    // Initialisation des boutons
    for (int i = 0; i < NUM_BUTTONS; i++) {
        env->buttons[i].rect.x = startX + i * (buttonWidth + spacing);
        env->buttons[i].rect.y = buttonY;
        env->buttons[i].rect.w = buttonWidth;
        env->buttons[i].rect.h = buttonHeight;
        env->buttons[i].id = i + 1;  // Assignation d'un ID unique à chaque bouton
        strncpy(env->buttons[i].text, buttonLabels[i], sizeof(env->buttons[i].text));
        env->buttons[i].text[sizeof(env->buttons[i].text) - 1] = '\0';  // Assure la null-termination
    }

    return env;
}


/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  // Clear screen with a grey background
  SDL_SetRenderDrawColor(ren, 188, 188, 188, 188);  // grey
  SDL_RenderClear(ren);

  // Get window size
  int winWidth, winHeight;
  SDL_GetWindowSize(win, &winWidth, &winHeight);

  // Calculate total grid size
  int gridWidth = env->cellSize * game_nb_cols(env->game_instance);
  int gridHeight = env->cellSize * game_nb_rows(env->game_instance);

  // Calculate offset to center the grid
  int offsetX = (winWidth - gridWidth) / 2;
  int offsetY = (winHeight - gridHeight) / 2;

  // Set the draw color for grid lines
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);  // Black

  char textBuffer[12];
  // Buffer for text rendering

  // Draw vertical lines and column numbers
  for (int col = 0; col <= game_nb_cols(env->game_instance); col++) {
    int x = col * env->cellSize + offsetX;
    SDL_RenderDrawLine(ren, x, offsetY, x, gridHeight + offsetY);
  }

  // Draw horizontal lines and row numbers

  for (int row = 0; row <= game_nb_rows(env->game_instance); row++) {
    int y = row * env->cellSize + offsetY;
    SDL_RenderDrawLine(ren, offsetX, y, gridWidth + offsetX, y);
  }

  for (int row = 0; row < game_nb_rows(env->game_instance); row++) {
    for (int col = 0; col < game_nb_cols(env->game_instance); col++) {
      int cellX = col * env->cellSize + offsetX;
      int cellY = row * env->cellSize + offsetY;
      SDL_Rect cellRect = {cellX, cellY, env->cellSize, env->cellSize};

      // Set squares' colors
      switch (game_get_color(env->game_instance, row, col)) {
        case BLACK:
          SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);  // Black
          SDL_RenderFillRect(ren, &cellRect);
          break;
        case WHITE:
          SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);  // White
          SDL_RenderFillRect(ren, &cellRect);
          break;

        default:

          break;
      }
    }
  }
  if (game_won(env->game_instance)) {
    int textWidth, textHeight;
    TTF_SizeText(env->font, "Game Won", &textWidth, &textHeight);
    int textX = (winWidth - textWidth) / 2;
    int textY = offsetY + gridHeight + 20;
    renderText(ren, textX, textY, "Game Won", env->font,
               (SDL_Color){0, 128, 128, 255});
  }

for (int col = 0; col < game_nb_cols(env->game_instance); col++) {
    for (int row = 0; row < game_nb_rows(env->game_instance); row++) {
        int x = col * env->cellSize + offsetX;
        int y = row * env->cellSize + offsetY;
        SDL_RenderDrawLine(ren, offsetX, y, gridWidth + offsetX, y);

        int constraint = game_get_constraint(env->game_instance, row, col);
        if (constraint != -1) {
            SDL_Color textColor = (game_get_status(env->game_instance, row, col) == ERROR) ? (SDL_Color){255, 0, 1, 0} : env->textColor;
            sprintf(textBuffer, "%d", constraint);
            renderText(ren, x + 5, y + 5, textBuffer, env->font, textColor);
        }
    }
}


  // Calculate button positions based on the current window size
  int buttonWidth = 100;
  int buttonHeight = 30;
  int spacing = 20;  // Space between buttons
  int totalButtonWidth =
      NUM_BUTTONS * buttonWidth + (NUM_BUTTONS - 1) * spacing;
  int startX = (winWidth - totalButtonWidth) / 2;
  int buttonY = 10;

  for (int i = 0; i < NUM_BUTTONS; i++) {
    env->buttons[i].rect.x = startX + i * (buttonWidth + spacing);
    env->buttons[i].rect.y = buttonY;
    env->buttons[i].rect.w = buttonWidth;
    env->buttons[i].rect.h = buttonHeight;
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    SDL_SetRenderDrawColor(ren, 120, 120, 255, 255);  // Button color
    SDL_RenderFillRect(ren, &env->buttons[i].rect);

    // Render button text
    renderText(ren, env->buttons[i].rect.x + 5,
               env->buttons[i].rect.y + (env->buttons[i].rect.h / 4),
               env->buttons[i].text, env->font,
               (SDL_Color){255, 255, 255, 255});
  }

  SDL_RenderPresent(ren); /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    return true;
  } else if (e->type == SDL_MOUSEBUTTONDOWN) {
    // Get click position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Calculate grid offset based on window size
    int winWidth, winHeight;
    SDL_GetWindowSize(win, &winWidth, &winHeight);
    int offsetX =
        (winWidth - env->cellSize * game_nb_cols(env->game_instance)) / 2;
    int offsetY =
        (winHeight - env->cellSize * game_nb_rows(env->game_instance)) / 2;

    // Check if click is within the grid
    if (mouseX >= offsetX &&
        mouseX < offsetX + env->cellSize * game_nb_cols(env->game_instance) &&
        mouseY >= offsetY &&
        mouseY < offsetY + env->cellSize * game_nb_rows(env->game_instance)) {
      int clickedCol = (mouseX - offsetX) / env->cellSize;
      int clickedRow = (mouseY - offsetY) / env->cellSize;

      // color squares on click
      if (game_get_color(env->game_instance, clickedRow, clickedCol) == EMPTY) {
        game_play_move(env->game_instance, clickedRow, clickedCol, BLACK);
      } else if (game_get_color(env->game_instance, clickedRow, clickedCol) ==
                 BLACK) {
        game_play_move(env->game_instance, clickedRow, clickedCol, WHITE);
      } else if (game_get_color(env->game_instance, clickedRow, clickedCol) ==
                 WHITE) {
        game_play_move(env->game_instance, clickedRow, clickedCol, EMPTY);
      }
    }
    for (int i = 0; i < NUM_BUTTONS; i++) {
      if (SDL_PointInRect(&((SDL_Point){mouseX, mouseY}),
                          &env->buttons[i].rect)) {
        switch (env->buttons[i].id) {
          case 1:  // Restart Game
            game_restart(env->game_instance);
            for (int row = 0; row < game_nb_rows(env->game_instance); ++row) {
              for (int col = 0; col < game_nb_cols(env->game_instance); ++col) {
                game_set_color(env->game_instance, row, col, EMPTY);
              }
            }

            break;
          case 2:  // Undo Move

            game_undo(env->game_instance);

            break;
          case 3:  // Redo Move
            game_redo(env->game_instance);
            break;
          case 4:  // Solve Game

            game_solve(env->game_instance);

            break;
          case 5:

            srand(time(NULL));  // initialize radom seed with current time
            bool wrapping = false;
            neighbourhood neigh = FULL;
            game random = game_random(4, 4, wrapping, neigh, true, 0.6f, 0.5f);
            if (game_nb_solutions(random) != 0) {
              env->game_instance = random;
              game_restart(env->game_instance);

            } else {
              while (game_nb_solutions(random) == 0) {
                random = game_random(4, 4, wrapping, neigh, false, 0.6f, 0.5f);
                if (game_nb_solutions(random) != 0) {
                  env->game_instance = random;
                  game_restart(env->game_instance);
                  break;
                }
              }
            }
        }
        return false;
      }
    }
    if (game_won(env->game_instance)) {
      printf("game won");
    }
  }

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */
  if (env->font) {
    TTF_CloseFont(env->font);
  }
  free(env);
}

/* **************************************************************** */
