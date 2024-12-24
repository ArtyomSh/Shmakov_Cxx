#include "sparseVector.cpp"
#include <cstddef>
#include <iostream>
#include <unordered_map>

using namespace std;

struct Index {
  size_t row;
  size_t col;

  Index(size_t r, size_t c) : row(r), col(c) {}

  bool operator==(const Index &other) const {
    return (row == other.row && col == other.col);
  }
};

struct Hash {
  size_t operator()(const Index &idx) const {
    return hash<size_t>()(idx.row) ^ (hash<size_t>()(idx.col) << 1);
  }
};

template <typename T> class SparseMatrix {
public:
  SparseMatrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {}

  T get(size_t row, size_t col) const {
    if (row >= rows_ || col >= cols_) {
      return T();
    }

    auto it = data_.find(Index(row, col));

    if (it == data_.end()) {
      return T();
    } else {
      return it->second;
    }
  }

  void set(size_t row, size_t col, const T &value) {
    if (row >= rows_ || col >= cols_) {
      return;
    }

    if (value == T()) {
      data_.erase(Index(row, col));
    } else {
      data_[Index(row, col)] = value;
    }
  }

  size_t rows() const { return rows_; }

  size_t cols() const { return cols_; }

  const unordered_map<Index, T, Hash> &getData() const { return data_; }

  SparseMatrix<T> transpose() {
    SparseMatrix<T> result(cols_, rows_);
    for (auto &pair : data_) {
      Index idx = pair.first;

      result.set(idx.col, idx.row, pair.second);
    }

    return result;
  }

  static SparseMatrix<T> pow(const SparseMatrix<T> &mat, int n) {
    if (mat.rows_ != mat.cols_) {
      throw runtime_error("error: rows should be equal columns");
    }

    if (n == 0) {
      return identity(mat.rows_);
    }

    SparseMatrix<T> result = mat;
    for (size_t i = 1; i <= n; ++i) {
      result = result * mat;
    }

    return result;
  }

  SparseMatrix<T> operator+(const SparseMatrix<T> &other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
      throw runtime_error("error: wrong input object sizes");
    }

    SparseMatrix<T> result(rows_, cols_);

    for (auto &pair : data_) {
      result.set(pair.first.row, pair.first.col, pair.second);
    }

    for (auto &pair : other.data_) {
      result.set(pair.first.row, pair.first.col,
                 result.get(pair.first.row, pair.first.col) + pair.second);
    }

    return result;
  }

  SparseMatrix<T> operator*(const SparseMatrix<T> &other) {
    if (cols_ != other.rows_) {
      throw runtime_error("error: wrong input object sizes");
    }

    SparseMatrix<T> result(rows_, other.cols_);

    for (auto &leftPair : data_) {
      Index idxLeft = leftPair.first;
      T valLeft = leftPair.second;

      for (auto &rightPair : other.data_) {
        Index idxRight = rightPair.first;
        if (idxRight.row == idxLeft.col) {
          size_t i = idxLeft.row;
          size_t j = idxRight.col;

          result.set(i, j, result.get(i, j) + valLeft * rightPair.second);
        }
      }
    }

    return result;
  }

  SparseMatrix<T> operator*(const T &scalar) {
    SparseMatrix<T> result(rows_, cols_);
    for (auto &pair : data_) {
      result.set(pair.first.row, pair.first.col, pair.second * scalar);
    }

    return result;
  }

  SparseVector<T> operator*(const SparseVector<T> &vec) {
    if (cols_ != vec.size()) {
      throw runtime_error("error: wrong input object sizes");
    }

    SparseVector<T> result(rows_);

    for (auto &mPair : data_) {
      Index idx = mPair.first;

      T mVal = mPair.second;
      T vVal = vec[idx.col];

      if (vVal != T()) {
        result.set(idx.row, result[idx.row] + mVal * vVal);
      }
    }
    return result;
  }

  SparseMatrix<T> operator+(const T &scalar) const {
    SparseMatrix<T> result(rows_, cols_);
    for (auto &pair : data_) {
      T newVal = pair.second + scalar;
      if (newVal != T()) {
        result.data_[pair.first] = newVal;
      }
    }

    if (scalar != T()) {
      for (size_t r = 0; r < rows_; ++r) {
        for (size_t c = 0; c < cols_; ++c) {
          Index idx(r, c);
          if (result.data_.find(idx) == result.data_.end()) {
            result.data_[idx] = scalar;
          }
        }
      }
    }

    return result;
  }

  SparseMatrix<T> operator-(const T &scalar) const {
    return (*this) + (T(-scalar));
  }

  SparseMatrix<T> operator/(const T &scalar) const {
    if (scalar == T()) {
      throw runtime_error("error: division by zero");
    }

    SparseMatrix<T> result(rows_, cols_);

    for (auto &pair : data_) {
      T newVal = pair.second / scalar;
      if (newVal != T()) {
        result.data_[pair.first] = newVal;
      }
    }

    return result;
  }

  SparseMatrix<T> elementWisePow(const T &exponent) const {
    SparseMatrix<T> result(rows_, cols_);

    for (auto &pair : data_) {
      T newVal = static_cast<T>(std::pow(static_cast<double>(pair.second),
                                         static_cast<double>(exponent)));

      if (newVal != T()) {
        result.data_[pair.first] = newVal;
      }
    }

    return result;
  }

  void print() const {
    for (size_t r = 0; r < rows_; ++r) {
      for (size_t c = 0; c < cols_; ++c) {
        cout << get(r, c) << " ";
      }
      cout << endl;
    }
    cout << endl;
  }

private:
  unordered_map<Index, T, Hash> data_;
  size_t rows_;
  size_t cols_;

  static SparseMatrix<T> identity(size_t n) {
    SparseMatrix<T> eye(n, n);

    for (size_t i = 0; i < n; ++i) {
      eye.set(i, i, T(1));
    }

    return eye;
  }
};
