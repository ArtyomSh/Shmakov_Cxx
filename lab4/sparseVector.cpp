#include <cstddef>
#include <iostream>
#include <unordered_map>

using namespace std;

template <typename T> class SparseVector {
public:
  explicit SparseVector(size_t size) : size_(size) {}

  T operator[](size_t index) const {
    if (index >= size_) {
      return T();
    }
    auto it = data_.find(index);
    return (it == data_.end()) ? T() : it->second;
  }

  void set(size_t index, const T &value) {
    if (index >= size_) {
      return;
    }
    if (value == T()) {
      data_.erase(index);
    } else {
      data_[index] = value;
    }
  }

  size_t size() const { return size_; }

  SparseVector<T> operator+(const SparseVector<T> &other) {
    if (size_ != other.size_) {
      throw runtime_error("error: vectors must be the same length");
    }

    SparseVector<T> result(size_);

    for (auto &pair : data_) {
      result.set(pair.first, pair.second);
    }

    for (auto &pair : other.data_) {
      T newVal = result[pair.first] + pair.second;
      result.set(pair.first, newVal);
    }

    return result;
  }

  SparseVector<T> operator-(const SparseVector<T> &other) {
    if (size_ != other.size_) {
      throw runtime_error("error: vectors must be the same length");
    }

    SparseVector<T> result(size_);

    for (auto &pair : data_) {
      result.set(pair.first, pair.second);
    }

    for (auto &pair : other.data_) {
      T newVal = result[pair.first] - pair.second;
      result.set(pair.first, newVal);
    }
    return result;
  }

  SparseVector<T> operator*(const T &scalar) {
    SparseVector<T> result(size_);

    for (auto &pair : data_) {
      result.set(pair.first, pair.second * scalar);
    }

    return result;
  }

  SparseVector<T> operator-() {
    SparseVector<T> result(size_);

    for (auto &pair : data_) {
      result.set(pair.first, -pair.second);
    }

    return result;
  }

  T dot(const SparseVector<T> &other) {
    if (size_ != other.size_) {
      throw runtime_error("error: vectors must be the same length");
    }

    T result = T(0);
    for (auto &pair : data_) {
      result += pair.second * other[pair.first];
    }

    return result;
  }

  SparseVector<T> operator+(const T &scalar) const {
    SparseVector<T> result(size_);

    for (auto &pair : data_) {
      T newVal = pair.second + scalar;
      if (newVal != T()) {
        result.data_[pair.first] = newVal;
      }
    }

    if (scalar != T()) {
      for (size_t i = 0; i < size_; ++i) {
        if (result.data_.find(i) == result.data_.end()) {
          result.data_[i] = scalar;
        }
      }
    }
    return result;
  }

  SparseVector<T> operator-(const T &scalar) const {
    return (*this) + (T(-scalar));
  }

  SparseVector<T> operator/(const T &scalar) const {
    SparseVector<T> result(size_);

    if (scalar == T()) {
      throw runtime_error("error: division by zero");
    }

    for (auto &pair : data_) {
      T newVal = pair.second / scalar;
      if (newVal != T()) {
        result.data_[pair.first] = newVal;
      }
    }
    return result;
  }

  SparseVector<T> elementWisePow(const T &exponent) const {
    SparseVector<T> result(size_);
    for (auto &pair : data_) {
      T newVal = static_cast<T>(
          pow(static_cast<double>(pair.second), static_cast<double>(exponent)));
      if (newVal != T()) {
        result.data_[pair.first] = newVal;
      }
    }
    return result;
  }

  void print() const {
    for (size_t i = 0; i < size_; ++i) {
      cout << (*this)[i] << " ";
    }
    cout << endl;
  }

  const unordered_map<size_t, T> &getData() const { return data_; }

private:
  unordered_map<size_t, T> data_;
  size_t size_;
};
