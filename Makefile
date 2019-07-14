.PHONY: all clean

all:
	g++ -std=c++17 -Wall -pedantic -lsqlite3 -Iinclude/ -o program.out src/SQL/*.cpp

clean:
	rm program.out
