CC = g++
DEBUG ?= 0
CFLAGS = -std=c++11 -Wall

.PHONY: tests, run_tests

ifeq ($(DEBUG), 1)
  	CFLAGS += -DDEBUG
  	TARGET = bin/test_DEBUG
else
    CFLAGS += -DNDEBUG
    TARGET = bin/test
endif


tests: make_tests run_tests

make_tests:
	mkdir -p bin/
	$(CC) $(CFLAGS) tests/test.cpp -o $(TARGET)

run_tests:
	./$(TARGET)

clean:
	rm -rf *o *.so *.dll *.exe bin/* bin/ obj/* obj/
