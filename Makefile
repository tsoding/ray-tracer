ray-tracer: main.cpp Makefile
	g++ -ggdb -Wall -Werror -Wconversion -std=c++14 main.cpp -o ray-tracer

.PHONY: clean

clean:
	rm -rfv ray-tracer
