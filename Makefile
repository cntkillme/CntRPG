.PHONY: all clean

# to-do: extension based on build type (windows: .exe, rest: .out)
TARGET := CntRPG.out
CXX := g++ --std=c++17 -Iinclude/ -lsqlite3$(CXX_FLAGS)
CXX_FLAGS := -Wall -pedantic -g

PRECOMPILED_HEADER = include/pch.hpp
PRECOMPILED_HEADERS :=

all: $(PRECOMPILED_HEADER).gch
	# to-do: proper handling of source files
	$(CXX) -o "$(TARGET)" src/SQL/*.cpp tests/SQL.cpp

clean:
	rm -f $(TARGET)
	rm -f $(PRECOMPILED_HEADER).gch

$(PRECOMPILED_HEADER).gch: $(PRECOMPILED_HEADER) $(PRECOMPILED_HEADERS)
	$(CXX) -c -fexceptions $< -o $@
