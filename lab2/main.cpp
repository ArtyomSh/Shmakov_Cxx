// main.cpp

#include "Container.hpp"
#include <iostream>
#include <list>
#include <vector>

void processBoxArray(Box *boxes, int size) {
  std::cout << "processBoxArray call" << std::endl;
  for (int i = 0; i < size; ++i) {
    std::cout << "Box_ processing" << boxes[i].getIdx() << ": {"
              << boxes[i].getNumber() << ": " << boxes[i].getInterpretation()
              << "}" << std::endl;
  }
}

Box functionByValue(Box b) {
  std::cout << "functionByValue call" << std::endl;

  return b;
}

Box &functionByReference(Box &b) {
  std::cout << "functionByReference call" << std::endl;
  return b;
}

int main() {
  Box box1(1);

  Box *box2 = new Box(2);

  Box boxArray[3] = {Box(3), Box(4), Box(5)};

  processBoxArray(boxArray, 3);

  Box box3 = functionByValue(box1);

  Box box30 = functionByValue(std::move(box1));

  Box &boxRef = functionByReference(*box2);

  Box box4(4);
  box4 = box1;

  Box box5(5);
  box5 = std::move(box3);

  std::vector<Box> boxVector;
  for (int i = 6; i <= 10; ++i) {
    boxVector.emplace_back(i);
  }

  std::list<Box> boxList;
  for (int i = 11; i <= 15; ++i) {
    boxList.__emplace_back(i);
  }

  delete box2;

  return 0;
}
