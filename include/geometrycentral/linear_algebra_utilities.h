#pragma once

#include "geometrycentral/utilities.h"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <iostream>

// === Various helper functions and sanity checks which are useful for linear algebra code

namespace geometrycentral {

template <typename T> Eigen::SparseMatrix<T> identityMatrix(size_t N);

// Shift the the diagonal of a matrix by a constant offset
template <typename T> void shiftDiagonal(Eigen::SparseMatrix<T>& m, T shiftAmount=1e-4);


// Verify that a matrix has finite entries, error out if not. Does nothing if NDEBUG is defined.
template <typename T> void checkFinite(const Eigen::SparseMatrix<T>& m);

template <typename T, int R, int C> void checkFinite(const Eigen::Matrix<T, R, C>& m);


// Verify that a sparse matrix is symmetric (hermitian), error out if not. Does nothing if NDEBUG is defined.
template <typename T> void checkHermitian(const Eigen::SparseMatrix<T>& m);

#include "geometrycentral/linear_algebra_utilities.ipp"

} // namespace geometrycentral
