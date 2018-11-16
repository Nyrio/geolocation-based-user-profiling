BUILD_DIR = ../build
LIB_DIR = ../lib
INCLUDE_DIR = ../include

CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CPP_FILES))


all: $(OBJ_FILES)
	echo "Compiled libs"

$(BUILD_DIR)/%.o: %.cpp
	g++ $(VERSION) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@