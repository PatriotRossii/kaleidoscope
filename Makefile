all:
	clang++-13 -g -O3 main.cpp `llvm-config-13 --cxxflags --ldflags --system-libs --libs core orcjit native` -I/usr/lib/llvm-13/include -I/usr/lib/llvm-c-13/include -o main
