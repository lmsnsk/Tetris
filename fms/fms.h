#ifndef S21_FSM_TETRIS_H
#define S21_FSM_TETRIS_H

#include "../brick_game/tetris/tetris_model.h"
#include "../gui/cli/cli.h"

typedef enum { START, PAUSE, SPAWN, SHIFT, STEP, GAME_OVER } StateStatus;

void gameLoop();
void startGame(UserAction_t *status, StateStatus *stateStatus);
void shiftCar(UserAction_t *status, StateStatus *stateStatus);

#endif  // S21_FSM_TETRIS_H