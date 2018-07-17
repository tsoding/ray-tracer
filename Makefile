all: output.png

output.png: output.ppm
	convert output.ppm output.png

output.ppm: ray-tracer
	./ray-tracer

ray-tracer: main.cpp vec.hpp Makefile
	g++ -ggdb -Wall -Werror -Wconversion -std=c++14 main.cpp -o ray-tracer

.PHONY: clean

clean:
	rm -rfv ray-tracer
