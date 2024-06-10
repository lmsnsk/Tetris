#ifndef S21_BRICKGAME_TETRIS_MODEL_H
#define S21_BRICKGAME_TETRIS_MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../../common.h"

#define FIG_COUNT 7
#define START_SPEED 400
#define NEXT_LEVEL 600

typedef enum {
  START,
  PAUSE,
  SPAWN,
  SHIFT,
  STEP,
  ATTACH,
  GAME_OVER,
  WAITING
} StateStatus;

typedef struct {
  int x;
  int y;
} coord_t;

typedef struct {
  coord_t pos;
  int field[ROWS][COLUMNS];
  int next[FIG_SIZE][FIG_SIZE];
  int score;
  int high_score;
  int level;
  int speed;
  int start;
  int pause;
  int crash;
  int fig;
  int next_fig;
  int wide;
  int next_wide;
  int game_over;
  StateStatus stateStatus;
  UserAction_t action;
  UserAction_t dir;
} State_t;

typedef struct {
  int field[ROWS][COLUMNS];
  int figure[FIG_SIZE][FIG_SIZE];
} State_temp_t;

typedef enum { O, L, J, S, Z, T, I } Figure_t;

State_t *init_state(State_t *state);
void shift();
int generate_random_fig();
void update_figure(int random_fig);
void init_first_figure(UserAction_t action);
void copy_figure(State_t *state, State_temp_t *temp_state);
void check_full_line(State_t *state, State_temp_t *temp_state);
State_t *getState();
State_temp_t *get_temp_state();
void step_down();
int check_collision(State_t *state, State_temp_t *temp_state);
void draw_figure_in_field(State_t *state, State_temp_t *temp_state);
void empty_fig(State_t *state);
void empty_current_fig(State_temp_t *state_temp);
void rotate_figure(State_t *state, State_temp_t *temp_state);
void check_game_over();
void update_info(State_t *state);
void saveHighScore(State_t *state);
void readHighScore(State_t *state);
void attachFigure();

#endif  // S21_BRICKGAME_TETRIS_MODEL_H