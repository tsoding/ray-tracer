.PHONY: all
all: ray-tracer

.PHONY: trace
trace: output.png

output.png: output.ppm
	convert output.ppm output.png

output.ppm: ray-tracer
	./ray-tracer 0 0 output.ppm

ray-tracer: src/main.cpp src/vec.h lint
	$(CXX) -Wall -Werror -Wconversion -Wno-missing-braces -std=c++17 src/main.cpp -o ray-tracer

.PHONY: lint
lint: cpplint.py
	python cpplint.py src/*.cpp src/*.h

cpplint.py:
	wget https://raw.githubusercontent.com/google/styleguide/ab48617e00be9d111804bd3715dd7b5f5732c9a3/cpplint/cpplint.py
