CXX=g++
CXXFLAGS=-std=c++14 -g

all: main

main: solver.o main.cpp
	$(CXX) $(CXXFLAGS) solver.o main.cpp -o main

clean:
	rm -f *.o main

run:
	./main

db:
	gdb main
