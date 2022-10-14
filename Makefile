CXX = g++

CFLAGS = -IDependencies/include -Wall
LDFLAGS = -LDependencies/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

SRC = game.cpp
OBJ = game.o

TARGET = game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) -o game.exe $(OBJ) $(LDFLAGS)
	.\game

game.o: game.cpp
	$(CXX) -c game.cpp $(CFLAGS)

clean:
	DEL $(OBJ)
	DEL $(TARGET).exe
