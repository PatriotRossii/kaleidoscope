all:
	clang++-13 -g -O3 main.cpp `llvm-config-13 --cxxflags --ldflags --system-libs --libs core` -I/usr/local/include/llvm-13 -I/usr/local/include/llvm-c-13 -o main
