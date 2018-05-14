/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2016 H. Jasak
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
    hypderVector

Description
    Templated hyperVector derived from VectorSpace.
    
    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.
    
    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.
    
Author
    Hrvoje Jasak, Wikki Ltd.

\*---------------------------------------------------------------------------*/

#ifndef hyperVector_HPP
#define hyperVector_HPP

#include "VectorSpace.hpp"
#include "tensor.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class hyperVector Declaration
\*---------------------------------------------------------------------------*/

template <class Cmpt, int length>
class hyperVector
:
    public VectorSpace<hyperVector<Cmpt, length>, Cmpt, length>
{

public:

    // Member constants
    enum
    {
        rank = 1 // Rank of hyperVector is 1
    };

    // Static data members
    static const char* const typeName;
    static const hyperVector zero;
    static const hyperVector one;
    static const hyperVector max;
    static const hyperVector min;

    //- Construct null
    inline hyperVector();

    //- Construct given VectorSpace
    inline hyperVector
    (
        const VectorSpace<hyperVector<Cmpt, length>, Cmpt, length>&
    );

    //- Construct given component value.  Special use only!
    explicit inline hyperVector(const Cmpt& vx);

    //- Construct from Istream
    inline hyperVector(Istream&);

    //- Return ith component
    inline const Cmpt& operator()(const direction i) const;

    //- Return access to ith component
    inline Cmpt& operator()(const direction i);

    //- Component-wise multiply
    inline hyperVector<Cmpt, length> cmptMultiply
    (
        const hyperVector<Cmpt, length>&
    );
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Cmpt, int length>
class hyperTensor;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template <class Cmpt, int length>
const char* const hyperVector<Cmpt, length>::typeName =
    ("vector" + name(length)).c_str();


template <class Cmpt, int length>
const hyperVector<Cmpt, length> hyperVector<Cmpt, length>::zero(0);


template <class Cmpt, int length>
const hyperVector<Cmpt, length> hyperVector<Cmpt, length>::one(1);


template <class Cmpt, int length>
const hyperVector<Cmpt, length> hyperVector<Cmpt, length>::max(VGREAT);


template <class Cmpt, int length>
const hyperVector<Cmpt, length> hyperVector<Cmpt, length>::min(-VGREAT);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct null
template <class Cmpt, int length>
inline hyperVector<Cmpt, length>::hyperVector()
{}


// Construct given VectorSpace
template <class Cmpt, int length>
inline hyperVector<Cmpt, length>::hyperVector
(
    const VectorSpace<hyperVector<Cmpt, length>, Cmpt, length>& vs
)
:
    VectorSpace<hyperVector<Cmpt, length>, Cmpt, length>(vs)
{}


// Construct given Cmpts
template <class Cmpt, int length>
inline hyperVector<Cmpt, length>::hyperVector(const Cmpt& vx)
{
    VectorSpaceOps<hyperVector<Cmpt, length>::nComponents, 0>::eqOpS
        (*this, vx, eqOp<Cmpt>());
}


// Construct from Istream
template <class Cmpt, int length>
inline hyperVector<Cmpt, length>::hyperVector(Istream& is)
:
    VectorSpace<hyperVector<Cmpt, length>, Cmpt, length>(is)
{}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template <class Cmpt, int length>
inline const Cmpt& hyperVector<Cmpt, length>::operator()
(
    const direction i
) const
{
    return this->operator[](i);
}


template <class Cmpt, int length>
inline Cmpt& hyperVector<Cmpt, length>::operator()(const direction i)
{
    return this->operator[](i);
}


//- Multiply components of hyperVector by hyperVector
template<class Cmpt, int length>
inline hyperVector<Cmpt, length>
hyperVector<Cmpt, length>::cmptMultiply(const hyperVector<Cmpt, length>& v)
{
    hyperVector<Cmpt, length> res;
    VectorSpaceOps<hyperVector<Cmpt, length>::nComponents, 0>::op
        (res, *this, v, multiplyOp<Cmpt>());
    return res;
}


//- Transform the spherical tensor
//- The components are assumed to be individual scalars
//- i.e. transform has no effect
template<class Cmpt, int length>
inline hyperVector<Cmpt, length> transform
(
    const tensor& tt,
    const hyperVector<Cmpt, length>& v
)
{
    return v;
}

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Cmpt, int length>
class outerProduct<Cmpt, hyperVector<Cmpt, length> >
{
public:

    typedef hyperVector<Cmpt, length> type;
};


template<class Cmpt, int length>
class outerProduct<hyperVector<Cmpt, length>, Cmpt>
{
public:

    typedef hyperVector<Cmpt, length> type;
};


template<class Cmpt, int length>
class outerProduct<hyperVector<Cmpt, length>, hyperVector<Cmpt, length> >
{
public:

    typedef hyperTensor<Cmpt, length> type;
};


//- Inner-product between a vector and a vector
template <class Cmpt, int length>
inline Cmpt operator&
(
    const hyperVector<Cmpt, length>& v1,
    const hyperVector<Cmpt, length>& v2
)
{
    Cmpt res = pTraits<Cmpt>::zero;
    VectorSpaceOps<hyperVector<Cmpt, length>::nComponents, 0>::SopEqOpVV
        (res, v1, v2, plusEqOp<Cmpt>(), multiplyOp<Cmpt>());
    return res;
}


//- Componentwise division of scalar by hyperVector
template<class Cmpt, int length>
inline hyperVector<Cmpt, length>
operator/(const scalar s, const hyperVector<Cmpt, length>& v)
{
    hyperVector<Cmpt, length> res;
    VectorSpaceOps<hyperVector<Cmpt, length>::nComponents, 0>::opSV
        (res, s, v, divideOp3<Cmpt, scalar, Cmpt>());
    return res;
}


//- Multiply components of hyperVector by hyperVector
template<class Cmpt, int length>
inline hyperVector<Cmpt, length>
cmptMultiply
(
    const hyperVector<Cmpt, length>& v1,
    const hyperVector<Cmpt, length>& v2
)
{
    hyperVector<Cmpt, length> res;
    VectorSpaceOps<hyperVector<Cmpt, length>::nComponents, 0>::op
        (res, v1, v2, multiplyOp<Cmpt>());
    return res;
}


//- Multiply row of hyperVector with a scalar
template<class Cmpt, int length>
inline hyperVector<Cmpt, length>
scaleRow(const hyperVector<Cmpt, length>& v, const Cmpt& c)
{
    return v*c;
}


} // End namespace CML
#endif
