#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <string>

class Box {
private:
  int idx;
  inline static int counter = 0;
  int number;
  char *interpretation;

  char *numberToString(int num) const;

public:
  Box();
  Box(int input);
  Box(const Box &other);
  Box(Box &&other);

  Box &operator=(const Box &other);
  Box &operator=(Box &&other);
  bool operator<(const Box &other) const;
  bool operator>(const Box &other) const;
  bool operator==(const Box &other) const;

  ~Box();

  int getIdx() const;
  int getNumber() const;
  const char *getInterpretation() const;
  std::string getBoxInfo() const;
};

#endif
