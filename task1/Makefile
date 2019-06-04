all: vecs_op matrices_mult substr_search

vecs_op: vecs_op.c
	gcc -O0 -g3 -march=skylake -ftree-vectorize -fopt-info-vec -fopt-info-vec-missed -fno-omit-frame-pointer -lstdc++ -o vecs_op ./vecs_op.c

matrices_mult: matrices_mult.c
	gcc -O0 -g3 -march=skylake -ftree-vectorize -fopt-info-vec -fopt-info-vec-missed -fno-omit-frame-pointer -lstdc++ -o matrices_mult ./matrices_mult.c

substr_search: substr_search.c
	gcc -O0 -g3 -march=skylake -ftree-vectorize -fopt-info-vec -fopt-info-vec-missed -fno-omit-frame-pointer -lstdc++ -o substr_search ./substr_search.c

clean: 
	rm vecs_op matrices_mult substr_search
