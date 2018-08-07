.PHONY: all
all: ray-tracer

.PHONY: trace
trace: output.png

output.png: output.ppm
	convert output.ppm output.png

output.ppm: ray-tracer
	./ray-tracer scene.txt output.ppm

ray-tracer: src/main.cpp src/vec.hpp src/mat4x4.hpp lint
	$(CXX) -Wall -Werror -Wconversion -Wno-missing-braces -Wunused-variable -std=c++17 src/main.cpp -o ray-tracer

.PHONY: lint
lint: cpplint.py
	python2 cpplint.py --headers=hpp src/*.cpp src/*.hpp

cpplint.py:
	wget https://raw.githubusercontent.com/google/styleguide/ab48617e00be9d111804bd3715dd7b5f5732c9a3/cpplint/cpplint.py
