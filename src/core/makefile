VERSION = -std=c++11
CFLAGS = -Wall


cli: common
	echo "Making command-line app"
	$(MAKE) -C application cli

api:
	echo "TODO"

common:
	mkdir -p build
	$(MAKE) -C data
	$(MAKE) -C services
	echo "TODO"

clean:
	echo "Cleaning build files"
	rm -r -f build