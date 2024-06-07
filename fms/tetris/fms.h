#ifndef S21_FMS_TETRIS_H
#define S21_FMS_TETRIS_H

#include "../../brick_game/tetris/tetris_model.h"
#include "../../gui/cli/cli.h"

void game_loop();
void start_game(UserAction_t *status, StateStatus *stateStatus);
void copy_field_to_info(GameInfo_t *info, State_t *state);

#endif  // S21_FMS_TETRIS_H