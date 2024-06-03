#ifndef S21_COMMON_H
#define S21_COMMON_H

#define ROWS 20
#define COLUMNS 10
#define FIG_SIZE 4

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

void userInput(UserAction_t action, int hold);
GameInfo_t updateCurrentState();

GameInfo_t *getInfo();

#endif  // S21_COMMON_H