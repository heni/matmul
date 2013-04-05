all: matrix-mul

matrix-mul: matrix-mul.cpp transposition.o multiplication.o
	g++ -O3 -std=c++0x $^ -o $@ -lboost_system -lboost_thread

transposition.o: transposition.cpp transposition.h common.h
	g++ -c -std=c++0x -O3 $<

multiplication.o: multiplication.cpp multiplication.h common.h
	g++ -c -std=c++0x -O3 $<

clean:
	rm matrix-mul transposition.o multiplication.o
