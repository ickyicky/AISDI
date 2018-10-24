CPPFLAGS = --std=c++11 -DBOOST_TEST_DYN_LINK

all: tests

test.o: test.cpp
	g++ -c $(CPPFLAGS) test.cpp

test_main.o: test_main.cpp
	g++ -c $(CPPFLAGS) test_main.cpp

test_bin: test.o test_main.o
	g++ -o test_bin test.o test_main.o -lboost_unit_test_framework

tests: test_bin
	./test_bin

clean:
	rm *.o
	rm test_bin
