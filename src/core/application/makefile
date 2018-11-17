BUILD_DIR = ../build
LIB_DIR = ../lib
INCLUDE_DIR = ../include

OBJ_FILES = $(wildcard $(BUILD_DIR)/*.o)
MAIN_OBJ = cli.o api.o

OSFLAG :=
ifeq ($(OS),Windows_NT)
	OSFLAG += -lws2_32
endif

cli: clear-main
	echo "Compiling CLI"
	g++ $(VERSION) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $(BUILD_DIR)/cli.o cli.cpp
	g++ $(VERSION) -pthread -o ../cli $(BUILD_DIR)/cli.o $(OBJ_FILES) $(OSFLAG)

api: clear-main
	echo "Compiling API"

clear-main:
	rm -f $(patsubst %, $(BUILD_DIR)/%, $(MAIN_OBJ))