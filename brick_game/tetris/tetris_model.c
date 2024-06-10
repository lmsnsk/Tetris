#include "tetris_model.h"

void copy_figure(State_t *state, State_temp_t *temp_state) {
  for (int i = 0; i < FIG_SIZE; i++) {
    for (int j = 0; j < FIG_SIZE; j++) {
      temp_state->figure[i][j] = state->next[i][j];
    }
  }
}

void copy_field_to_temp(State_t *state, State_temp_t *temp_state) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      temp_state->field[i][j] = state->field[i][j];
    }
  }
}

void copy_field_from_temp(State_t *state, State_temp_t *temp_state) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      state->field[i][j] = temp_state->field[i][j];
    }
  }
}

int generate_random_fig() {
  srand(time(NULL));
  int random = rand() % FIG_COUNT;
  return random;
};

void empty_fig(State_t *state) {
  for (int i = 0; i < FIG_SIZE; i++) {
    for (int j = 0; j < FIG_SIZE; j++) {
      state->next[i][j] = 0;
    }
  }
}

void empty_current_fig(State_temp_t *state_temp) {
  for (int i = 0; i < FIG_SIZE; i++) {
    for (int j = 0; j < FIG_SIZE; j++) {
      state_temp->figure[i][j] = 0;
    }
  }
}

void update_figure(int random_fig) {
  State_t *state = getState();
  empty_fig(state);

  state->next_fig = random_fig;

  if (random_fig == I)
    state->next_wide = 4;
  else if (random_fig == O)
    state->next_wide = 2;
  else
    state->next_wide = 3;

  switch (random_fig) {
    case I:
      for (int i = 0; i < FIG_SIZE; i++) state->next[0][i] = '*';
      break;
    case L:
      state->next[1][2] = '*';
      for (int i = 0; i < FIG_SIZE - 1; i++) state->next[0][i] = '*';
      break;
    case J:
      state->next[1][0] = '*';
      for (int i = 0; i < FIG_SIZE - 1; i++) state->next[0][i] = '*';
      break;
    case S:
      for (int i = 0; i < FIG_SIZE - 2; i++) state->next[0][i] = '*';
      for (int i = 1; i < FIG_SIZE - 1; i++) state->next[1][i] = '*';
      break;
    case Z:
      for (int i = 0; i < FIG_SIZE - 2; i++) state->next[1][i] = '*';
      for (int i = 1; i < FIG_SIZE - 1; i++) state->next[0][i] = '*';
      break;
    case T:
      for (int i = 0; i < FIG_SIZE - 1; i++) state->next[0][i] = '*';
      state->next[1][1] = '*';
      break;
    case O:
      for (int i = 0; i < FIG_SIZE - 2; i++) {
        state->next[i][0] = '*';
        state->next[i][1] = '*';
      }
      break;
  }
}

void init_first_figure(UserAction_t action) {
  State_t *state = getState();
  State_temp_t *temp_state = get_temp_state();
  static int check_gen_figure = 0;
  if (!check_gen_figure) {
    update_figure(generate_random_fig());
    for (int i = 0; i < FIG_SIZE; i++) {
      for (int j = 0; j < FIG_SIZE; j++) {
        temp_state->figure[i][j] = state->next[i][j];
      }
    }
    state->fig = state->next_fig;
    state->wide = state->next_wide;
    check_gen_figure = 1;
  }
  if (action == Start && !state->start) update_figure(generate_random_fig());
}

State_temp_t *get_temp_state() {
  static State_temp_t state;
  return &state;
}

State_t *init_state(State_t *state) {
  State_temp_t *temp_state = get_temp_state();
  state->pos.x = (COLUMNS / 2 - 1);
  state->pos.y = -1;
  state->score = 0;
  state->high_score = 0;
  readHighScore(state);
  state->level = 1;
  state->start = 0;
  state->pause = 0;
  state->speed = START_SPEED;
  state->crash = 0;
  state->dir = Right;
  state->stateStatus = START;
  state->action = Down;
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      state->field[i][j] = 0;
      temp_state->field[i][j] = 0;
    }
  }
  return state;
}

int check_collision(State_t *state, State_temp_t *temp_state) {
  int check = 0;
  for (int i = 0; i < FIG_SIZE; i++) {
    for (int j = 0; j < FIG_SIZE; j++) {
      if (state->pos.y - i >= 0 && temp_state->figure[i][j] == '*') {
        if (state->pos.y >= ROWS ||
            temp_state->field[state->pos.y - i][j + state->pos.x] == '*') {
          check = 1;
        }
      }
    }
  }
  return check;
}

void draw_figure_in_field(State_t *state, State_temp_t *temp_state) {
  for (int i = 0; i < FIG_SIZE; i++) {
    for (int j = 0; j < FIG_SIZE; j++) {
      if (state->pos.y - i >= 0 && temp_state->figure[i][j] == '*') {
        if (state->pos.y < ROWS &&
            temp_state->field[state->pos.y - i][j + state->pos.x] != '*') {
          state->field[state->pos.y - i][j + state->pos.x] =
              temp_state->figure[i][j];
        }
      }
    }
  }
}

void case_I(State_t *state, State_temp_t *temp_state) {
  if (state->dir == Right) {
    state->dir = Down;
    state->wide = 1;
    for (int i = 0; i < FIG_SIZE; i++) temp_state->figure[i][0] = '*';
  } else {
    state->dir = Right;
    state->wide = 4;
    for (int i = 0; i < FIG_SIZE; i++) temp_state->figure[0][i] = '*';
  }
}

void case_Z(State_t *state, State_temp_t *temp_state) {
  if (state->dir == Right) {
    state->dir = Down;
    state->wide = 2;
    for (int i = 0; i < FIG_SIZE - 2; i++) temp_state->figure[i][0] = '*';
    for (int i = 1; i < FIG_SIZE - 1; i++) temp_state->figure[i][1] = '*';
  } else {
    state->dir = Right;
    state->wide = 3;
    for (int i = 0; i < FIG_SIZE - 2; i++) temp_state->figure[1][i] = '*';
    for (int i = 1; i < FIG_SIZE - 1; i++) temp_state->figure[0][i] = '*';
  }
}

void case_S(State_t *state, State_temp_t *temp_state) {
  if (state->dir == Right) {
    state->dir = Down;
    state->wide = 2;
    for (int i = 0; i < FIG_SIZE - 2; i++) temp_state->figure[i][1] = '*';
    for (int i = 1; i < FIG_SIZE - 1; i++) temp_state->figure[i][0] = '*';
  } else {
    state->dir = Right;
    state->wide = 3;
    for (int i = 0; i < FIG_SIZE - 2; i++) temp_state->figure[0][i] = '*';
    for (int i = 1; i < FIG_SIZE - 1; i++) temp_state->figure[1][i] = '*';
  }
}

void case_T(State_t *state, State_temp_t *temp_state) {
  if (state->dir == Right) {
    state->dir = Down;
    state->wide = 2;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[i][0] = '*';
    temp_state->figure[1][1] = '*';
  } else if (state->dir == Down) {
    state->dir = Left;
    state->wide = 3;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[1][i] = '*';
    temp_state->figure[0][1] = '*';
  } else if (state->dir == Left) {
    state->dir = Up;
    state->wide = 2;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[i][1] = '*';
    temp_state->figure[1][0] = '*';
  } else {
    state->dir = Right;
    state->wide = 3;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[0][i] = '*';
    temp_state->figure[1][1] = '*';
  }
}

void case_L(State_t *state, State_temp_t *temp_state) {
  if (state->dir == Right) {
    state->dir = Down;
    state->wide = 2;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[i][0] = '*';
    temp_state->figure[0][1] = '*';
  } else if (state->dir == Down) {
    state->dir = Left;
    state->wide = 3;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[1][i] = '*';
    temp_state->figure[0][0] = '*';
  } else if (state->dir == Left) {
    state->dir = Up;
    state->wide = 2;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[i][1] = '*';
    temp_state->figure[2][0] = '*';
  } else {
    state->dir = Right;
    state->wide = 3;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[0][i] = '*';
    temp_state->figure[1][2] = '*';
  }
}

void case_J(State_t *state, State_temp_t *temp_state) {
  if (state->dir == Right) {
    state->dir = Down;
    state->wide = 2;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[i][0] = '*';
    temp_state->figure[2][1] = '*';
  } else if (state->dir == Down) {
    state->dir = Left;
    state->wide = 3;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[1][i] = '*';
    temp_state->figure[0][2] = '*';
  } else if (state->dir == Left) {
    state->dir = Up;
    state->wide = 2;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[i][1] = '*';
    temp_state->figure[0][0] = '*';
  } else {
    state->dir = Right;
    state->wide = 3;
    for (int i = 0; i < FIG_SIZE - 1; i++) temp_state->figure[0][i] = '*';
    temp_state->figure[1][0] = '*';
  }
}

void rotate_figure(State_t *state, State_temp_t *temp_state) {
  if (state->fig != O) empty_current_fig(temp_state);
  switch (state->fig) {
    case I:
      case_I(state, temp_state);
      break;
    case Z:
      case_Z(state, temp_state);
      break;
    case S:
      case_S(state, temp_state);
      break;
    case T:
      case_T(state, temp_state);
      break;
    case L:
      case_L(state, temp_state);
      break;
    case J:
      case_J(state, temp_state);
      break;
  }
}

void shift(UserAction_t action) {
  State_t *state = getState();
  State_temp_t *temp_state = get_temp_state();
  static int check_shift = 0;
  if (action == Left) {
    if (state->pos.x > 0) state->pos.x -= 1;
    if (check_collision(state, temp_state)) state->pos.x += 1;
    check_shift++;
  } else if (action == Right) {
    if (state->pos.x < COLUMNS - state->wide) state->pos.x += 1;
    if (check_collision(state, temp_state)) state->pos.x -= 1;
    check_shift++;
  } else if (action == Action) {
    rotate_figure(state, temp_state);
    if (check_collision(state, temp_state) ||
        state->pos.x > COLUMNS - state->wide) {
      rotate_figure(state, temp_state);
      rotate_figure(state, temp_state);
      rotate_figure(state, temp_state);
    }
    check_shift++;
  }
  // if (check_shift > 3) {
  //   state->pos.y += 1;
  //   if (check_collision(state, temp_state)) state->crash = 1;
  //   check_shift = 0;
  // }
  // if (state->crash) state->pos.y -= 1;
}

void check_full_line(State_t *state, State_temp_t *temp_state) {
  int count = 0;
  for (int i = ROWS - 1; i >= 0; i--) {
    int check = 1;
    for (int j = 0; j < COLUMNS; j++) {
      if (state->field[i][j] == 0) {
        check = 0;
        break;
      }
    }
    if (check) {
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < COLUMNS; j++) {
          temp_state->field[k][j] = state->field[k - 1][j];
        }
      }
      copy_field_from_temp(state, temp_state);
      i++, count++;
    }
  }
  if (count == 1)
    state->score += 100;
  else if (count == 2)
    state->score += 300;
  else if (count == 3)
    state->score += 700;
  else if (count == 4)
    state->score += 1500;
}

void check_game_over(State_t *state) {
  for (int j = 0; j < COLUMNS; j++) {
    if (state->field[0][j] == '*') {
      state->level = -1;
      return;
    }
  }
}

void update_info(State_t *state) {
  for (int i = 10; i > 1; i--) {
    if (state->score >= NEXT_LEVEL * (i - 1)) {
      state->level = i;
      state->speed = START_SPEED - START_SPEED / 10 * (i - 1);
      break;
    }
  }
  if (state->score > state->high_score) saveHighScore(state);
}

void saveHighScore(State_t *state) {
  FILE *file = fopen("tetris_score.txt", "w");
  if (file) {
    fprintf(file, "%d", state->score);
    fclose(file);
  }
};

void readHighScore(State_t *state) {
  FILE *file = fopen("tetris_score.txt", "r");
  if (file) {
    int score = 0;
    if (fscanf(file, "%d", &score) == 1) state->high_score = score;
    fclose(file);
  }
};

void step_down(UserAction_t action) {
  State_t *state = getState();
  State_temp_t *temp_state = get_temp_state();
  if (state->crash) {
    copy_field_to_temp(state, temp_state);
    check_full_line(state, temp_state);
    state->fig = state->next_fig;
    state->wide = state->next_wide;
    copy_figure(state, temp_state);
    update_figure(generate_random_fig());
    state->pos.x = (COLUMNS / 2 - 1);
    state->pos.y = -1;
    state->crash = 0;
    state->dir = Right;
    update_info(state);
    check_game_over(state);
  } else {
    copy_field_from_temp(state, temp_state);
  }
  state->pos.y += 1;
  draw_figure_in_field(state, temp_state);
}

// void userInput(UserAction_t action, int hold) {
//   State_t *state = getState();
//   State_temp_t *temp_state = get_temp_state();
//   (void)hold;

//   init_first_figure(state, temp_state, action);

//   if (action == Pause) {
//     state->pause = !state->pause;
//   } else if (action == Start && !state->start) {
//     state->start = 1;
//   } else if (!state->pause && state->start && action != Up) {
//     moveFigure(state, temp_state, action);
//   }
// }
