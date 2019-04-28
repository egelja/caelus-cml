/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2016 H. Jasak
Copyright (C) 2017-2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    hyperTensor

Description
    Templated NXN Tensor derived from VectorSpace adding construction from
    N components, and the inner-product (dot-product) and outer-product
    operators.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.
    
    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.
    
Author
    Hrvoje Jasak, Wikki Ltd.
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef hyperTensor_HPP
#define hyperTensor_HPP

#include "hyperVector.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class hyperTensor Declaration
\*---------------------------------------------------------------------------*/

template <class Cmpt, int length>
class hyperTensor
:
    public VectorSpace<hyperTensor<Cmpt, length>, Cmpt, length*length>
{

public:

    // Member constants
    enum
    {
        rank = 2,            // Rank of hyperTensor is 2
        rowLength = length   // Number of components in a row
    };

    // Static data members
    static const char* const typeName;
    static const hyperTensor zero;
    static const hyperTensor one;

    //- Construct null
    inline hyperTensor();

    //- Construct given VectorSpace
    inline hyperTensor
    (
        const VectorSpace<hyperTensor<Cmpt, length>, Cmpt, length*length>&
    );

    //- Construct given component value.  Special use only!
    explicit inline hyperTensor(const Cmpt& tx);

    //- Construct from Istream
    hyperTensor(Istream&);

    //- Return direction given (i, j) indices
    static inline direction cmpt
    (
        const direction i,
        const direction j
    );

    //- Return (i, j) component
    inline const Cmpt& operator()
    (
        const direction i,
        const direction j
    ) const;

    //- Return access to (i, j) component
    inline Cmpt& operator()
    (
        const direction i,
        const direction j
    );

    //- Return transpose
    inline hyperTensor<Cmpt, length> T() const;

    //- Negative sum the vertical off-diagonal components
    inline hyperTensor<Cmpt, length> negSumDiag() const;

};


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template <class Cmpt, int length>
const char* const hyperTensor<Cmpt, length>::typeName =
    ("tensor" + name(length)).c_str();


template <class Cmpt, int length>
const hyperTensor<Cmpt, length> hyperTensor<Cmpt, length>::zero(0);


template <class Cmpt, int length>
const hyperTensor<Cmpt, length> hyperTensor<Cmpt, length>::one(1);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct null
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length>::hyperTensor()
{}


//- Construct given VectorSpace
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length>::hyperTensor
(
    const VectorSpace<hyperTensor<Cmpt, length>, Cmpt, length*length>& vs
)
:
    VectorSpace<hyperTensor<Cmpt, length>, Cmpt, length*length>(vs)
{}


//- Construct from component
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length>::hyperTensor(const Cmpt& tx)
{
    VectorSpaceOps<hyperTensor<Cmpt, length>::nComponents, 0>::eqOpS
    (
        *this,
        tx,
        eqOp<Cmpt>()
    );
}


//- Construct from Istream
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length>::hyperTensor(Istream& is)
:
    VectorSpace<hyperTensor<Cmpt, length>, Cmpt, length*length>(is)
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Return tensor transpose
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length> hyperTensor<Cmpt, length>::T() const
{
    hyperTensor<Cmpt, length> transpose;

    label i = 0;
    for (label row = 0; row < hyperTensor<Cmpt, length>::rowLength; row++)
    {
        label j = row;
        for (label col = 0; col < hyperTensor<Cmpt, length>::rowLength; col++)
        {
            transpose.v_[i] = this->v_[j];
            i++;
            j += hyperTensor<Cmpt, length>::rowLength;
        }
    }

    return transpose;
}


//- Negative sum the vertical off-diagonal components
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length> hyperTensor<Cmpt, length>::negSumDiag() const
{
    hyperTensor<Cmpt, length> negsumdiag;

    // Zero main diagonal
    label diagI = 0;
    for (label i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        negsumdiag.v_[diagI] = 0.0;
        diagI += hyperTensor<Cmpt, length>::rowLength + 1;
    }

    label k = 0;
    for (label i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        diagI = 0;
        for (label j = 0; j < hyperTensor<Cmpt, length>::rowLength; j++)
        {
            if (k != diagI)
            {
                negsumdiag.v_[k] = this->v_[k];
                negsumdiag.v_[diagI] -= this->v_[k];
            }
            k++;
            diagI += hyperTensor<Cmpt, length>::rowLength + 1;
        }
    }

    return negsumdiag;
}


//- Transform the tensor
//- The components are assumed to be individual scalars
//- i.e. transform has no effect
template<class Cmpt, int length>
inline hyperTensor<Cmpt, length> transform
(
    const tensor& tt,
    const hyperTensor<Cmpt, length>& v
)
{
    return v;
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template <class Cmpt, int length>
inline direction hyperTensor<Cmpt, length>::cmpt
(
    const direction i,
    const direction j
)
{
    if
    (
        i > hyperTensor<Cmpt, length>::rowLength - 1
     || j > hyperTensor<Cmpt, length>::rowLength - 1
    )
    {
        FatalErrorInFunction
            << "Direction out of range (0 "
            << hyperTensor<Cmpt, length>::rowLength - 1 << ")"
            << " and (i j) = ("  << i << " " << j << ")"
            << abort(FatalError);
    }

    return i*hyperTensor<Cmpt, length>::rowLength + j;
}


template <class Cmpt, int length>
inline const Cmpt& hyperTensor<Cmpt, length>::operator()
(
    const direction i,
    const direction j
) const
{
    return this->operator[](i*hyperTensor<Cmpt, length>::rowLength + j);
}


template <class Cmpt, int length>
inline Cmpt& hyperTensor<Cmpt, length>::operator()
(
    const direction i,
    const direction j
)
{
    return this->operator[](i*hyperTensor<Cmpt, length>::rowLength + j);
}

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

//- Inner-product between two tensors
template <class Cmpt, int length>
inline typename
innerProduct<hyperTensor<Cmpt, length>, hyperTensor<Cmpt, length> >::type
operator&
(
    const hyperTensor<Cmpt, length>& t1,
    const hyperTensor<Cmpt, length>& t2
)
{
    hyperTensor<Cmpt, length> result(hyperTensor<Cmpt, length>::zero);

    label i = 0;
    label j = 0;

    label m, n;

    for (label row = 0; row < hyperTensor<Cmpt, length>::rowLength; row++)
    {
        for (label col = 0; col < hyperTensor<Cmpt, length>::rowLength; col++)
        {
            Cmpt& r = result.v_[i];
            m = j;
            n = col;

            for
            (
                label row2 = 0;
                row2 < hyperTensor<Cmpt, length>::rowLength;
                row2++
            )
            {
                r += t1.v_[m]*t2.v_[n];
                m++;
                n += hyperTensor<Cmpt, length>::rowLength;
            }
            i++;
        }
        j += hyperTensor<Cmpt, length>::rowLength;
    }

    return result;
}


//- Inner-product between a tensor and a vector
template <class Cmpt, int length>
inline typename
innerProduct<hyperTensor<Cmpt, length>, hyperVector<Cmpt, length> >::type
operator&
(
    const hyperTensor<Cmpt, length>& t,
    const hyperVector<Cmpt, length>& v
)
{
    hyperVector<Cmpt, length> result(hyperVector<Cmpt, length>::zero);

    label i = 0;
    for (label row = 0; row < hyperTensor<Cmpt, length>::rowLength; row++)
    {
        Cmpt& r = result.v_[row];

        for (label col = 0; col < hyperTensor<Cmpt, length>::rowLength; col++)
        {
            r += t.v_[i]*v.v_[col];
            i++;
        }
    }

    return result;
}


//- Inner-product between a vector and a tensor
template <class Cmpt, int length>
inline typename
innerProduct<hyperVector<Cmpt, length>, hyperTensor<Cmpt, length> >::type
operator&
(
    const hyperVector<Cmpt, length>& v,
    const hyperTensor<Cmpt, length>& t
)
{
    hyperVector<Cmpt, length> result(hyperVector<Cmpt, length>::zero);

    for (label col = 0; col < hyperTensor<Cmpt, length>::rowLength; col++)
    {
        label j = col;
        Cmpt& r = result.v_[col];

        for (label row = 0; row < hyperTensor<Cmpt, length>::rowLength; row++)
        {
            r += v.v_[row]*t.v_[j];
            j += hyperTensor<Cmpt, length>::rowLength;
        }
    }

    return result;
}


//- Outer-product between two vectors
template <class Cmpt, int length>
inline typename
outerProduct<hyperVector<Cmpt, length>, hyperVector<Cmpt, length> >::type
operator*
(
    const hyperVector<Cmpt, length>& v1,
    const hyperVector<Cmpt, length>& v2
)
{
    hyperTensor<Cmpt, length> result(hyperTensor<Cmpt, length>::zero);

    label i = 0;
    for (label row = 0; row < hyperTensor<Cmpt, length>::rowLength; row++)
    {
        for (label col = 0; col < hyperTensor<Cmpt, length>::rowLength; col++)
        {
            result.v_[i] = v1.v_[row]*v2.v_[col];
            i++;
        }
    }

    return result;
}


//- Addition of hyperTensor and hyperTensor
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length>
operator+
(
    const hyperTensor<Cmpt, length>& t1,
    const hyperTensor<Cmpt, length>& t2
)
{
    hyperTensor<Cmpt, length> res;
    VectorSpaceOps<hyperTensor<Cmpt, length>::nComponents, 0>::op
    (
        res,
        t1,
        t2,
        plusOp<Cmpt>()
    );

    return res;
}


//- Subtraction of hyperTensor and hyperTensor
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length>
operator-
(
    const hyperTensor<Cmpt, length>& t1,
    const hyperTensor<Cmpt, length>& t2
)
{
    hyperTensor<Cmpt, length> res;
    VectorSpaceOps<hyperTensor<Cmpt, length>::nComponents, 0>::op
    (
        res,
        t1,
        t2,
        minusOp<Cmpt>()
    );

    return res;
}


//- Division of a scalar by a hyperTensor
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length>
operator/(const scalar s, const hyperTensor<Cmpt, length>& t)
{
    return s*inv(t);
}


//- Inner Product of a hyperVector by an inverse hyperTensor
template <class Cmpt, int length>
inline hyperVector<Cmpt, length>
operator/
(
    const hyperVector<Cmpt, length>& v,
    const hyperTensor<Cmpt, length>& t
)
{
    return v & inv(t);
}


//- Inner Product of a hyperTensor by an inverse hyperTensor
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length>
operator/
(
    const hyperTensor<Cmpt, length>& t1,
    const hyperTensor<Cmpt, length>& t2
)
{
    return t1 & inv(t2);
}


template <class Cmpt, int length>
inline Cmpt det(const hyperTensor<Cmpt, length>& t)
{
    // Calculate determinant via sub-determinants
    Cmpt result = pTraits<Cmpt>::zero;

    hyperTensor<Cmpt, length - 1> subMatrix;

    // Laplace expansion using first column
    // Bugfix, TU, 31 May 2016

    for (label i = 0; i < length; i++)
    {
        // Column of submatrix
        label js = 0;

        for (label j = 1; j < length; j++)
        {
            // Row of submatrix
            label is = 0;

            for (label k = 0; k < length; k++)
            {
                // Row which is crossed out in the expansion
                if (k == i)
                {
                    continue;
                }
                else
                {
                    subMatrix(is, js) = t(k, j);
                    is++;
                }
            }
            js++;
        }

        // Handle +/- sign switch
        result += pow(scalar(-1.0), i)*t(i, 0)*det(subMatrix);
    }

    return result;
}


// Determinant: partial specialisation for rank 1
template<class Cmpt>
inline Cmpt det(const hyperTensor<Cmpt, 1>& t)
{
    return t(0, 0);
}


// Determinant: partial specialisation for rank 2
template<class Cmpt>
inline Cmpt det(const hyperTensor<Cmpt, 2>& t)
{
    return t(0, 0)*t(1, 1) - t(0, 1)*t(1, 0);
}


//- Return the inverse of a tensor give the determinant
//  Uses Gauss-Jordan Elimination with full pivoting
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length> inv(const hyperTensor<Cmpt, length>& t)
{
    hyperTensor<Cmpt, length> result(t);

    label iRow = 0, iCol = 0;
    Cmpt largestCoeff, temp;
    Cmpt* __restrict__ srcIter;
    Cmpt* __restrict__ destIter;

    // Lists used for bookkeeping on the pivoting
    List<label> indexCol(length), indexRow(length), iPivot(length);

    iPivot = 0;

    label curRowOffset;

    // Main loop over columns to be reduced
    for (label i = 0; i < length; i++)
    {
        largestCoeff = pTraits<Cmpt>::zero;

        // Search for pivot element
        curRowOffset = 0;

        for (label j = 0; j < length; j++)
        {
            if (iPivot[j] != 1)
            {
                for (int k = 0; k < length; k++)
                {
                    if (iPivot[k] == 0)
                    {
                        if
                        (
                            (temp = CML::mag(result[curRowOffset+k]))
                         >= largestCoeff
                        )
                        {
                            largestCoeff = temp;
                            iRow = j;
                            iCol = k;
                        }
                    }
                }
            }
            curRowOffset += length;
        }
        ++(iPivot[iCol]);

        // We now have the pivot element
        // Interchange rows if needed
        if (iRow != iCol)
        {
            srcIter = &result(iRow, 0);
            destIter = &result(iCol, 0);

            for (label j = 0; j < length; j++)
            {
                Swap((*srcIter), (*destIter));
                srcIter++;
                destIter++;
            }
        }
        indexRow[i] = iRow;
        indexCol[i] = iCol;

        //Check for singularity
        srcIter = &result(iCol, iCol);  // Dummy pointer to reduce indexing
        if ((*srcIter) == Cmpt(0))
        {
            FatalErrorInFunction
                << "Singular tensor" << length << CML::abort(FatalError);
        }

        // Divide the pivot row by pivot element
        temp = pTraits<Cmpt>::one/(*srcIter);
        (*srcIter) = pTraits<Cmpt>::one;

        srcIter = &result(iCol, 0);
        for (label j = 0; j < length; j++)
        {
            (*srcIter) *= temp;
            srcIter++;
        }

        // Reduce the rows, excluding the pivot row
        for (label j = 0; j < length; j++)
        {
            if (j != iCol)
            {
                destIter = &result(j, 0);
                srcIter = &result(iCol, 0);

                temp=destIter[iCol];
                destIter[iCol] = pTraits<Cmpt>::zero;

                for (label k = 0; k < length; k++)
                {
                    (*destIter) -= (*srcIter)*temp;
                    srcIter++;
                    destIter++;
                }
            }
        }
    }

    // Unscamble the solution
    for (label i = length - 1; i >= 0; i--)
    {
        if (indexRow[i] != indexCol[i])
        {
            srcIter = &result[indexRow[i]];
            destIter = &result[indexCol[i]];
            for (label j = 0; j < length; j++)
            {
                Swap((*srcIter), (*destIter));
                srcIter += length;
                destIter += length;
            }
        }
    }

    return result;
}


//- Inverse: partial template specialisation for rank 1
template <class Cmpt>
inline hyperTensor<Cmpt, 1> inv(const hyperTensor<Cmpt, 1>& t)
{
    return hyperTensor<Cmpt, 1>(1/t(0, 0));
}


//- Inverse: partial template specialisation for rank 2
template <class Cmpt>
inline hyperTensor<Cmpt, 2> inv(const hyperTensor<Cmpt, 2>& t)
{
    hyperTensor<Cmpt, 2> result(t);

    Cmpt oneOverDet = 1/(t(0, 0)*t(1, 1) - t(0, 1)*t(1, 0));

    result(0, 0) = oneOverDet*t(1, 1);
    result(0, 1) = -oneOverDet*t(0, 1);
    result(1, 0) = -oneOverDet*t(1, 0);
    result(1, 1) = oneOverDet*t(0, 0);

    return result;
}


//- Return tensor diagonal
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length> negSumDiag(const hyperTensor<Cmpt, length>& t)
{
    return t.negSumDiag();
}


//- Scale tensor row with a hyperVector
template <class Cmpt, int length>
inline hyperTensor<Cmpt, length> scaleRow
(
    const hyperTensor<Cmpt, length>& t,
    const hyperVector<Cmpt, length>& v
)
{
    hyperTensor<Cmpt, length> result;

    for (label i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        for (label j = 0; j < hyperTensor<Cmpt, length>::rowLength; j++)
        {
            result(i, j) = t(i, j)*v(j);
        }
    }

    return result;
}

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Cmpt, int length>
class outerProduct<hyperTensor<Cmpt, length>, Cmpt>
{
public:

    typedef hyperTensor<Cmpt, length> type;
};


template<class Cmpt, int length>
class outerProduct<Cmpt, hyperTensor<Cmpt, length> >
{
public:

    typedef hyperTensor<Cmpt, length> type;
};


template<class Cmpt, int length>
class innerProduct<hyperVector<Cmpt, length>, hyperTensor<Cmpt, length> >
{
public:

    typedef hyperVector<Cmpt, length> type;
};


template<class Cmpt, int length>
class innerProduct<hyperTensor<Cmpt, length>, hyperVector<Cmpt, length> >
{
public:

    typedef hyperVector<Cmpt, length> type;
};


template<class Cmpt, int length>
class innerProduct<hyperTensor<Cmpt, length>, hyperTensor<Cmpt, length> >
{
public:

    typedef hyperTensor<Cmpt, length> type;
};


} // End namespace CML
#endif
