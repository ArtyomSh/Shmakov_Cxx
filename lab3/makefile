CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: main

main: main.o Container.o NumberToWords.o
	$(CXX) $(CXXFLAGS) -o main main.o Container.o NumberToWords.o

main.o: main.cpp ../lab2/Container.hpp ../lab2/NumberToWords.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

NumberToWords.o: ../lab2/NumberToWords.cpp ../lab2/NumberToWords.hpp
	$(CXX) $(CXXFLAGS) -c ../lab2/NumberToWords.cpp

Container.o: ../lab2/Container.cpp ../lab2/Container.hpp ../lab2/NumberToWords.hpp
	$(CXX) $(CXXFLAGS) -c ../lab2/Container.cpp

clean:
	rm -f *.o main
