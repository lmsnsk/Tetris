#include "fms.h"

void game_loop() {
  GameInfo_t *info = getInfo();
  State_t *state = getState();

  if (state->action == Terminate) {
    getState()->level = -1;
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
    state->stateStatus = SHIFT;
  }

  if (state->stateStatus == SHIFT) {
    getPressedKey();
    if (state->action == Pause) {
      state->stateStatus = PAUSE;
      getState()->pause = 1;
      state->action = Down;
    } else {
      shift(state->action);
      state->stateStatus = STEP;
    }
  }

  if (state->stateStatus == PAUSE) {
    getPressedKey();
    if (state->action == Pause) {
      getState()->pause = 0;
      state->action = Down;
      state->stateStatus = SHIFT;
      updateCurrentState();
      draw(*info);
    }
  }

  if (state->stateStatus == STEP) {
    step_down(state->action);
    updateCurrentState();
    draw(*info);
    getState()->stateStatus = ATTACH;
  }

  if (state->stateStatus == ATTACH) {
    attachFigure();
  }

  if (state->stateStatus == GAME_OVER) {
    // state->action = getPressedKey();
    // if (!getState()->game_over) draw(*info);
    // updateCurrentState();
  }

  // draw(*info);
};

void start_game() {
  GameInfo_t *info = getInfo();
  State_t *state = getState();
  init_state(state);
  updateCurrentState();
  draw(*info);
  getPressedKey();
  if (state->action == Start) state->stateStatus = SPAWN;
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

void getPressedKey() {
  State_t *state = getState();
  int ch = getch();
  if (ch != ERR) {
    if (ch == 'q' || ch == 'Q') {
      state->action = Terminate;
    } else if (ch == 'p' || ch == 'P') {
      state->action = Pause;
    } else if (ch == 'e' || ch == 'E') {
      state->action = Start;
    } else if (ch == KEY_DOWN) {
      state->action = Down;
    } else if (ch == KEY_UP) {
      state->action = Up;
    } else if (ch == KEY_LEFT) {
      state->action = Left;
    } else if (ch == KEY_RIGHT) {
      state->action = Right;
    } else if (ch == ' ') {
      state->action = Action;
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