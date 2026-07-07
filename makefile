CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra \
           -Iinclude

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = app

SRC = src/main.cpp src/SPH.cpp src/utilities.cpp

OBJ = build/main.o build/SPH.o build/utilities.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)


