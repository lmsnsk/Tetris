#include "fms.h"

void game_loop() {
  UserAction_t status = Down;
  GameInfo_t *info = getInfo();
  State_t *state = getState();
  StateStatus stateStatus = state->stateStatus;

  if (state->game_over) {
    stateStatus = GAME_OVER;
  }

  if (stateStatus == START) {
    start_game(&status, &stateStatus);
  }

  if (stateStatus == SPAWN) {
    init_first_figure(status);
    stateStatus = SHIFT;
  }

  if (stateStatus == SHIFT) {
    status = getPressedKey();
    if (status == Pause) {
      stateStatus = PAUSE;
      getState()->pause = 1;
    } else {
      shift(status);
      stateStatus = ATTACH;
    }
  }

  if (stateStatus == ATTACH) {
    stateStatus = STEP;
  }

  if (stateStatus == PAUSE) {
    status = getPressedKey();
    userInput(status, 0);
    if (status == Terminate) getState()->level = -1;
    if (status == Pause) getState()->pause = 0;
    stateStatus = SHIFT;
  }

  if (stateStatus == STEP) {
    step_down(status);
    updateCurrentState();
    draw(*info);
    getState()->stateStatus = SHIFT;
  }

  if (stateStatus == GAME_OVER) {
    status = getPressedKey();
    if (status == Terminate) getState()->level = -1;
    if (!getState()->game_over) draw(*info);
    updateCurrentState();
  }
};

void start_game(UserAction_t *status, StateStatus *stateStatus) {
  GameInfo_t *info = getInfo();
  init_state(getState());
  draw(*info);
  *status = getPressedKey();
  if (*status == Start) *stateStatus = SPAWN;
  if (*status == Terminate) {
    getState()->level = -1;
    updateCurrentState();
  }
}

GameInfo_t updateCurrentState() {
  GameInfo_t *info = getInfo();
  State_t *state = getState();
  // static int check_init_state = 0;

  // if (!check_init_state) {
  //   init_state();
  //   check_init_state = 1;
  // }
  copy_field_to_info(info, state);
  info->high_score = state->high_score;
  info->pause = state->pause;
  info->score = state->score;
  info->speed = state->speed;
  info->level = state->level;
  return *info;
};

void userInput(UserAction_t action, int hold) {
  (void)hold;
  if (action == Pause) {
    getState()->action = Pause;
    getState()->stateStatus = PAUSE;
  } else if (action == Start) {
    getState()->action = Start;
    getState()->stateStatus = START;
  } else if (action == Up) {
    getState()->action = Up;
  } else if (action == Down) {
    getState()->action = Down;
  } else if (action == Left) {
    getState()->action = Left;
  } else if (action == Right) {
    getState()->action = Right;
  } else if (action == Terminate) {
    getState()->action = Terminate;
  } else if (action == Action) {
    getState()->action = Action;
  }
};

State_t *getState() {
  static State_t state;
  return &state;
}

void copy_field_to_info(GameInfo_t *info, State_t *state) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      info->field[i][j] = state->field[i][j];

      if (i < FIG_SIZE && j < FIG_SIZE) {
        info->next[i][j] = state->next[i][j];
      }
    }
  }
}