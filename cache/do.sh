#!/bin/bash
make
time ./matrix_order
# valgrind --tool=cachegrind ./matrix_order
