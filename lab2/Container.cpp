#include "Container.hpp"
#include "NumberToWords.hpp"
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

char *Box::numberToString(int num) const {
  string str = convertToWords(num);

  char *result = new char[str.size() + 1];
  std::strcpy(result, str.c_str());

  return result;
}

Box::Box() : idx(++counter), number(-1), interpretation(nullptr) {
  interpretation = new char[13];
  std::strcpy(interpretation, "unspecified");

  cout << "Default constructor" << endl;
}

Box::Box(int input) : idx(++counter), number(input), interpretation(nullptr) {
  interpretation = numberToString(input);

  cout << "Parameterized constructor: Create Box_" << idx << ": {" << number
       << ": " << interpretation << "}" << endl;
}

Box::Box(const Box &other)
    : idx(++counter), number(other.number), interpretation(nullptr) {
  if (other.interpretation) {
    interpretation = new char[std::strlen(other.interpretation) + 1];
    std::strcpy(interpretation, other.interpretation);
  }

  cout << "Copy constructor: Create New Box_" << idx << " from Box_"
       << other.idx << " : {" << number << ": " << interpretation << "}"
       << endl;
}

Box::Box(Box &&other)
    : idx(++counter), number(other.number),
      interpretation(other.interpretation) {
  other.interpretation = nullptr;

  cout << "Move constructor" << endl;
}

Box &Box::operator=(const Box &other) {
  if (this != &other) {
    number = other.number;
    delete[] interpretation;

    if (other.interpretation) {
      interpretation = new char[std::strlen(other.interpretation) + 1];
      std::strcpy(interpretation, other.interpretation);
    } else {
      interpretation = nullptr;
    }
  }

  cout << "Copy assignment operator" << endl;

  return *this;
}

Box &Box::operator=(Box &&other) {
  cout << "Move assignment operator" << endl;

  if (this != &other) {
    number = other.number;
    delete[] interpretation;

    interpretation = other.interpretation;
    other.interpretation = nullptr;
  }
  return *this;
}

bool Box::operator<(const Box &other) const {
  cout << number << " < " << other.number << " compare" << endl;

  return number < other.number;
}

bool Box::operator>(const Box &other) const {
  cout << number << " > " << other.number << " compare" << endl;

  return number > other.number;
}

bool Box::operator==(const Box &other) const {
  cout << number << " == " << other.number << " compare" << endl;

  return number == other.number;
}

Box::~Box() {
  cout << "Remove Box_" << idx << ": {" << number << ": "
       << (interpretation ? interpretation : "null") << "}" << endl;
  delete[] interpretation;
}

int Box::getIdx() const { return idx; }

int Box::getNumber() const { return number; }

const char *Box::getInterpretation() const { return interpretation; }

std::string Box::getBoxInfo() const {
  stringstream ss;
  ss << "Box_" << idx << ": {" << number << ": "
     << (interpretation ? interpretation : "null") << "}";
  return ss.str();
}
