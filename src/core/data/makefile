VERSION = -std=c++11
CFLAGS = -Wall

BUILD_DIR = ../build
LIB_DIR = ../lib
INCLUDE_DIR = ../include

CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CPP_FILES))


all: $(OBJ_FILES)
	echo "Compiled data layer"

$(BUILD_DIR)/%.o: %.cpp
	g++ $(VERSION) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@