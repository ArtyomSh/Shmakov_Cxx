#include "sparseMatrix.cpp"

#include <chrono>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

#include <algorithm>
#include <random>
#include <vector>

using namespace std;

vector<size_t> getIdxs(size_t size, size_t nonZeroElementsCount) {
  if (nonZeroElementsCount > size)
    nonZeroElementsCount = size;

  vector<size_t> indices(size);
  for (size_t i = 0; i < size; ++i) {
    indices[i] = i;
  }

  mt19937 gen(42);

  shuffle(indices.begin(), indices.end(), gen);

  indices.resize(nonZeroElementsCount);

  return indices;
}

vector<int> sumDenseVectors(const vector<int> &v1, const vector<int> &v2) {
  if (v1.size() != v2.size()) {
    throw runtime_error("error: vectors must be same length");
  }

  vector<int> result(v1.size());

  for (size_t i = 0; i < v1.size(); ++i) {
    result[i] = v1[i] + v2[i];
  }

  return result;
}

vector<int> mulDenseVectorOnScalar(const vector<int> &v, int scalar) {
  vector<int> result(v.size());
  for (size_t i = 0; i < v.size(); ++i) {
    result[i] = v[i] * scalar;
  }
  return result;
}

int dotDenseVectors(const vector<int> &v1, const vector<int> &v2) {
  if (v1.size() != v2.size()) {
    throw runtime_error("error: vectors must be same length");
  }

  int result = 0;

  for (size_t i = 0; i < v1.size(); ++i) {
    result += v1[i] * v2[i];
  }

  return result;
}

vector<int> powDenseVector(const vector<int> &v, int exponent) {
  vector<int> result(v.size());

  for (size_t i = 0; i < v.size(); ++i) {
    result[i] = static_cast<int>(
        pow(static_cast<double>(v[i]), static_cast<double>(exponent)));
  }

  return result;
}

vector<vector<int>> sumDenseMatrix(const vector<vector<int>> &m1,
                                   const vector<vector<int>> &m2) {
  if (m1.size() != m2.size() || m1.empty() || m1[0].size() != m2[0].size()) {
    throw runtime_error("error: wrong input format");
  }

  size_t rows = m1.size();
  size_t cols = m1[0].size();

  vector<vector<int>> result(rows, vector<int>(cols, 0));

  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      result[r][c] = m1[r][c] + m2[r][c];
    }
  }

  return result;
}

vector<vector<int>> mulDenseMatrix(const vector<vector<int>> &m1,
                                   const vector<vector<int>> &m2) {
  if (m1.empty() || m2.empty() || m1[0].size() != m2.size()) {
    throw runtime_error("error: wrong input format");
  }
  size_t rows1 = m1.size();
  size_t cols1 = m1[0].size();
  size_t cols2 = m2[0].size();

  vector<vector<int>> result(rows1, vector<int>(cols2, 0));

  for (size_t r = 0; r < rows1; ++r) {
    for (size_t c = 0; c < cols2; ++c) {
      for (size_t k = 0; k < cols1; ++k) {
        result[r][c] += m1[r][k] * m2[k][c];
      }
    }
  }

  return result;
}

vector<vector<int>> mulDenseMatrixOnScalar(const vector<vector<int>> &m,
                                           int scalar) {
  size_t rows = m.size();
  size_t cols = m[0].size();

  vector<vector<int>> result(rows, vector<int>(cols, 0));

  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      result[r][c] = m[r][c] * scalar;
    }
  }

  return result;
}

vector<vector<int>> powDenseMatrix(const vector<vector<int>> &m, int power) {
  size_t rows = m.size();
  size_t cols = m[0].size();

  vector<vector<int>> result(rows, vector<int>(cols, 0));

  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      result[r][c] = static_cast<int>(
          pow(static_cast<double>(m[r][c]), static_cast<double>(power)));
    }
  }

  return result;
}

vector<vector<int>> transposeDenseMatrix(const vector<vector<int>> &m) {
  size_t rows = m.size();
  size_t cols = m[0].size();

  vector<vector<int>> result(cols, vector<int>(rows, 0));

  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      result[c][r] = m[r][c];
    }
  }

  return result;
}

int main() {
  try {
    // VECTORS
    const size_t vectorSize = 1000000;
    vector<double> vNonZeroElementsProportion = {0.001, 0.01, 0.1, 0.5, 0.9};

    cout << "VECTOR COMPARISON" << endl;
    cout << "Proportion | Operation | Sparse (ms) | Dense (ms) | "
            "Diff (ms)"
         << endl;

    for (double proportion : vNonZeroElementsProportion) {
      size_t nonZeroElementsCount =
          static_cast<size_t>(vectorSize * proportion);

      SparseVector<int> sv1(vectorSize);
      SparseVector<int> sv2(vectorSize);
      auto indices = getIdxs(vectorSize, nonZeroElementsCount);
      mt19937 gen(1);
      uniform_int_distribution<int> dist_val(1, 100);

      for (size_t idx : indices) {
        sv1.set(idx, dist_val(gen));
        sv2.set(idx, dist_val(gen));
      }

      vector<int> dv1(vectorSize, 0);
      vector<int> dv2(vectorSize, 0);
      for (size_t idx : indices) {
        dv1[idx] = sv1[idx];
        dv2[idx] = sv2[idx];
      }

      // SUMM
      auto start = chrono::high_resolution_clock::now();
      SparseVector<int> svSum = sv1 + sv2;
      auto end = chrono::high_resolution_clock::now();
      auto sDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      start = chrono::high_resolution_clock::now();
      vector<int> dvSumm = sumDenseVectors(dv1, dv2);
      end = chrono::high_resolution_clock::now();
      auto dDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      cout << proportion << " | SUM | " << sDuration << " | " << dDuration
           << " | " << (dDuration - sDuration) << endl;

      // MUL ON SCALAR
      start = chrono::high_resolution_clock::now();
      SparseVector<int> svMul = sv1 * 5;
      end = chrono::high_resolution_clock::now();
      sDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      start = chrono::high_resolution_clock::now();
      vector<int> dvMul = mulDenseVectorOnScalar(dv1, 5);
      end = chrono::high_resolution_clock::now();
      dDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      cout << proportion << " | MUL_SCALAR | " << sDuration << " | "
           << dDuration << " | " << (dDuration - sDuration) << endl;

      // DOT
      start = chrono::high_resolution_clock::now();
      int svDot = sv1.dot(sv2);
      end = chrono::high_resolution_clock::now();
      sDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      start = chrono::high_resolution_clock::now();
      int dvDot = dotDenseVectors(dv1, dv2);
      end = chrono::high_resolution_clock::now();
      dDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      cout << proportion << " | DOT | " << sDuration << " | " << dDuration
           << " | " << (dDuration - sDuration) << endl;

      // POW
      start = chrono::high_resolution_clock::now();
      SparseVector<int> svPow = sv1.elementWisePow(2);
      end = chrono::high_resolution_clock::now();
      sDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      start = chrono::high_resolution_clock::now();
      vector<int> dvPow = powDenseVector(dv1, 2);
      end = chrono::high_resolution_clock::now();
      dDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      cout << proportion << " | POW | " << sDuration << " | " << dDuration
           << " | " << (dDuration - sDuration) << endl;

      cout << "---------------------------------------------" << endl;
    }

    // MATRICES
    cout << "\nMATRICES COMPARISON" << endl;
    cout << "Proportion | Operation | Sparse (ms) | Dense (ms) | "
            "Diff (ms)"
         << endl;

    const size_t mSize = 1000;
    vector<double> mNonZeroElementsProportion = {0.001, 0.005, 0.01, 0.05};

    for (double proportion : mNonZeroElementsProportion) {
      size_t totalElements = mSize * mSize;
      size_t nonZeroElementsCount1 =
          static_cast<size_t>(totalElements * proportion);

      SparseMatrix<int> sm1(mSize, mSize);
      SparseMatrix<int> sm2(mSize, mSize);
      auto matrix_indices = getIdxs(totalElements, nonZeroElementsCount1);

      mt19937 gen(1);
      uniform_int_distribution<int> dist_val(1, 100);

      for (size_t idx : matrix_indices) {
        size_t row = idx / mSize;
        size_t col = idx % mSize;
        sm1.set(row, col, dist_val(gen));
        sm2.set(row, col, dist_val(gen));
      }

      vector<vector<int>> dm1(mSize, vector<int>(mSize, 0));
      vector<vector<int>> dm2(mSize, vector<int>(mSize, 0));

      for (size_t r = 0; r < mSize; ++r) {
        for (size_t c = 0; c < mSize; ++c) {
          dm1[r][c] = sm1.get(r, c);
          dm2[r][c] = sm2.get(r, c);
        }
      }

      // SUMM
      auto start = chrono::high_resolution_clock::now();
      SparseMatrix<int> smSum = sm1 + sm2;
      auto end = chrono::high_resolution_clock::now();
      auto sDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      start = chrono::high_resolution_clock::now();
      vector<vector<int>> dmSum = sumDenseMatrix(dm1, dm2);
      end = chrono::high_resolution_clock::now();
      auto dDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      cout << proportion << " | SUM | " << sDuration << " | " << dDuration
           << " | " << (dDuration - sDuration) << endl;

      // MUL
      start = chrono::high_resolution_clock::now();
      SparseMatrix<int> smMul = sm1 * sm2;
      end = chrono::high_resolution_clock::now();
      sDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      start = chrono::high_resolution_clock::now();
      vector<vector<int>> dmMul = mulDenseMatrix(dm1, dm2);
      end = chrono::high_resolution_clock::now();
      dDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      cout << proportion << " | MUL | " << sDuration << " | " << dDuration
           << " | " << (dDuration - sDuration) << endl;

      // MUL ON SCALAR
      start = chrono::high_resolution_clock::now();
      SparseMatrix<int> smMulScalar = sm1 * 5;
      end = chrono::high_resolution_clock::now();
      sDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      start = chrono::high_resolution_clock::now();
      vector<vector<int>> dmScalar = mulDenseMatrixOnScalar(dm1, 5);
      end = chrono::high_resolution_clock::now();
      dDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      cout << proportion << " | MUL_SCALAR | " << sDuration << " | "
           << dDuration << " | " << (dDuration - sDuration) << endl;

      // POW BY ELEMS
      start = chrono::high_resolution_clock::now();
      SparseMatrix<int> smPow = sm1.elementWisePow(2);
      end = chrono::high_resolution_clock::now();
      sDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      start = chrono::high_resolution_clock::now();
      vector<vector<int>> dnPow = powDenseMatrix(dm1, 2);
      end = chrono::high_resolution_clock::now();
      dDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      cout << proportion << " | POW | " << sDuration << " | " << dDuration
           << " | " << (dDuration - sDuration) << endl;

      // TRANSPOSE
      start = chrono::high_resolution_clock::now();
      SparseMatrix<int> smTranspose = sm1.transpose();
      end = chrono::high_resolution_clock::now();
      sDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      start = chrono::high_resolution_clock::now();
      vector<vector<int>> dmTrasnpose = transposeDenseMatrix(dm1);
      end = chrono::high_resolution_clock::now();
      dDuration =
          chrono::duration_cast<chrono::milliseconds>(end - start).count();

      cout << proportion << " | TRANSPOSE | " << sDuration << " | " << dDuration
           << " | " << (dDuration - sDuration) << endl;

      cout << "---------------------------------------------" << endl;
    }

  } catch (const exception &e) {
    cerr << "error: " << e.what() << endl;
  }

  return 0;
}
