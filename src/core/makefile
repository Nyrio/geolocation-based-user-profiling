export VERSION = -std=c++11
export CFLAGS = -Wall
export EDLFLAGS = -pthread


cli: common
	echo "Making command-line app"
	$(MAKE) -C application cli

api:
	echo "TODO"

common:
	echo "Compiling libs, data and services layers"
	mkdir -p build
	$(MAKE) -C lib
	$(MAKE) -C data
	$(MAKE) -C services

clean:
	echo "Cleaning build files"
	- rm -rf build