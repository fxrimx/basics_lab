// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2020
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// Version: 4.0.2019.08.13

#pragma once

#include <Mathematics/GVector.h>
#include <Mathematics/GaussianElimination.h>
#include <algorithm>

namespace bcg {
template<typename Real>
class GMatrix {
public:
    // The table is length zero and mNumRows and mNumCols are set to zero.
    GMatrix()
            :
            mNumRows(0),
            mNumCols(0) {
    }

    // The table is length numRows*numCols and the elements are
    // initialized to zero.
    GMatrix(int numRows, int numCols) {
        SetSize(numRows, numCols);
        std::fill(mElements.begin(), mElements.end(), (Real) 0);
    }

    // For 0 <= r < numRows and 0 <= c < numCols, element (r,c) is 1 and
    // all others are 0.  If either of r or c is invalid, the zero matrix
    // is created.  This is a convenience for creating the standard
    // Euclidean basis matrices; see also MakeUnit(int,int) and
    // Unit(int,int).
    GMatrix(int numRows, int numCols, int r, int c) {
        SetSize(numRows, numCols);
        MakeUnit(r, c);
    }

    // The copy constructor, destructor, and assignment operator are
    // generated by the compiler.

    // Member access for which the storage representation is transparent.
    // The matrix entry in row r and column c is A(r,c).  The first
    // operator() returns a const reference rather than a Real value.
    // This supports writing via standard file operations that require a
    // const pointer to data.
    void SetSize(int numRows, int numCols) {
        if (numRows > 0 && numCols > 0) {
            mNumRows = numRows;
            mNumCols = numCols;
            mElements.resize(mNumRows * mNumCols);
        } else {
            mNumRows = 0;
            mNumCols = 0;
            mElements.clear();
        }
    }

    inline void GetSize(int &numRows, int &numCols) const {
        numRows = mNumRows;
        numCols = mNumCols;
    }

    inline int GetNumRows() const {
        return mNumRows;
    }

    inline int GetNumCols() const {
        return mNumCols;
    }

    inline int GetNumElements() const {
        return static_cast<int>(mElements.size());
    }

    inline Real const &operator()(int r, int c) const {
        if (0 <= r && r < GetNumRows() && 0 <= c && c < GetNumCols()) {
#if defined(GTE_USE_ROW_MAJOR)
            return mElements[c + mNumCols * r];
#else
            return mElements[r + mNumRows * c];
#endif
        }
        LogError("Invalid index.");
    }

    inline Real &operator()(int r, int c) {
        if (0 <= r && r < GetNumRows() && 0 <= c && c < GetNumCols()) {
#if defined(GTE_USE_ROW_MAJOR)
            return mElements[c + mNumCols * r];
#else
            return mElements[r + mNumRows * c];
#endif
        }
        LogError("Invalid index.");
    }

    // Member access by rows or by columns.  The input vectors must have
    // the correct number of elements for the matrix size.
    void SetRow(int r, GVector<Real> const &vec) {
        if (0 <= r && r < mNumRows) {
            if (vec.GetSize() == GetNumCols()) {
                for (int c = 0; c < mNumCols; ++c) {
                    operator()(r, c) = vec[c];
                }
            }
            LogError("Mismatched sizes.");
        }
        LogError("Invalid index.");
    }

    void SetCol(int c, GVector<Real> const &vec) {
        if (0 <= c && c < mNumCols) {
            if (vec.GetSize() == GetNumRows()) {
                for (int r = 0; r < mNumRows; ++r) {
                    operator()(r, c) = vec[r];
                }
                return;
            }
            LogError("Mismatched sizes.");
        }
        LogError("Invalid index.");
    }

    GVector<Real> GetRow(int r) const {
        if (0 <= r && r < mNumRows) {
            GVector<Real> vec(mNumCols);
            for (int c = 0; c < mNumCols; ++c) {
                vec[c] = operator()(r, c);
            }
            return vec;
        }
        LogError("Invalid index.");
    }

    GVector<Real> GetCol(int c) const {
        if (0 <= c && c < mNumCols) {
            GVector<Real> vec(mNumRows);
            for (int r = 0; r < mNumRows; ++r) {
                vec[r] = operator()(r, c);
            }
            return vec;
        }
        LogError("Invalid index.");
    }

    // Member access by 1-dimensional index.  NOTE: These accessors are
    // useful for the manipulation of matrix entries when it does not
    // matter whether storage is row-major or column-major.  Do not use
    // constructs such as M[c+NumCols*r] or M[r+NumRows*c] that expose the
    // storage convention.
    inline Real const &operator[](int i) const {
        return mElements[i];
    }

    inline Real &operator[](int i) {
        return mElements[i];
    }

    // Comparisons for sorted containers and geometric ordering.
    inline bool operator==(GMatrix const &mat) const {
        return mNumRows == mat.mNumRows && mNumCols == mat.mNumCols
               && mElements == mat.mElements;
    }

    inline bool operator!=(GMatrix const &mat) const {
        return mNumRows == mat.mNumRows && mNumCols == mat.mNumCols
               && mElements != mat.mElements;
    }

    inline bool operator<(GMatrix const &mat) const {
        return mNumRows == mat.mNumRows && mNumCols == mat.mNumCols
               && mElements < mat.mElements;
    }

    inline bool operator<=(GMatrix const &mat) const {
        return mNumRows == mat.mNumRows && mNumCols == mat.mNumCols
               && mElements <= mat.mElements;
    }

    inline bool operator>(GMatrix const &mat) const {
        return mNumRows == mat.mNumRows && mNumCols == mat.mNumCols
               && mElements > mat.mElements;
    }

    inline bool operator>=(GMatrix const &mat) const {
        return mNumRows == mat.mNumRows && mNumCols == mat.mNumCols
               && mElements >= mat.mElements;
    }

    // Special matrices.

    // All components are 0.
    void MakeZero() {
        std::fill(mElements.begin(), mElements.end(), (Real) 0);
    }

    // Component (r,c) is 1, all others zero.
    void MakeUnit(int r, int c) {
        if (0 <= r && r < mNumRows && 0 <= c && c < mNumCols) {
            MakeZero();
            operator()(r, c) = (Real) 1;
            return;
        }
        LogError("Invalid index.");
    }

    // Diagonal entries 1, others 0, even when nonsquare.
    void MakeIdentity() {
        MakeZero();
        int const numDiagonal = (mNumRows <= mNumCols ? mNumRows : mNumCols);
        for (int i = 0; i < numDiagonal; ++i) {
            operator()(i, i) = (Real) 1;
        }
    }

    static GMatrix Zero(int numRows, int numCols) {
        GMatrix<Real> M(numRows, numCols);
        M.MakeZero();
        return M;
    }

    static GMatrix Unit(int numRows, int numCols, int r, int c) {
        GMatrix<Real> M(numRows, numCols);
        M.MakeUnit(r, c);
        return M;
    }

    static GMatrix Identity(int numRows, int numCols) {
        GMatrix<Real> M(numRows, numCols);
        M.MakeIdentity();
        return M;
    }

protected:
    // The matrix is stored as a 1-dimensional array.  The convention of
    // row-major or column-major is your choice.
    int mNumRows, mNumCols;
    std::vector<Real> mElements;
};

// Unary operations.
template<typename Real>
GMatrix<Real> operator+(GMatrix<Real> const &M) {
    return M;
}

template<typename Real>
GMatrix<Real> operator-(GMatrix<Real> const &M) {
    GMatrix<Real> result(M.GetNumRows(), M.GetNumCols());
    for (int i = 0; i < M.GetNumElements(); ++i) {
        result[i] = -M[i];
    }
    return result;
}

// Linear-algebraic operations.
template<typename Real>
GMatrix<Real> operator+(GMatrix<Real> const &M0, GMatrix<Real> const &M1) {
    GMatrix<Real> result = M0;
    return result += M1;
}

template<typename Real>
GMatrix<Real> operator-(GMatrix<Real> const &M0, GMatrix<Real> const &M1) {
    GMatrix<Real> result = M0;
    return result -= M1;
}

template<typename Real>
GMatrix<Real> operator*(GMatrix<Real> const &M, Real scalar) {
    GMatrix<Real> result = M;
    return result *= scalar;
}

template<typename Real>
GMatrix<Real> operator*(Real scalar, GMatrix<Real> const &M) {
    GMatrix<Real> result = M;
    return result *= scalar;
}

template<typename Real>
GMatrix<Real> operator/(GMatrix<Real> const &M, Real scalar) {
    GMatrix<Real> result = M;
    return result /= scalar;
}

template<typename Real>
GMatrix<Real> &operator+=(GMatrix<Real> &M0, GMatrix<Real> const &M1) {
    if (M0.GetNumRows() == M1.GetNumRows() && M0.GetNumCols() == M1.GetNumCols()) {
        for (int i = 0; i < M0.GetNumElements(); ++i) {
            M0[i] += M1[i];
        }
        return M0;
    }
    LogError("Mismatched sizes");
}

template<typename Real>
GMatrix<Real> &operator-=(GMatrix<Real> &M0, GMatrix<Real> const &M1) {
    if (M0.GetNumRows() == M1.GetNumRows() && M0.GetNumCols() == M1.GetNumCols()) {
        for (int i = 0; i < M0.GetNumElements(); ++i) {
            M0[i] -= M1[i];
        }
        return M0;
    }
    LogError("Mismatched sizes");
}

template<typename Real>
GMatrix<Real> &operator*=(GMatrix<Real> &M, Real scalar) {
    for (int i = 0; i < M.GetNumElements(); ++i) {
        M[i] *= scalar;
    }
    return M;
}

template<typename Real>
GMatrix<Real> &operator/=(GMatrix<Real> &M, Real scalar) {
    if (scalar != (Real) 0) {
        Real invScalar = ((Real) 1) / scalar;
        for (int i = 0; i < M.GetNumElements(); ++i) {
            M[i] *= invScalar;
        }
        return M;
    }
    LogError("Division by zero.");
}

// Geometric operations.
template<typename Real>
Real L1Norm(GMatrix<Real> const &M) {
    Real sum(0);
    for (int i = 0; i < M.GetNumElements(); ++i) {
        sum += std::fabs(M[i]);
    }
    return sum;
}

template<typename Real>
Real L2Norm(GMatrix<Real> const &M) {
    Real sum(0);
    for (int i = 0; i < M.GetNumElements(); ++i) {
        sum += M[i] * M[i];
    }
    return std::sqrt(sum);
}

template<typename Real>
Real LInfinityNorm(GMatrix<Real> const &M) {
    Real maxAbsElement(0);
    for (int i = 0; i < M.GetNumElements(); ++i) {
        Real absElement = std::fabs(M[i]);
        if (absElement > maxAbsElement) {
            maxAbsElement = absElement;
        }
    }
    return maxAbsElement;
}

template<typename Real>
GMatrix<Real> Inverse(GMatrix<Real> const &M, bool *reportInvertibility = nullptr) {
    if (M.GetNumRows() == M.GetNumCols()) {
        GMatrix<Real> invM(M.GetNumRows(), M.GetNumCols());
        Real determinant;
        bool invertible = GaussianElimination<Real>()(M.GetNumRows(), &M[0],
                                                      &invM[0], determinant, nullptr, nullptr, nullptr, 0, nullptr);
        if (reportInvertibility) {
            *reportInvertibility = invertible;
        }
        return invM;
    }
    LogError("Matrix must be square.");
}

template<typename Real>
Real Determinant(GMatrix<Real> const &M) {
    if (M.GetNumRows() == M.GetNumCols()) {
        Real determinant;
        GaussianElimination<Real>()(M.GetNumRows(), &M[0], nullptr,
                                    determinant, nullptr, nullptr, nullptr, 0, nullptr);
        return determinant;
    }
    LogError("Matrix must be square.");
}

// M^T
template<typename Real>
GMatrix<Real> Transpose(GMatrix<Real> const &M) {
    GMatrix<Real> result(M.GetNumCols(), M.GetNumRows());
    for (int r = 0; r < M.GetNumRows(); ++r) {
        for (int c = 0; c < M.GetNumCols(); ++c) {
            result(c, r) = M(r, c);
        }
    }
    return result;
}

// M*V
template<typename Real>
GVector<Real> operator*(GMatrix<Real> const &M, GVector<Real> const &V) {
    if (V.GetSize() == M.GetNumCols()) {
        GVector<Real> result(M.GetNumRows());
        for (int r = 0; r < M.GetNumRows(); ++r) {
            result[r] = (Real) 0;
            for (int c = 0; c < M.GetNumCols(); ++c) {
                result[r] += M(r, c) * V[c];
            }
        }
        return result;
    }
    LogError("Mismatched sizes.");
}

// V^T*M
template<typename Real>
GVector<Real> operator*(GVector<Real> const &V, GMatrix<Real> const &M) {
    if (V.GetSize() == M.GetNumRows()) {
        GVector<Real> result(M.GetNumCols());
        for (int c = 0; c < M.GetNumCols(); ++c) {
            result[c] = (Real) 0;
            for (int r = 0; r < M.GetNumRows(); ++r) {
                result[c] += V[r] * M(r, c);
            }
        }
        return result;
    }
    LogError("Mismatched sizes.");
}

// A*B
template<typename Real>
GMatrix<Real> operator*(GMatrix<Real> const &A, GMatrix<Real> const &B) {
    return MultiplyAB(A, B);
}

template<typename Real>
GMatrix<Real> MultiplyAB(GMatrix<Real> const &A, GMatrix<Real> const &B) {
    if (A.GetNumCols() == B.GetNumRows()) {
        GMatrix<Real> result(A.GetNumRows(), B.GetNumCols());
        int const numCommon = A.GetNumCols();
        for (int r = 0; r < result.GetNumRows(); ++r) {
            for (int c = 0; c < result.GetNumCols(); ++c) {
                result(r, c) = (Real) 0;
                for (int i = 0; i < numCommon; ++i) {
                    result(r, c) += A(r, i) * B(i, c);
                }
            }
        }
        return result;
    }
    LogError("Mismatched sizes.");
}

// A*B^T
template<typename Real>
GMatrix<Real> MultiplyABT(GMatrix<Real> const &A, GMatrix<Real> const &B) {
    if (A.GetNumCols() == B.GetNumCols()) {
        GMatrix<Real> result(A.GetNumRows(), B.GetNumRows());
        int const numCommon = A.GetNumCols();
        for (int r = 0; r < result.GetNumRows(); ++r) {
            for (int c = 0; c < result.GetNumCols(); ++c) {
                result(r, c) = (Real) 0;
                for (int i = 0; i < numCommon; ++i) {
                    result(r, c) += A(r, i) * B(c, i);
                }
            }
        }
        return result;
    }
    LogError("Mismatched sizes.");
}

// A^T*B
template<typename Real>
GMatrix<Real> MultiplyATB(GMatrix<Real> const &A, GMatrix<Real> const &B) {
    if (A.GetNumRows() == B.GetNumRows()) {
        GMatrix<Real> result(A.GetNumCols(), B.GetNumCols());
        int const numCommon = A.GetNumRows();
        for (int r = 0; r < result.GetNumRows(); ++r) {
            for (int c = 0; c < result.GetNumCols(); ++c) {
                result(r, c) = (Real) 0;
                for (int i = 0; i < numCommon; ++i) {
                    result(r, c) += A(i, r) * B(i, c);
                }
            }
        }
        return result;
    }
    LogError("Mismatched sizes.");
}

// A^T*B^T
template<typename Real>
GMatrix<Real> MultiplyATBT(GMatrix<Real> const &A, GMatrix<Real> const &B) {
    if (A.GetNumRows() == B.GetNumCols()) {
        GMatrix<Real> result(A.GetNumCols(), B.GetNumRows());
        int const numCommon = A.GetNumRows();
        for (int r = 0; r < result.GetNumRows(); ++r) {
            for (int c = 0; c < result.GetNumCols(); ++c) {
                result(r, c) = (Real) 0;
                for (int i = 0; i < numCommon; ++i) {
                    result(r, c) += A(i, r) * B(c, i);
                }
            }
        }
        return result;
    }
    LogError("Mismatched sizes.");
}

// M*D, D is square diagonal (stored as vector)
template<typename Real>
GMatrix<Real> MultiplyMD(GMatrix<Real> const &M, GVector<Real> const &D) {
    if (D.GetSize() == M.GetNumCols()) {
        GMatrix<Real> result(M.GetNumRows(), M.GetNumCols());
        for (int r = 0; r < result.GetNumRows(); ++r) {
            for (int c = 0; c < result.GetNumCols(); ++c) {
                result(r, c) = M(r, c) * D[c];
            }
        }
        return result;
    }
    LogError("Mismatched sizes.");
}

// D*M, D is square diagonal (stored as vector)
template<typename Real>
GMatrix<Real> MultiplyDM(GVector<Real> const &D, GMatrix<Real> const &M) {
    if (D.GetSize() == M.GetNumRows()) {
        GMatrix<Real> result(M.GetNumRows(), M.GetNumCols());
        for (int r = 0; r < result.GetNumRows(); ++r) {
            for (int c = 0; c < result.GetNumCols(); ++c) {
                result(r, c) = D[r] * M(r, c);
            }
        }
        return result;
    }
    LogError("Mismatched sizes.");
}

// U*V^T, U is N-by-1, V is M-by-1, result is N-by-M.
template<typename Real>
GMatrix<Real> OuterProduct(GVector<Real> const &U, GVector<Real> const &V) {
    GMatrix<Real> result(U.GetSize(), V.GetSize());
    for (int r = 0; r < result.GetNumRows(); ++r) {
        for (int c = 0; c < result.GetNumCols(); ++c) {
            result(r, c) = U[r] * V[c];
        }
    }
    return result;
}

// Initialization to a diagonal matrix whose diagonal entries are the
// components of D, even when nonsquare.
template<typename Real>
void MakeDiagonal(GVector<Real> const &D, GMatrix<Real> &M) {
    int const numRows = M.GetNumRows();
    int const numCols = M.GetNumCols();
    int const numDiagonal = (numRows <= numCols ? numRows : numCols);
    M.MakeZero();
    for (int i = 0; i < numDiagonal; ++i) {
        M(i, i) = D[i];
    }
}
}
