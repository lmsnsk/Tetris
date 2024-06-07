CC=gcc
FLAGS=-Wall -Wextra -std=c11
# FLAGS=-Wall -Werror -Wextra -std=c11
FLAGCUR=-lncurses
ASAN=-g -fsanitize=address
TETRIS_LIB=tetris.a
TETRIS_FILES=gui/cli/cli.c game.c tetris.a
FILETEST=tests/*.c  brick_game/tetris/*.c fms/tetris/*.c game.c
FILEREPORT=s21_test_report
FILETESTO=s21_test
DIRREPORT=report
COVFLAG=-fprofile-arcs -ftest-coverage

OS = $(shell uname)

ifeq ($(OS), Darwin)
	TESTFLAGS=-pthread -lcheck 
	OPEN_CMD=open
else
	TESTFLAGS=-pthread -lcheck_pic -lrt -lsubunit 
	OPEN_CMD=xdg-open
endif

all: tetris_cli

tetris: install_tetris
	./app/Tetris.app

tetris.a:
	$(CC) $(FLAGS) -c brick_game/tetris/*.c fms/tetris/*.c
	ar rcs tetris.a *.o
	ranlib tetris.a

tetris_cli: tetris.a
	$(CC) $(FLAGS) $(TETRIS_FILES) $(FLAGCUR)
	./a.out

asan_t: $(TETRIS_LIB)
	$(CC) $(FLAGS) $(ASAN) $(TETRIS_FILES) $(FLAGCUR)

install:
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
	$(CC) $(FLAGS) $(FILETEST) $(TETRIS_LIB) -o $(FILETESTO) $(TESTFLAGS)
	rm -rf *.o *.a
	./$(FILETESTO)

test_val: $(SNAKE_LIB)
	$(CC) $(FLAGS) $(FILETEST) $(TETRIS_LIB) -o $(FILETESTO) $(TESTFLAGS)
	valgrind --tool=memcheck --leak-check=yes \
	--track-origins=yes -s ./$(FILETESTO)

gcov_report:
	$(CC) $(FLAGS) $(COVFLAG) $(FILETEST) -o $(FILEREPORT) $(TESTFLAGS)
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
	gui/cli/*.c tests/*.c fms/tetris/*.c game.c
