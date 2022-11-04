CC = g++ -std=c++17
CFLAGS = -Wall -Wextra -Werror
GTEST = -lgtest

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	LFLAGS = -lpthread
	OPEN = -xdg-open
else
	OPEN = -open
endif

APP = transactions

all : test build

build : $(APP)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(APP) : main.o interface.o
	$(CC) $^ -o $@ $(LFLAGS)

test : tests

tests : tests.o
	$(CC) $(CFLAGS) $^ $(GTEST) $(LFLAGS) -o $@.out
	./$@.out

style :
	@clang-format -n --Werror *.hpp --style=Google --ferror-limit=3
	@clang-format -n --Werror *.cpp --style=Google --ferror-limit=3

cppcheck :
	cppcheck --std=c++17 --language=c++ --enable=all --suppress=missingIncludeSystem --suppress=missingInclude --suppress=unmatchedSuppression --suppress=unreadVariable --suppress=knownConditionTrueFalse --suppress=unusedFunction *.cpp *.hpp *.h *.inl

mem : clean
	$(MAKE)  CC="$(CC) -DMEM_TEST" tests
ifeq ($(OS), Linux)
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./tests.out
else
	CK_FORK=no leaks --atExit -- ./tests.out
endif

gcov : gcov_report

gcov_report : clean
	$(MAKE) CFLAGS="$(CFLAGS) --coverage" LFLAGS="$(LFLAGS) --coverage" tests
	lcov -t tests.out -o test.info -c -d . --no-external
	genhtml -o report test.info
	$(OPEN) report/index.html

clean : clean_report
	$(RM) *.o *.out $(APP)

clean_report :
	$(RM) -rf *.gcda *.gcno *.info *.gch report

rebuild : clean all
