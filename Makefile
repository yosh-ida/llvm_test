build:
	clang++ main.cpp `llvm-config --cxxflags --ldflags --libs --system-libs`
llvmir:
	clang++ -S -emit-llvm sample.cpp