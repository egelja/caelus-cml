/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::Matrix

Description
    A templated 2D matrix of objects of \<T\>, where the n x m matrix
    dimensions are known and used for subscript bounds checking, etc.

SourceFiles
    Matrix.cpp
    MatrixI.hpp
    MatrixIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef Matrix_H
#define Matrix_H

#include "bool.hpp"
#include "label.hpp"
#include "uLabel.hpp"
#include "List.hpp"
#include "autoPtr.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Form, class Type> class Matrix;

template<class Form, class Type> Istream& operator>>
(
    Istream&,
    Matrix<Form, Type>&
);

template<class Form, class Type> Ostream& operator<<
(
    Ostream&,
    const Matrix<Form, Type>&
);


/*---------------------------------------------------------------------------*\
                           Class Matrix Declaration
\*---------------------------------------------------------------------------*/

template<class Form, class Type>
class Matrix
{
    // Private data

        //- Number of rows and columns in Matrix.
        label n_, m_;

        //- Row pointers
        Type** RESTRICT v_;

        //- Allocate the storage for the row-pointers and the data
        //  and set the row pointers
        void allocate();


public:

    // Static Member Functions

        //- Return a null Matrix
        inline static const Matrix<Form, Type>& null();


    // Constructors

        //- Null constructor.
        inline Matrix();

        //- Construct given number of rows and columns.
        Matrix(const label n, const label m);

        //- Construct with given number of rows and columns
        //  and value for all elements.
        Matrix(const label n, const label m, const Type&);

        //- Copy constructor.
        Matrix(const Matrix<Form, Type>&);

        //- Construct from Istream.
        Matrix(Istream&);

        //- Clone
        inline autoPtr<Matrix<Form, Type> > clone() const;


    //- Destructor
    ~Matrix();


    // Member Functions

        // Access

            //- Return the number of rows
            inline label n() const;

            //- Return the number of columns
            inline label m() const;

            //- Return the number of elements in matrix (n*m)
            inline label size() const;


        // Check

            //- Check index i is within valid range (0 ... n-1).
            inline void checki(const label i) const;

            //- Check index j is within valid range (0 ... m-1).
            inline void checkj(const label j) const;


        // Edit

            //- Clear the Matrix, i.e. set sizes to zero.
            void clear();

            //- Transfer the contents of the argument Matrix into this Matrix
            //  and annul the argument Matrix.
            void transfer(Matrix<Form, Type>&);


        //- Return the transpose of the matrix
        Form T() const;


    // Member operators

        //- Return subscript-checked row of Matrix.
        inline Type* operator[](const label);

        //- Return subscript-checked row of constant Matrix.
        inline const Type* operator[](const label) const;

        //- Assignment operator. Takes linear time.
        void operator=(const Matrix<Form, Type>&);

        //- Assignment of all entries to the given value
        void operator=(const Type&);


    // IOstream operators

        //- Read Matrix from Istream, discarding contents of existing Matrix.
        friend Istream& operator>> <Form, Type>
        (
            Istream&,
            Matrix<Form, Type>&
        );

        // Write Matrix to Ostream.
        friend Ostream& operator<< <Form, Type>
        (
            Ostream&,
            const Matrix<Form, Type>&
        );
};


// Global functions and operators

template<class Form, class Type> const Type& max(const Matrix<Form, Type>&);
template<class Form, class Type> const Type& min(const Matrix<Form, Type>&);

template<class Form, class Type> Form operator-(const Matrix<Form, Type>&);

template<class Form, class Type> Form operator+
(
    const Matrix<Form, Type>&,
    const Matrix<Form, Type>&
);

template<class Form, class Type> Form operator-
(
    const Matrix<Form, Type>&,
    const Matrix<Form, Type>&
);

template<class Form, class Type> Form operator*
(
    const scalar,
    const Matrix<Form, Type>&
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Form, class Type>
inline CML::Matrix<Form, Type>::Matrix()
:
    n_(0),
    m_(0),
    v_(NULL)
{}


template<class Form, class Type>
inline CML::autoPtr<CML::Matrix<Form, Type> > CML::Matrix<Form, Type>::
clone() const
{
    return autoPtr<Matrix<Form, Type> >(new Matrix<Form, Type>(*this));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Form, class Type>
inline const CML::Matrix<Form, Type>& CML::Matrix<Form, Type>::null()
{
    return NullSingletonRef< Matrix<Form, Type> >();
}


//- Return the number of rows
template<class Form, class Type>
inline CML::label CML::Matrix<Form, Type>::n() const
{
    return n_;
}


template<class Form, class Type>
inline CML::label CML::Matrix<Form, Type>::m() const
{
    return m_;
}


template<class Form, class Type>
inline CML::label CML::Matrix<Form, Type>::size() const
{
    return n_*m_;
}


template<class Form, class Type>
inline void CML::Matrix<Form, Type>::checki(const label i) const
{
    if (!n_)
    {
        FatalErrorIn("Matrix<Form, Type>::checki(const label)")
            << "attempt to access element from zero sized row"
            << abort(FatalError);
    }
    else if (i<0 || i>=n_)
    {
        FatalErrorIn("Matrix<Form, Type>::checki(const label)")
            << "index " << i << " out of range 0 ... " << n_-1
            << abort(FatalError);
    }
}


template<class Form, class Type>
inline void CML::Matrix<Form, Type>::checkj(const label j) const
{
    if (!m_)
    {
        FatalErrorIn("Matrix<Form, Type>::checkj(const label)")
            << "attempt to access element from zero sized column"
            << abort(FatalError);
    }
    else if (j<0 || j>=m_)
    {
        FatalErrorIn("Matrix<Form, Type>::checkj(const label)")
            << "index " << j << " out of range 0 ... " << m_-1
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Form, class Type>
inline Type* CML::Matrix<Form, Type>::operator[](const label i)
{
#   ifdef FULLDEBUG
    checki(i);
#   endif
    return v_[i];
}


template<class Form, class Type>
inline const Type* CML::Matrix<Form, Type>::operator[](const label i) const
{
#   ifdef FULLDEBUG
    checki(i);
#   endif
    return v_[i];
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class Form, class Type>
void CML::Matrix<Form, Type>::allocate()
{
    if (n_ && m_)
    {
        v_ = new Type*[n_];
        v_[0] = new Type[n_*m_];

        for (register label i=1; i<n_; i++)
        {
            v_[i] = v_[i-1] + m_;
        }
    }
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

template<class Form, class Type>
CML::Matrix<Form, Type>::~Matrix()
{
    if (v_)
    {
        delete[] (v_[0]);
        delete[] v_;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Form, class Type>
CML::Matrix<Form, Type>::Matrix(const label n, const label m)
:
    n_(n),
    m_(m),
    v_(NULL)
{
    if (n_ < 0 || m_ < 0)
    {
        FatalErrorIn("Matrix<Form, Type>::Matrix(const label n, const label m)")
            << "bad n, m " << n_ << ", " << m_
            << abort(FatalError);
    }

    allocate();
}


template<class Form, class Type>
CML::Matrix<Form, Type>::Matrix(const label n, const label m, const Type& a)
:
    n_(n),
    m_(m),
    v_(NULL)
{
    if (n_ < 0 || m_ < 0)
    {
        FatalErrorIn
        (
            "Matrix<Form, Type>::Matrix(const label n, const label m, const T&)"
        )   << "bad n, m " << n_ << ", " << m_
            << abort(FatalError);
    }

    allocate();

    if (v_)
    {
        Type* v = v_[0];

        label nm = n_*m_;

        for (register label i=0; i<nm; i++)
        {
            v[i] = a;
        }
    }
}


template<class Form, class Type>
CML::Matrix<Form, Type>::Matrix(const Matrix<Form, Type>& a)
:
    n_(a.n_),
    m_(a.m_),
    v_(NULL)
{
    if (a.v_)
    {
        allocate();
        Type* v = v_[0];
        const Type* av = a.v_[0];

        label nm = n_*m_;
        for (register label i=0; i<nm; i++)
        {
            v[i] = av[i];
        }
    }
}


template<class Form, class Type>
void CML::Matrix<Form, Type>::clear()
{
    if (v_)
    {
        delete[] (v_[0]);
        delete[] v_;
    }
    n_ = 0;
    m_ = 0;
    v_ = NULL;
}


template<class Form, class Type>
void CML::Matrix<Form, Type>::transfer(Matrix<Form, Type>& a)
{
    clear();

    n_ = a.n_;
    a.n_ = 0;

    m_ = a.m_;
    a.m_ = 0;

    v_ = a.v_;
    a.v_ = NULL;
}


template<class Form, class Type>
Form CML::Matrix<Form, Type>::T() const
{
    const Matrix<Form, Type>& A = *this;
    Form At(m(), n());

    for (register label i=0; i<n(); i++)
    {
        for (register label j=0; j<m(); j++)
        {
            At[j][i] = A[i][j];
        }
    }

    return At;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Form, class Type>
void CML::Matrix<Form, Type>::operator=(const Type& t)
{
    if (v_)
    {
        Type* v = v_[0];

        label nm = n_*m_;
        for (register label i=0; i<nm; i++)
        {
            v[i] = t;
        }
    }
}


// Assignment operator. Takes linear time.
template<class Form, class Type>
void CML::Matrix<Form, Type>::operator=(const Matrix<Form, Type>& a)
{
    if (this == &a)
    {
        FatalErrorIn("Matrix<Form, Type>::operator=(const Matrix<Form, Type>&)")
            << "attempted assignment to self"
            << abort(FatalError);
    }

    if (n_ != a.n_ || m_ != a.m_)
    {
        clear();
        n_ = a.n_;
        m_ = a.m_;
        allocate();
    }

    if (v_)
    {
        Type* v = v_[0];
        const Type* av = a.v_[0];

        label nm = n_*m_;
        for (register label i=0; i<nm; i++)
        {
            v[i] = av[i];
        }
    }
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Form, class Type>
const Type& CML::max(const Matrix<Form, Type>& a)
{
    label nm = a.n()*a.m();

    if (nm)
    {
        label curMaxI = 0;
        const Type* v = a[0];

        for (register label i=1; i<nm; i++)
        {
            if (v[i] > v[curMaxI])
            {
                curMaxI = i;
            }
        }

        return v[curMaxI];
    }
    else
    {
        FatalErrorIn("max(const Matrix<Form, Type>&)")
            << "matrix is empty"
            << abort(FatalError);

        // Return in error to keep compiler happy
        return a[0][0];
    }
}


template<class Form, class Type>
const Type& CML::min(const Matrix<Form, Type>& a)
{
    label nm = a.n()*a.m();

    if (nm)
    {
        label curMinI = 0;
        const Type* v = a[0];

        for (register label i=1; i<nm; i++)
        {
            if (v[i] < v[curMinI])
            {
                curMinI = i;
            }
        }

        return v[curMinI];
    }
    else
    {
        FatalErrorIn("min(const Matrix<Form, Type>&)")
            << "matrix is empty"
            << abort(FatalError);

        // Return in error to keep compiler happy
        return a[0][0];
    }
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Form, class Type>
Form CML::operator-(const Matrix<Form, Type>& a)
{
    Form na(a.n(), a.m());

    if (a.n() && a.m())
    {
        Type* nav = na[0];
        const Type* av = a[0];

        label nm = a.n()*a.m();
        for (register label i=0; i<nm; i++)
        {
            nav[i] = -av[i];
        }
    }

    return na;
}


template<class Form, class Type>
Form CML::operator+(const Matrix<Form, Type>& a, const Matrix<Form, Type>& b)
{
    if (a.n() != b.n())
    {
        FatalErrorIn
        (
            "Matrix<Form, Type>::operator+"
            "(const Matrix<Form, Type>&, const Matrix<Form, Type>&)"
        )   << "attempted add matrices with different number of rows: "
            << a.n() << ", " << b.n()
            << abort(FatalError);
    }

    if (a.m() != b.m())
    {
        FatalErrorIn
        (
            "Matrix<Form, Type>::operator+"
            "(const Matrix<Form, Type>&, const Matrix<Form, Type>&)"
        )   << "attempted add matrices with different number of columns: "
            << a.m() << ", " << b.m()
            << abort(FatalError);
    }

    Form ab(a.n(), a.m());

    Type* abv = ab[0];
    const Type* av = a[0];
    const Type* bv = b[0];

    label nm = a.n()*a.m();
    for (register label i=0; i<nm; i++)
    {
        abv[i] = av[i] + bv[i];
    }

    return ab;
}


template<class Form, class Type>
Form CML::operator-(const Matrix<Form, Type>& a, const Matrix<Form, Type>& b)
{
    if (a.n() != b.n())
    {
        FatalErrorIn
        (
            "Matrix<Form, Type>::operator-"
            "(const Matrix<Form, Type>&, const Matrix<Form, Type>&)"
        )   << "attempted add matrices with different number of rows: "
            << a.n() << ", " << b.n()
            << abort(FatalError);
    }

    if (a.m() != b.m())
    {
        FatalErrorIn
        (
            "Matrix<Form, Type>::operator-"
            "(const Matrix<Form, Type>&, const Matrix<Form, Type>&)"
        )   << "attempted add matrices with different number of columns: "
            << a.m() << ", " << b.m()
            << abort(FatalError);
    }

    Form ab(a.n(), a.m());

    Type* abv = ab[0];
    const Type* av = a[0];
    const Type* bv = b[0];

    label nm = a.n()*a.m();
    for (register label i=0; i<nm; i++)
    {
        abv[i] = av[i] - bv[i];
    }

    return ab;
}


template<class Form, class Type>
Form CML::operator*(const scalar s, const Matrix<Form, Type>& a)
{
    Form sa(a.n(), a.m());

    if (a.n() && a.m())
    {
        Type* sav = sa[0];
        const Type* av = a[0];

        label nm = a.n()*a.m();
        for (register label i=0; i<nm; i++)
        {
            sav[i] = s*av[i];
        }
    }

    return sa;
}


// * * * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * //

#include "Istream.hpp"
#include "Ostream.hpp"
#include "token.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * Ostream Operator *  * * * * * * * * * * * * //

template<class Form, class Type>
CML::Matrix<Form, Type>::Matrix(Istream& is)
:
    n_(0),
    m_(0),
    v_(NULL)
{
    operator>>(is, *this);
}


template<class Form, class Type>
CML::Istream& CML::operator>>(Istream& is, Matrix<Form, Type>& M)
{
    // Anull matrix
    M.clear();

    is.fatalCheck("operator>>(Istream&, Matrix<Form, Type>&)");

    token firstToken(is);

    is.fatalCheck
    (
        "operator>>(Istream&, Matrix<Form, Type>&) : reading first token"
    );

    if (firstToken.isLabel())
    {
        M.n_ = firstToken.labelToken();
        M.m_ = readLabel(is);

        label nm = M.n_*M.m_;

        // Read list contents depending on data format
        if (is.format() == IOstream::ASCII || !contiguous<Type>())
        {
            // Read beginning of contents
            char listDelimiter = is.readBeginList("Matrix");

            if (nm)
            {
                M.allocate();
                Type* v = M.v_[0];

                if (listDelimiter == token::BEGIN_LIST)
                {
                    label k = 0;

                    // loop over rows
                    for (register label i=0; i<M.n(); i++)
                    {
                        listDelimiter = is.readBeginList("MatrixRow");

                        for (register label j=0; j<M.m(); j++)
                        {
                            is >> v[k++];

                            is.fatalCheck
                            (
                                "operator>>(Istream&, Matrix<Form, Type>&) : "
                                "reading entry"
                            );
                        }

                        is.readEndList("MatrixRow");
                    }
                }
                else
                {
                    Type element;
                    is >> element;

                    is.fatalCheck
                    (
                        "operator>>(Istream&, Matrix<Form, Type>&) : "
                        "reading the single entry"
                    );

                    for (register label i=0; i<nm; i++)
                    {
                        v[i] = element;
                    }
                }
            }

            // Read end of contents
            is.readEndList("Matrix");
        }
        else
        {
            if (nm)
            {
                M.allocate();
                Type* v = M.v_[0];

                is.read(reinterpret_cast<char*>(v), nm*sizeof(Type));

                is.fatalCheck
                (
                    "operator>>(Istream&, Matrix<Form, Type>&) : "
                    "reading the binary block"
                );
            }
        }
    }
    else
    {
        FatalIOErrorIn("operator>>(Istream&, Matrix<Form, Type>&)", is)
            << "incorrect first token, expected <int>, found "
            << firstToken.info()
            << exit(FatalIOError);
    }

    return is;
}


template<class Form, class Type>
CML::Ostream& CML::operator<<(Ostream& os, const Matrix<Form, Type>& M)
{
    label nm = M.n_*M.m_;

    os  << M.n() << token::SPACE << M.m();

    // Write list contents depending on data format
    if (os.format() == IOstream::ASCII || !contiguous<Type>())
    {
        if (nm)
        {
            bool uniform = false;

            const Type* v = M.v_[0];

            if (nm > 1 && contiguous<Type>())
            {
                uniform = true;

                for (register label i=0; i< nm; i++)
                {
                    if (v[i] != v[0])
                    {
                        uniform = false;
                        break;
                    }
                }
            }

            if (uniform)
            {
                // Write size of list and start contents delimiter
                os  << token::BEGIN_BLOCK;

                // Write list contents
                os << v[0];

                // Write end of contents delimiter
                os << token::END_BLOCK;
            }
            else if (nm < 10 && contiguous<Type>())
            {
                // Write size of list and start contents delimiter
                os  << token::BEGIN_LIST;

                label k = 0;

                // loop over rows
                for (register label i=0; i< M.n(); i++)
                {
                    os  << token::BEGIN_LIST;

                    // Write row
                    for (register label j=0; j< M.m(); j++)
                    {
                        if (j > 0) os << token::SPACE;
                        os << v[k++];
                    }

                    os << token::END_LIST;
                }

                // Write end of contents delimiter
                os << token::END_LIST;
            }
            else
            {
                // Write size of list and start contents delimiter
                os  << nl << token::BEGIN_LIST;

                label k = 0;

                // loop over rows
                for (register label i=0; i< M.n(); i++)
                {
                    os  << nl << token::BEGIN_LIST;

                    // Write row
                    for (register label j=0; j< M.m(); j++)
                    {
                        os << nl << v[k++];
                    }

                    os << nl << token::END_LIST;
                }

                // Write end of contents delimiter
                os << nl << token::END_LIST << nl;
            }
        }
        else
        {
            os  << token::BEGIN_LIST << token::END_LIST << nl;
        }
    }
    else
    {
        if (nm)
        {
            os.write(reinterpret_cast<const char*>(M.v_[0]), nm*sizeof(Type));
        }
    }

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const Matrix&)");

    return os;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
