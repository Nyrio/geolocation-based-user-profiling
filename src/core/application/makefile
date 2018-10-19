VERSION = -std=c++11
CFLAGS = -Wall

BUILD_DIR = ../build
LIB_DIR = ../lib
INCLUDE_DIR = ../include


cli:
	echo "CLI"
	g++ $(VERSION) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $(BUILD_DIR)/Terminal.o Terminal.cpp
	g++ $(VERSION) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $(BUILD_DIR)/ConsoleApplication1.o ConsoleApplication1.cpp
	g++ $(VERSION) -o cli $(BUILD_DIR)/Terminal.o $(BUILD_DIR)/ConsoleApplication1.o $(BUILD_DIR)/Core.o

api:
	echo "API"