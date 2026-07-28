CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra \
           -Iinclude \
           -Iexternal/imgui \
           -Iexternal/imgui-sfml

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lGL

TARGET = app

SRC = \
    src/main.cpp \
    src/SPH.cpp \
    src/utilities.cpp \
    external/imgui/imgui.cpp \
    external/imgui/imgui_draw.cpp \
    external/imgui/imgui_widgets.cpp \
    external/imgui/imgui_tables.cpp \
    external/imgui-sfml/imgui-SFML.cpp


OBJ = $(patsubst %.cpp,build/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)


build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)


