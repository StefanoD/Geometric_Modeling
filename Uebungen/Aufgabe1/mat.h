////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#ifndef _MAT_H_
#define _MAT_H_

#include <string>

template <class T, unsigned SIZE> class CVector;

// template square matrix class for SIMPLE data types
template <class T, unsigned SIZE>
class CMatrix {

public:
    // constructors
    CMatrix<T, SIZE> (              T         t=T(1)             ); // default constructor: identity
    CMatrix<T, SIZE> (const         T         aatData[SIZE][SIZE]); // construction with data array
    CMatrix<T, SIZE> (const CMatrix<T, SIZE> &mat                ); // copy constructor

    // destructor
    ~CMatrix<T, SIZE> ();

    // index operators
    T &operator () (unsigned i, unsigned j);
    T  operator () (unsigned i, unsigned j) const;

    // arithmetic operators
    CMatrix<T, SIZE> operator + (const CMatrix<T, SIZE> &mat);
    CMatrix<T, SIZE> operator - (const CMatrix<T, SIZE> &mat);
    CMatrix<T, SIZE> operator * (const CMatrix<T, SIZE> &mat);
    CVector<T, SIZE> operator * (const CVector<T, SIZE> &vec);

    CMatrix<T, SIZE> getTransposed();

    // output
    void print(std::string name= "v") const;
private:

    T m_aatData[SIZE][SIZE];
};

// some common vector classes (abbr. names)
typedef CMatrix<float, 2u> CMat2f;
typedef CMatrix<float, 3u> CMat3f;
typedef CMatrix<float, 4u> CMat4f;

typedef CMatrix<double, 2u> CMat2d;
typedef CMatrix<double, 3u> CMat3d;
typedef CMatrix<double, 4u> CMat4d;


template <class T, unsigned SIZE>
CMatrix<T, SIZE>::CMatrix (T t)
{
    for (unsigned j=0; j<SIZE; j++) {
        for (unsigned i=0; i<SIZE; i++) {
            m_aatData[i][j] = (i==j)? t: T(0);
        }
    }
}


template <class T, unsigned SIZE>
CMatrix<T, SIZE>::CMatrix (const T aatData[SIZE][SIZE])
{
    for (unsigned j=0; j<SIZE; j++) {
        for (unsigned i=0; i<SIZE; i++) {
            m_aatData[i][j] = aatData[i][j];
}
        }
    }

template <class T, unsigned SIZE>
CMatrix<T, SIZE>::CMatrix (const CMatrix<T, SIZE> &mat)
{
    for (unsigned j=0; j<SIZE; j++) {
        for (unsigned i=0; i<SIZE; i++) {
            m_aatData[i][j] = mat(i,j);
        }
    }
}

template <class T, unsigned SIZE>
CMatrix<T, SIZE>::~CMatrix ()
{
    // nothing to do here ...
}

template <class T, unsigned SIZE>
T &CMatrix<T, SIZE>::operator () (unsigned i, unsigned j)
{
    return m_aatData[i%SIZE][j%SIZE];
}

template <class T, unsigned SIZE>
T CMatrix<T, SIZE>::operator () (unsigned i, unsigned j) const
{
    return m_aatData[i%SIZE][j%SIZE];
}

template <class T, unsigned SIZE>
CMatrix<T, SIZE> CMatrix<T, SIZE>::operator + (const CMatrix<T, SIZE> &mat)
{
    CMatrix<T, SIZE> buf(T(0));
    for (int i=0; i<SIZE; i++)
        for (int j=0; j<SIZE; j++)
            buf(i,j) = m_aatData[i][j] + mat(i,j);
    return buf;
}

template <class T, unsigned SIZE>
CMatrix<T, SIZE> CMatrix<T, SIZE>::operator - (const CMatrix<T, SIZE> &mat)
{
    CMatrix<T, SIZE> buf(T(0));
    for (int i=0; i<SIZE; i++)
        for (int j=0; j<SIZE; j++)
            buf(i,j) = m_aatData[i][j] - mat(i,j);
    return buf;
}

template <class T, unsigned SIZE>
CMatrix<T, SIZE> CMatrix<T, SIZE>::operator * (const CMatrix<T, SIZE> &mat)
{
    CMatrix<T, SIZE> buf(T(0));
    for (unsigned i=0; i<SIZE; i++) // ZEILE i
        for (unsigned j=0; j<SIZE; j++) // Spalte j
            for (unsigned k=0; k<SIZE; k++) // k
                buf(i,j) += m_aatData[i][k] * mat(k,j);
    return buf;
}

template <class T, unsigned SIZE>
CVector<T, SIZE> CMatrix<T, SIZE>::operator * (const CVector<T, SIZE> &vec)
{
    CVector<T, SIZE> buf;
    for (unsigned j=0; j<SIZE; j++) // SPALTE j
        for (unsigned i=0; i<SIZE; i++) // ZEILE i
            buf(i) += m_aatData[i][j]*vec(j);
    return buf;
}
template <class T, unsigned SIZE>
void CMatrix<T, SIZE>::print(std::string name) const
{
    std::cout << name << "\n";
    for (int j=0; j<SIZE; j++) {			// ZEILE j
        for (int i=0; i<SIZE; i++) {		// SPALTE i
            std::cout << m_aatData[j][i];
            if (i<SIZE-1) std::cout << " ";
            else          std::cout << "\n";
        }
    }
    std::cout << "\n";
}

template <class T, unsigned SIZE>
CMatrix<T, SIZE>  CMatrix<T, SIZE>::getTransposed()
{
    CMatrix<T, SIZE> transposedMatrix;

    for (unsigned i = 0; i < SIZE; ++i) {
        for (unsigned j = 0; j < SIZE; ++j) {
            transposedMatrix(j, i) = m_aatData[i][j];
        }
    }

    return transposedMatrix;
}


#endif
