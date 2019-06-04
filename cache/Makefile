all: matrix_order matrix_fill object_cache

matrix_order: matrix_order.c
	g++ -O3 -g3 -fno-omit-frame-pointer -lstdc++ -o matrix_order ./matrix_order.c

matrix_fill: matrix_fill.c
	g++ -O3 -g3 -fno-omit-frame-pointer -lstdc++ -o matrix_fill ./matrix_fill.c

object_cache: object_cache.cpp
	g++ -O3 -g3 -fno-omit-frame-pointer -lstdc++ -o object_cache ./object_cache.cpp

clean: 
	rm matrix_order matrix_fill object_cache
