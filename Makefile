.PHONY: all clean

TARGET := CntRPG
CXX := g++ --std=c++17 -Iinclude/ -lsqlite3 $(CXX_FLAGS)
CXX_FLAGS := -Wall -pedantic -g -H

PRECOMPILED_HEADER = include/PrecompiledHeader.hpp
PRECOMPILED_HEADERS :=

all: $(PRECOMPILED_HEADER).gch
	# to-do: proper handling of source files
	$(CXX) -H -o "$(TARGET)" src/SQL/*.cpp

clean:
	rm -f program.out
	rm -f $(PRECOMPILED_HEADER).gch

$(PRECOMPILED_HEADER).gch: include/PrecompiledHeader.hpp $(PRECOMPILED_HEADERS)
	$(CXX) -c -fexceptions $< -o $@
