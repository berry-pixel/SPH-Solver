CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = app

SRC = main.cpp SPH.cpp utilities.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)