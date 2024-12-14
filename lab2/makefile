CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: main

main: main.o Container.o NumberToWords.o
	$(CXX) $(CXXFLAGS) -o main main.o Container.o NumberToWords.o

main.o: main.cpp Container.hpp NumberToWords.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Container.o: NumberToWords.cpp NumberToWords.hpp
	$(CXX) $(CXXFLAGS) -c NumberToWords.cpp

Container.o: Container.cpp Container.hpp NumberToWords.hpp
	$(CXX) $(CXXFLAGS) -c Container.cpp

clean:
	rm -f *.o main
