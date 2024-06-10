#include "fms.h"

void game_loop() {
  GameInfo_t *info = getInfo();
  State_t *state = getState();
  getPressedKey();

  if (state->action == Terminate) {
    state->level = -1;
    updateCurrentState();
    return;
  }

  if (state->game_over) {
    state->stateStatus = GAME_OVER;
  }

  if (state->stateStatus == START) {
    start_game();
  }

  if (state->stateStatus == SPAWN) {
    init_first_figure(state->action);
  }

  if (state->stateStatus == SHIFT) {
    if (state->action == Pause) {
      state->action = Down;
      state->stateStatus = PAUSE;
      state->pause = 1;
    } else {
      shift();
      state->stateStatus = STEP;
    }
  }

  if (state->stateStatus == PAUSE) {
    if (state->action == Pause) {
      state->pause = 0;
      state->stateStatus = SHIFT;
    }
  }

  if (state->stateStatus == STEP) {
    step_down();
    updateCurrentState();
    draw(*info);
  }

  if (state->stateStatus == ATTACH) {
    attachFigure();
  }

  if (state->stateStatus == GAME_OVER) {
    state->level = -1;
    updateCurrentState();
  }
};

void start_game() {
  GameInfo_t *info = getInfo();
  State_t *state = getState();
  init_state(state);
  updateCurrentState();
  draw(*info);
  if (state->action == Start) state->stateStatus = SPAWN;
}

GameInfo_t updateCurrentState() {
  GameInfo_t *info = getInfo();
  State_t *state = getState();
  copy_field_to_info(info, state);
  info->high_score = state->high_score;
  info->pause = state->pause;
  info->score = state->score;
  info->speed = state->speed;
  info->level = state->level;
  return *info;
};

void getPressedKey() {
  State_t *state = getState();
  state->action = Down;
  int ch = getch();
  if (ch != ERR) {
    switch (ch) {
      case KEY_LEFT:
        state->action = Left;
        break;
      case KEY_RIGHT:
        state->action = Right;
        break;
      case KEY_DOWN:
        state->action = Down;
        break;
      case ' ':
        state->action = Action;
        break;
      case 'q':
      case 'Q':
        state->action = Terminate;
        break;
      case 'p':
      case 'P':
        state->action = Pause;
        break;
      case 'e':
      case 'E':
        state->action = Start;
        break;
    }
  }
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