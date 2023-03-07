CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

SRC     := src
INCLUDE := include

EXECUTABLE  := a.exe

all: $(EXECUTABLE)

run: clean all
	clear
	./$(EXECUTABLE)

$(EXECUTABLE): $(SRC)/*.cpp
	@$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@

clean:
	@-rm *.exe