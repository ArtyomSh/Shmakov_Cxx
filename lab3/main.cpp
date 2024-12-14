#include "../lab2/Container.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <vector>

using namespace std;

int main() {
  // Пункт 1: создать вектор из (500 - 1000) объектов.

  random_device rd;
  mt19937 gen(rd());

  uniform_int_distribution<> size_dist(500, 1000);
  int size = size_dist(gen);

  vector<Box> v1;
  v1.reserve(size);

  uniform_int_distribution<> interval(1, 1000);

  for (int i = 0; i < size; ++i) {
    v1.emplace_back(interval(gen));
  }

  cout << " v1:  ";
  for (const Box &box : v1) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  // Пункт 2: Создание вектора v2 (200 последних элементов v1)

  int b = v1.size() - 200;
  int e = v1.size();

  if (b < 0)
    b = 0;
  if (e > static_cast<int>(v1.size()))
    e = v1.size();

  vector<Box> v2(v1.begin() + b, v1.begin() + e);

  cout << " v2:  ";
  for (const Box &box : v2) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  // Пункт 3: создание list1 (первые n максимальных элементов v1, n -
  // случайное от 20 до 50)

  uniform_int_distribution<> intervalForN(20, 50);
  int n = intervalForN(gen);

  vector<Box> largest;

  largest.resize(n);
  partial_sort_copy(v1.begin(), v1.end(), largest.begin(), largest.end(),
                    greater<Box>());

  list<Box> list1(largest.begin(), largest.end());

  cout << n << " largest values (list1): ";
  for (const Box &box : list1) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  // Пункт 4: создание list2 (первые n минимальных элементов v2, n -
  // случайное от 20 до 50)

  vector<Box> smallest;

  smallest.resize(n);
  partial_sort_copy(v2.begin(), v2.end(), smallest.begin(), smallest.end());

  sort(smallest.begin(), smallest.end());

  list<Box> list2(smallest.begin(), smallest.end());

  cout << n << " smallest values (list2): ";
  for (const Box &box : list2) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  // Пункт 5: удалить из векторов v1 & v2 элементы, помещенные в list1 & list2
  map<int, Box> forRemoveV1;
  map<int, Box> forRemoveV2;

  for (const auto &box : list1) {
    forRemoveV1[box.getNumber()] = box;
  }

  for (const auto &box : list2) {
    forRemoveV2[box.getNumber()] = box;
  }

  v1.erase(remove_if(v1.begin(), v1.end(),
                     [&forRemoveV1](const Box &box) {
                       return forRemoveV1.find(box.getNumber()) !=
                              forRemoveV1.end();
                     }),
           v1.end());

  v2.erase(remove_if(v2.begin(), v2.end(),
                     [&forRemoveV2](const Box &box) {
                       return forRemoveV2.find(box.getNumber()) !=
                              forRemoveV2.end();
                     }),
           v2.end());
  cout << " v1:  ";
  for (const Box &box : v1) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  cout << " v2:  ";
  for (const Box &box : v2) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  // Пункт 6: Найти среднее значение list1 & перегрупировать.

  cout << "list1: ";
  for (const Box &box : list1) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  double sum = accumulate(
      list1.begin(), list1.end(), 0.0,
      [](double acc, const Box &box) { return acc + box.getNumber(); });
  double mean = sum / static_cast<double>(list1.size());

  cout << "list1 mean value: " << mean << endl;

  partition(list1.begin(), list1.end(),
            [mean](const Box &box) { return box.getNumber() > mean; });

  cout << "list1 reshufled: ";
  for (const Box &box : list1) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  // Пункт 7: удалить из list2 все нечетные элементы.

  cout << "list2: ";
  for (const Box &box : list2) {
    cout << box.getNumber() << " ";
  }

  cout << endl;
  list2.remove_if([](const Box &box) { return box.getNumber() % 2 != 0; });

  cout << "list2 remove odd: ";
  for (const Box &box : list2) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  // Пункт 8: создать v3, в котором будут элементы из v1 & v2 (пересечение).

  vector<Box> v3;
  v3.reserve(v1.size() + v2.size());

  copy_if(v1.begin(), v1.end(), back_inserter(v3), [&](const Box &box) {
    return find(v2.begin(), v2.end(), box) != v2.end();
  });

  cout << "v3: ";
  for (const Box &box : v3) {
    cout << box.getNumber() << " ";
  }
  cout << endl;

  // Пункт 9: создать list3, в котором будут пары эл-ов из list1 & list2.

  size_t size1 = list1.size();
  size_t size2 = list2.size();

  if (size1 > size2) {
    size_t diff = size1 - size2;

    auto it = list1.begin();
    advance(it, diff);
    list1.erase(list1.begin(), it);
  } else {
    size_t diff = size2 - size1;

    auto it = list2.begin();
    advance(it, diff);
    list2.erase(list2.begin(), it);
  }

  typedef pair<Box, Box> BoxPair;
  list<BoxPair> list3;

  transform(
      list1.begin(), list1.end(), list2.begin(), back_inserter(list3),
      [](const Box &a, const Box &b) -> BoxPair { return make_pair(a, b); });

  cout << "list3:" << endl;
  for (const auto &pair : list3) {
    cout << "(" << pair.first.getNumber() << ", " << pair.second.getNumber()
         << ")" << endl;
  }

  // 10 пункт: то же самое (с нюансами), только для векторов v1 & v2

  typedef pair<Box, Box> BoxPair;
  vector<BoxPair> result;

  size_t min_size = min(v1.size(), v2.size());

  result.reserve(min_size);

  transform(
      v1.begin(), v1.begin() + min_size, v2.begin(), back_inserter(result),
      [](const Box &a, const Box &b) -> BoxPair { return make_pair(a, b); });

  cout << "result:" << endl;
  for (const auto &pair : result) {
    cout << "(" << pair.first.getNumber() << ", " << pair.second.getNumber()
         << ")" << endl;
  }
}
