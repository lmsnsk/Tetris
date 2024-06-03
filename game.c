#include "common.h"
#include "fms/fms.h"

void setup();
int initInfoField(GameInfo_t *info);
void clearInfo(GameInfo_t *info);

int main(void) {
  int error = 0;
  GameInfo_t *info = getInfo();

  setup();
  error = initInfoField(info);
  if (!error) {
    updateCurrentState();

    while (info->level != -1) {
      gameLoop();
      timeout(info->speed);
    }
  }
  clearInfo(info);
  endwin();
  return 0;
}

void setup() {
  initscr();
  clear();
  noecho();
  cbreak();
  keypad(stdscr, true);
  curs_set(false);
}

int initInfoField(GameInfo_t *info) {
  int error = 0;
  info->field = (int **)calloc(ROWS, sizeof(int *));
  info->next = (int **)calloc(FIG_SIZE, sizeof(int *));
  if (!info->field || !info->next) {
    error = 1;
  } else {
    for (int i = 0; i < ROWS; i++) {
      info->field[i] = (int *)calloc(ROWS, sizeof(int));
      if (!info->field[i]) error = 1;
    }
    for (int i = 0; i < FIG_SIZE; i++) {
      info->next[i] = (int *)calloc(FIG_SIZE, sizeof(int));
      if (!info->next[i]) error = 1;
    }
  }
  info->level = 1;
  return error;
}

void clearInfo(GameInfo_t *info) {
  for (int i = 0; i < ROWS; i++) free(info->field[i]);
  for (int i = 0; i < FIG_SIZE; i++) free(info->next[i]);
  free(info->field);
  free(info->next);
}

GameInfo_t *getInfo() {
  static GameInfo_t info;
  return &info;
}