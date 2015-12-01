CXX = g++

PROG = play_robots
TEST = test_robots
OBJS = main.o robots.o auxiliary.o test.o
CPPFLAGS = -std=c++11 -Wall -g

$(PROG) : main.o robots.o auxiliary.o
	$(CXX) -o $(PROG) main.o robots.o auxiliary.o $(CPPFLAGS)

test : test.o robots.o auxiliary.o
	$(CXX) -o $(TEST) test.o robots.o auxiliary.o $(CPPFLAGS)

clean :
	rm -f $(PROG) $(TEST) $(OBJS)

robots.o : robots.cpp robots.h auxiliary.h
	$(CXX) -c -o robots.o robots.cpp $(CPPFLAGS)

auxiliary.o : auxiliary.cpp auxiliary.h robots.h
	$(CXX) -c -o auxiliary.o auxiliary.cpp $(CPPFLAGS)

main.o : main.cpp robots.h
	$(CXX) -c -o main.o main.cpp $(CPPFLAGS)

test.o : test.cpp robots.h
	$(CXX) -c -o test.o test.cpp $(CPPFLAGS)
