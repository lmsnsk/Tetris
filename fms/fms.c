#include "fms.h"

void gameLoop() {
  UserAction_t status = Down;
  GameInfo_t *info = getInfo();
  StateStatus stateStatus = getState()->stateStatus;

  updateField();
  if (getState()->game_over) {
    stateStatus = GAME_OVER;
  }

  if (stateStatus == START) {
    startGame(status, stateStatus);
  }

  if (stateStatus == SPAWN) {
    // race->spawnEnemy();
    stateStatus = SHIFT;
  }

  if (stateStatus == SHIFT) {
    shiftCar(status, stateStatus);
  }

  if (stateStatus == PAUSE) {
    status = getPressedKey();
    if (status == Terminate) getState()->level = -1;
    if (status == Pause) getState()->pause = 0;
    stateStatus = SHIFT;
  }

  if (stateStatus == STEP) {
    step();
    updateCurrentState();
    draw(*info);
  }

  if (stateStatus == GAME_OVER) {
    status = getPressedKey();
    if (status == Terminate) getState()->level = -1;
    if (!getState()->game_over) draw(*info);
    updateCurrentState();
  }
};

void startGame(UserAction_t *status, StateStatus *stateStatus) {
  GameInfo_t *info = getInfo();
  draw(*info);
  status = getPressedKey();
  if (status == Start) stateStatus = SPAWN;
  if (status == Terminate) {
    getState()->level = -1;
    updateCurrentState();
  }
}

// void shiftCar(UserAction_t &status, StateStatus &stateStatus) {
//   Race *race = getRace();
//   GameInfo_t *info = getInfo();
//   RaceState_t *state = race->getState();
//   int &pause = state->pause;
//   status = state->first_step ? getPressedKey() : Down;
//   userInput(status, 0);
//   state->stateStatus = pause ? PAUSE : STEP;
//   if (!state->pause && (status == Right || status == Left)) {
//     race->shift(status);
//     updateCurrentState();
//     draw(*info);
//   }
//   if (status == Terminate) state->level = -1;
//   if (status == Pause) {
//     pause = !pause;
//     stateStatus = PAUSE;
//   }
//   status = Down;
// }

GameInfo_t updateCurrentState() {
  GameInfo_t *info = getInfo();
  State_t *state = getState();
  copyField(state->field, info->field);
  info->high_score = state->high_score;
  info->pause = state->pause;
  info->score = state->score;
  info->speed = state->speed;
  info->level = state->level;
  return *info;
};

void userInput(UserAction_t action, int hold) {
  (void)hold;
  if (action == Pause)
    getState()->action = Pause;
  else if (action == Start)
    getState()->action = Start;
  else if (action == Up)
    getState()->action = Up;
  else if (action == Down)
    getState()->action = Down;
  else if (action == Left)
    getState()->action = Left;
  else if (action == Right)
    getState()->action = Right;
  else if (action == Terminate)
    getState()->action = Terminate;
  else if (action == Action)
    getState()->action = Action;
};