CFLAGS= -std=c++17 -O2

all: compile link run clean
compile:
	g++ $(CFLAGS) -I src/include -c main.cpp
link:
	g++ main.o -o main -L src/Lib -l sfml-graphics -l sfml-window -l sfml-system
run:
	./main.exe
clean:
	del *.o
	del *.exe