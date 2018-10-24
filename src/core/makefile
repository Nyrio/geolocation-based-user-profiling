VERSION = -std=c++11
CFLAGS = -Wall


cli: common
	echo "Making command-line app"
	$(MAKE) -C application cli

api:
	echo "TODO"

common:
	echo "Compiling data and services layers"
	mkdir -p build
	$(MAKE) -C data
	$(MAKE) -C services

clean:
	echo "Cleaning build files"
	- rm -rf build