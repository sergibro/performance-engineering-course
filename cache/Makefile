all: matrix_order matrix_fill object_cache test

matrix_order: matrix_order.c
	g++ -O3 -g3 -fno-omit-frame-pointer -lstdc++ -o matrix_order ./matrix_order.c

matrix_fill: matrix_fill.c
	g++ -O3 -g3 -fno-omit-frame-pointer -lstdc++ -o matrix_fill ./matrix_fill.c

object_cache: object_cache.cpp
	g++ -O3 -g3 -fno-omit-frame-pointer -lstdc++ -o object_cache ./object_cache.cpp

test: test.c
	gcc -O1 -g3 -march=skylake -ftree-vectorize -fopt-info-vec -fno-omit-frame-pointer -lstdc++ -o test ./test.c

clean: 
	rm matrix_order matrix_fill object_cache test
