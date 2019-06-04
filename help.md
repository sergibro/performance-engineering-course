``kcachegrind
callgrind_annotate``

``sudo apt install linux-tools kcachegrind graphviz
cat /proc/cpuinfo
valgrind --tool=callgrind ./fibo --dump-instr=yes --collect-jumps=yes
sed 's#/home/andrii/UCU/2019/w2/TSP-master#path_to_your_source#g' callgrind.out.NUM > callgrind.out.replaced
sudo perf record -g ./fibo
sudo perf report
-fopt-info
sysctl -n machdep.cpu.brand_string``

`gcc -O0 -g3 -fno-omit-frame-pointer -lstdc++ -o test ./test.c`

``-ftree-vectorize (enable vectorization)
-fno-tree-vectorize (disable vectorization)
-march=‘you-rarchitecture’
-O3 (enable vectorization by default)
-fopt-info-vec (enable messages about vectorization)
-fopt-info-vec-missed (enable messages about missed
vectorization)``
