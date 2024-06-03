CC=g++
FLAGS=-Wall -Werror -Wextra -std=c++17
FLAGCUR=-lncurses
ASAN=-g -fsanitize=address
SNAKE_LIB=snake.a
TETRIS_LIB=tetris.a
SNAKE_FILES=controller/controller.cpp $(SNAKE_LIB) gui/cli/cli.c
TETRIS_FILES=controller/controller.cpp $(TETRIS_LIB) gui/cli/cli.c
FILETEST=tests/*.cpp controller/controller.cpp brick_game/snake/*.cpp
FILEREPORT=s21_test_report
FILETESTO=s21_test
DIRREPORT=report
COVFLAG=-fprofile-arcs -ftest-coverage

OS = $(shell uname)

ifeq ($(OS), Darwin)
	TESTFLAGS=-pthread -lcheck -lgtest -lstdc++
	OPEN_CMD=open
else
	TESTFLAGS=-pthread -lcheck_pic -lrt -lsubunit -lgtest -lstdc++
	OPEN_CMD=xdg-open
endif

all: install
	./app/Snake.app

snake: install
	./app/Snake.app

tetris: install_tetris
	./app/Tetris.app

$(SNAKE_LIB):
	$(CC) $(FLAGS) -DSNAKE -c brick_game/snake/*.cpp
	ar rcs $(SNAKE_LIB) *.o
	ranlib $(SNAKE_LIB)

$(TETRIS_LIB):
	$(CC) $(FLAGS) -DTETRIS -c brick_game/tetris/*.c
	ar rcs $(TETRIS_LIB) *.o
	ranlib $(TETRIS_LIB)

snake_cli: $(SNAKE_LIB)
	$(CC) $(FLAGS) -DSNAKE $(SNAKE_FILES) $(FLAGCUR)
	./a.out

tetris_cli: $(TETRIS_LIB)
	$(CC) $(FLAGS) -DTETRIS $(TETRIS_FILES) $(FLAGCUR)
	./a.out

asan_s: $(SNAKE_LIB)
	$(CC) $(FLAGS) -DSNAKE $(ASAN) $(SNAKE_FILES) $(FLAGCUR)

asan_t: $(TETRIS_LIB)
	$(CC) $(FLAGS) -DTETRIS $(ASAN) $(TETRIS_FILES) $(FLAGCUR)

install:
	mkdir ./build
	mkdir ./app
	cp gui/desktop/helper/desktop_snake.pro gui/desktop/desktop.pro
	cd ./build && qmake ../gui/desktop/ && make && cp desktop ../app/Snake.app

install_tetris:
	mkdir ./build
	mkdir ./app
	cp gui/desktop/helper/desktop_tetris.pro gui/desktop/desktop.pro
	cd ./build && qmake ../gui/desktop/ && make && cp desktop ../app/Tetris.app

uninstall:
	rm -rf ./app ./build *.txt

dist:
	mkdir dist
	mkdir dist/src
	cp -r brick_game/ tests/ controller/ gui/ Makefile *.h dist/src/
	cd ./dist && tar cvzf brick_game.tgz *
	rm -rf dist/src/

dvi:
	$(OPEN_CMD) Snake.html

test: $(SNAKE_LIB)
	$(CC) $(FLAGS) $(FILETEST) $(SNAKE_LIB) -DSNAKE -o $(FILETESTO) $(TESTFLAGS)
	rm -rf *.o *.a
	./$(FILETESTO)

test_val: $(SNAKE_LIB)
	$(CC) $(FLAGS) $(FILETEST) $(SNAKE_LIB) -DSNAKE -o $(FILETESTO) $(TESTFLAGS)
	valgrind --tool=memcheck --leak-check=yes \
	--track-origins=yes -s ./$(FILETESTO)

gcov_report:
	$(CC) $(FLAGS) $(COVFLAG) $(FILETEST) -DSNAKE -o $(FILEREPORT) $(TESTFLAGS)
	./$(FILEREPORT)
	lcov -t "snake" -o test.info -c -d .  
	genhtml -o $(DIRREPORT) test.info
	rm -rf *.o *.a *.gcno *.gcda *.info $(FILEREPORT)
	$(OPEN_CMD) $(DIRREPORT)/index.html

rep: gcov_report

clean:
	rm -rf *.o *.a *.gcno *.gcda *.log *.info *.txt *.out build/ app/ \
	$(DIRREPORT) $(FILETESTO) $(FILEREPORT) gui/build-* dist/

#style
cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem brick_game/tetris/*.c \
	brick_game/snake/*.cpp controller/*.cpp gui/cli/*.c gui/desktop/*.cpp tests/*.cpp
