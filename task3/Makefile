all: threads_min threads_sum

threads_min: threads_min.cpp
	g++ -march=skylake -std=c++11 -pthread -o threads_min ./threads_min.cpp

threads_sum: threads_sum.cpp
	g++ -march=skylake -std=c++11 -pthread -o threads_sum ./threads_sum.cpp

clean: 
	rm threads_min
