.PHONY: all clean build build_distributed build_regular run run_distributed run_regular

ARGS ?= "10 0.5"

all: build

build:
	mkdir -p build
	cd build && cmake ..
	cd build && make

build_distributed:
	mkdir -p build
	cd build && cmake ..
	cd build && make distributed

build_regular:
	mkdir -p build
	cd build && cmake ..
	cd build && make regular

run: run_distributed

run_distributed: build_distributed
	./build/distributed/distributed $(ARGS)

run_regular: build_regular
	./build/regular/regular $(ARGS)

clean:
	rm -rf build
