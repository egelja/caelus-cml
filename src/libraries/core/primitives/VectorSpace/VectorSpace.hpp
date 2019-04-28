/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::VectorSpace

Description
    Templated vector space.

    Template arguments are the Form the vector space will be used to create,
    the type of the elements and the number of elements.

SourceFiles
    VectorSpaceI.hpp
    VectorSpace.cpp

\*---------------------------------------------------------------------------*/

#ifndef VectorSpace_H
#define VectorSpace_H

#include "direction.hpp"
#include "scalar.hpp"
#include "word.hpp"
#include "zero.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Form, class Cmpt, direction Ncmpts> class VectorSpace;

template<class Form, class Cmpt, direction Ncmpts>
Istream& operator>>
(
    Istream&,
    VectorSpace<Form, Cmpt, Ncmpts>&
);

template<class Form, class Cmpt, direction Ncmpts>
Ostream& operator<<
(
    Ostream&,
    const VectorSpace<Form, Cmpt, Ncmpts>&
);


/*---------------------------------------------------------------------------*\
                           Class VectorSpace Declaration
\*---------------------------------------------------------------------------*/

template<class Form, class Cmpt, direction Ncmpts>
class VectorSpace
{

public:

    //- The components of this vector space
    Cmpt v_[Ncmpts];

    //- VectorSpace type
    typedef VectorSpace<Form, Cmpt, Ncmpts> vsType;

    //- Component type
    typedef Cmpt cmptType;


    // Static constants

        //- Dimensionality of space
        static const direction dim = 3;

        //- Number of components in this vector space
        static const direction nComponents = Ncmpts;


        // VectorSpace currently defaults to a column-vector
        // This will be removed when column-vector is introduced
        // as a specialization
        static const direction mRows = Ncmpts;
        static const direction nCols = 1;


    // Static data members

        static const char* const typeName;
        static const char* const componentNames[];
        static const Form zero;
        static const Form one;
        static const Form max;
        static const Form min;
        static const Form rootMax;
        static const Form rootMin;


    // Sub-Block Classes

        //- Const sub-block type
        template
        <
            class SubVector,
            direction BStart
        >
        class ConstBlock
        {
            const vsType& vs_;

        public:

            //- Number of components in this vector space
            static const direction nComponents = SubVector::nComponents;

            //- Construct for a given vector
            inline ConstBlock(const vsType& vs);

            //- [i] const element access operator
            inline const Cmpt& operator[](const direction i) const;

            //- (i, 0) const element access operator
            inline const Cmpt& operator()
            (
                const direction i,
                const direction
            ) const;
        };


    // Constructors

        //- Construct null
        inline VectorSpace();

        //- Construct initialized to zero
        inline VectorSpace(const CML::zero);

        //- Construct from Istream
        VectorSpace(Istream&);

        //- Construct as copy of a VectorSpace with the same size
        template<class Form2, class Cmpt2>
        inline explicit VectorSpace(const VectorSpace<Form2, Cmpt2, Ncmpts>&);


    // Member Functions

        //- Return the number of elements in the VectorSpace = Ncmpts.
        inline static direction size();

        inline const Cmpt& component(const direction) const;
        inline Cmpt& component(const direction);

        inline void component(Cmpt&, const direction) const;
        inline void replace(const direction, const Cmpt&);

        //- Return a VectorSpace with all elements = s
        inline static Form uniform(const Cmpt& s);

        template<class SubVector, direction BStart>
        inline const ConstBlock<SubVector, BStart> block() const;


    // Member Operators

        inline const Cmpt& operator[](const direction) const;
        inline Cmpt& operator[](const direction);

        inline void operator+=(const VectorSpace<Form, Cmpt, Ncmpts>&);
        inline void operator-=(const VectorSpace<Form, Cmpt, Ncmpts>&);

        inline void operator=(const CML::zero);
        inline void operator*=(const scalar);
        inline void operator/=(const scalar);


    // IOstream Operators

        friend Istream& operator>> <Form, Cmpt, Ncmpts>
        (
            Istream&,
            VectorSpace<Form, Cmpt, Ncmpts>&
        );

        friend Ostream& operator<< <Form, Cmpt, Ncmpts>
        (
            Ostream&,
            const VectorSpace<Form, Cmpt, Ncmpts>&
        );
};


// * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * * //

//- Return a string representation of a VectorSpace
template<class Form, class Cmpt, direction Ncmpts>
word name(const VectorSpace<Form, Cmpt, Ncmpts>&);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"
#include "products.hpp"
#include "VectorSpaceM.hpp"
#include "ops.hpp"
#include "StaticAssert.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Form, class Cmpt, direction Ncmpts>
inline VectorSpace<Form, Cmpt, Ncmpts>::VectorSpace()
{}


template<class Form, class Cmpt, direction Ncmpts>
inline VectorSpace<Form, Cmpt, Ncmpts>::VectorSpace(const CML::zero)
{
    VectorSpaceOps<Ncmpts,0>::eqOpS(*this, Zero, eqOp<Cmpt>());
}


template<class Form, class Cmpt, direction Ncmpts>
template<class Form2, class Cmpt2>
inline VectorSpace<Form, Cmpt, Ncmpts>::VectorSpace
(
    const VectorSpace<Form2, Cmpt2, Ncmpts>& vs
)
{
    VectorSpaceOps<Ncmpts,0>::eqOp(*this, vs, eqOp<Cmpt>());
}


template<class Form, class Cmpt, direction Ncmpts>
template<class SubVector, direction BStart>
inline
VectorSpace<Form, Cmpt, Ncmpts>::ConstBlock<SubVector, BStart>::ConstBlock
(
    const vsType& vs
)
:
    vs_(vs)
{
    StaticAssert(vsType::nComponents >= BStart + nComponents);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Form, class Cmpt, direction Ncmpts>
inline direction VectorSpace<Form, Cmpt, Ncmpts>::size()
{
    return Ncmpts;
}


template<class Form, class Cmpt, direction Ncmpts>
inline const Cmpt& VectorSpace<Form, Cmpt, Ncmpts>::component
(
    const direction d
) const
{
    #ifdef FULLDEBUG
    if (d >= Ncmpts)
    {
        FatalErrorInFunction
            << "index out of range"
            << abort(FatalError);
    }
    #endif

    return v_[d];
}


template<class Form, class Cmpt, direction Ncmpts>
inline Cmpt& VectorSpace<Form, Cmpt, Ncmpts>::component
(
    const direction d
)
{
    #ifdef FULLDEBUG
    if (d >= Ncmpts)
    {
        FatalErrorInFunction
            << "index out of range"
            << abort(FatalError);
    }
    #endif

    return v_[d];
}


template<class Form, class Cmpt, direction Ncmpts>
inline void VectorSpace<Form, Cmpt, Ncmpts>::component
(
    Cmpt& c,
    const direction d
) const
{
    #ifdef FULLDEBUG
    if (d >= Ncmpts)
    {
        FatalErrorInFunction
            << "index out of range"
            << abort(FatalError);
    }
    #endif

    c = v_[d];
}


template<class Form, class Cmpt, direction Ncmpts>
inline void VectorSpace<Form, Cmpt, Ncmpts>::replace
(
    const direction d,
    const Cmpt& c
)
{
    #ifdef FULLDEBUG
    if (d >= Ncmpts)
    {
        FatalErrorInFunction
            << "index out of range"
            << abort(FatalError);
    }
    #endif

    v_[d] = c;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form VectorSpace<Form, Cmpt, Ncmpts>::uniform(const Cmpt& s)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::eqOpS(v, s, eqOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
template<class SubVector, direction BStart>
inline const typename VectorSpace<Form, Cmpt, Ncmpts>::template
    ConstBlock<SubVector, BStart>
VectorSpace<Form, Cmpt, Ncmpts>::block() const
{
    return *this;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Form, class Cmpt, direction Ncmpts>
inline const Cmpt& VectorSpace<Form, Cmpt, Ncmpts>::operator[]
(
    const direction d
) const
{
    #ifdef FULLDEBUG
    if (d >= Ncmpts)
    {
        FatalErrorInFunction
            << "index out of range"
            << abort(FatalError);
    }
    #endif

    return v_[d];
}


template<class Form, class Cmpt, direction Ncmpts>
inline Cmpt& VectorSpace<Form, Cmpt, Ncmpts>::operator[]
(
    const direction d
)
{
    #ifdef FULLDEBUG
    if (d >= Ncmpts)
    {
        FatalErrorInFunction
            << "index out of range"
            << abort(FatalError);
    }
    #endif

    return v_[d];
}


template<class Form, class Cmpt, direction Ncmpts>
template<class SubVector, direction BStart>
inline const Cmpt&
VectorSpace<Form, Cmpt, Ncmpts>::
ConstBlock<SubVector, BStart>::operator[]
(
    const direction i
) const
{
    #ifdef FULLDEBUG
    if (i >= Ncmpts)
    {
        FatalErrorInFunction
            << "index out of range"
            << abort(FatalError);
    }
    #endif

    return vs_[BStart + i];
}


template<class Form, class Cmpt, direction Ncmpts>
template<class SubVector, direction BStart>
inline const Cmpt&
VectorSpace<Form, Cmpt, Ncmpts>::
ConstBlock<SubVector, BStart>::operator()
(
    const direction i,
    const direction j
) const
{
    #ifdef FULLDEBUG
    if (i >= Ncmpts)
    {
        FatalErrorInFunction
            << "index out of range"
            << abort(FatalError);
    }

    if (j != 0)
    {
        FatalErrorInFunction
            << "j != 0"
            << abort(FatalError);
    }
    #endif

    return vs_[BStart + i];
}


template<class Form, class Cmpt, direction Ncmpts>
inline void VectorSpace<Form, Cmpt, Ncmpts>::operator+=
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    VectorSpaceOps<Ncmpts,0>::eqOp(*this, vs, plusEqOp<Cmpt>());
}


template<class Form, class Cmpt, direction Ncmpts>
inline void VectorSpace<Form, Cmpt, Ncmpts>::operator-=
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    VectorSpaceOps<Ncmpts,0>::eqOp(*this, vs, minusEqOp<Cmpt>());
}


template<class Form, class Cmpt, direction Ncmpts>
inline void VectorSpace<Form, Cmpt, Ncmpts>::operator=(const CML::zero)
{
    VectorSpaceOps<Ncmpts,0>::eqOpS(*this, 0, eqOp<Cmpt>());
}


template<class Form, class Cmpt, direction Ncmpts>
inline void VectorSpace<Form, Cmpt, Ncmpts>::operator*=
(
    const scalar s
)
{
    VectorSpaceOps<Ncmpts,0>::eqOpS(*this, s, multiplyEqOp2<Cmpt, scalar>());
}


template<class Form, class Cmpt, direction Ncmpts>
inline void VectorSpace<Form, Cmpt, Ncmpts>::operator/=
(
    const scalar s
)
{
    VectorSpaceOps<Ncmpts,0>::eqOpS(*this, s, divideEqOp2<Cmpt, scalar>());
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Form, class Cmpt, direction Ncmpts>
inline Cmpt& setComponent
(
    VectorSpace<Form, Cmpt, Ncmpts>& vs,
    const direction d
)
{
    return vs.component(d);
}


template<class Form, class Cmpt, direction Ncmpts>
inline const Cmpt& component
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs,
    const direction d
)
{
    return vs.component(d);
}


// Powers of a Form
// Equivalent to outer-products between the Form and itself
// Form^0 = 1.0
template<class Form, class Cmpt, direction Ncmpts>
inline typename powProduct<Form, 0>::type pow
(
    const VectorSpace<Form, Cmpt, Ncmpts>&,
    typename powProduct<Form, 0>::type
    = pTraits<typename powProduct<Form, 0>::type>::zero
)
{
    return 1.0;
}

// Form^1 = Form
template<class Form, class Cmpt, direction Ncmpts>
inline typename powProduct<Form, 1>::type pow
(
    const VectorSpace<Form, Cmpt, Ncmpts>& v,
    typename powProduct<Form, 1>::type
  = pTraits<typename powProduct<Form, 1>::type>::zero
)
{
    return static_cast<const Form&>(v);
}


// Form^2 = sqr(Form)
template<class Form, class Cmpt, direction Ncmpts>
inline typename powProduct<Form, 2>::type pow
(
    const VectorSpace<Form, Cmpt, Ncmpts>& v,
    typename powProduct<Form, 2>::type
  = pTraits<typename powProduct<Form, 2>::type>::zero
)
{
    return sqr(static_cast<const Form&>(v));
}


template<class Form, class Cmpt, direction Ncmpts>
inline scalar magSqr
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    scalar ms = magSqr(vs.v_[0]);
    VectorSpaceOps<Ncmpts,1>::SeqOp(ms, vs, plusEqMagSqrOp2<scalar, Cmpt>());
    return ms;
}


template<class Form, class Cmpt, direction Ncmpts>
inline scalar mag
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    return ::sqrt(magSqr(static_cast<const Form&>(vs)));
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form normalised
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    const scalar magVs = mag(vs);
    return magVs > 0 ? vs/magVs : Zero;
}


template<class Form, class Cmpt, direction Ncmpts>
inline VectorSpace<Form, Cmpt, Ncmpts> cmptMultiply
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::op(v, vs1, vs2, cmptMultiplyOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline VectorSpace<Form, Cmpt, Ncmpts> cmptPow
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::op(v, vs1, vs2, cmptPowOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline VectorSpace<Form, Cmpt, Ncmpts> cmptDivide
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::op(v, vs1, vs2, cmptDivideOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline VectorSpace<Form, Cmpt, Ncmpts> stabilise
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs,
    const Cmpt& small
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::opVS(v, vs, small, stabiliseOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Cmpt cmptMax
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    Cmpt cMax = vs.v_[0];
    VectorSpaceOps<Ncmpts,1>::SeqOp(cMax, vs, maxEqOp<Cmpt>());
    return cMax;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Cmpt cmptMin
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    Cmpt cMin = vs.v_[0];
    VectorSpaceOps<Ncmpts,1>::SeqOp(cMin, vs, minEqOp<Cmpt>());
    return cMin;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Cmpt cmptSum
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    Cmpt sum = vs.v_[0];
    VectorSpaceOps<Ncmpts,1>::SeqOp(sum, vs, plusEqOp<Cmpt>());
    return sum;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Cmpt cmptAv
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    return cmptSum(vs)/Ncmpts;
}

template<class Form, class Cmpt, direction Ncmpts>
inline Cmpt cmptProduct
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    Cmpt product = vs.v_[0];
    VectorSpaceOps<Ncmpts,1>::SeqOp(product, vs, multiplyEqOp<Cmpt>());
    return product;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form cmptSqr
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::eqOp(v, vs, eqSqrOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form cmptMag
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::eqOp(v, vs, eqMagOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form max
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::op(v, vs1, vs2, maxOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form min
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::op(v, vs1, vs2, minOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form minMod
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::op(v, vs1, vs2, minModOp<Cmpt>());
    return v;
}


template<class Type>
inline Type dot(const scalar s, const Type& t)
{
    return s * t;
}


template<class Type>
inline Type dot(const Type& t, const scalar s)
{
    return t * s;
}


template
<
    class Form1, class Cmpt1, direction Ncmpts1,
    class Form2, class Cmpt2, direction Ncmpts2
>
inline typename innerProduct<Form1, Form2>::type dot
(
    const VectorSpace<Form1, Cmpt1, Ncmpts1>& t1,
    const VectorSpace<Form2, Cmpt2, Ncmpts2>& t2
)
{
    return static_cast<const Form1&>(t1) & static_cast<const Form2&>(t2);
}


template<class Form, class Cmpt, direction Ncmpts>
inline direction findMax(const VectorSpace<Form, Cmpt, Ncmpts>& vs)
{
    direction index = 0;
    for (direction i=1; i<Ncmpts; ++i)
    {
        index = vs[index] > vs[i] ? index : i;
    }
    return index;
}


template<class Form, class Cmpt, direction Ncmpts>
inline direction findMin(const VectorSpace<Form, Cmpt, Ncmpts>& vs)
{
    direction index = 0;
    for (direction i=1; i<Ncmpts; ++i)
    {
        index = vs[index] < vs[i] ? index : i;
    }
    return index;
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Form, class Cmpt, direction Ncmpts>
inline Form operator-
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::eqOp(v, vs, eqMinusOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form operator+
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::op(v, vs1, vs2, plusOp<Cmpt>());
    return v;
}

template<class Form, class Cmpt, direction Ncmpts>
inline Form operator-
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::op(v, vs1, vs2, minusOp<Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form operator*
(
    scalar s,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::opSV(v, s, vs, multiplyOp3<Cmpt, scalar, Cmpt>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form operator*
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs,
    scalar s
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::opVS(v, vs, s, multiplyOp3<Cmpt, Cmpt, scalar>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Form operator/
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs,
    scalar s
)
{
    Form v;
    VectorSpaceOps<Ncmpts,0>::opVS(v, vs, s, divideOp3<Cmpt, Cmpt, scalar>());
    return v;
}


template<class Form, class Cmpt, direction Ncmpts>
inline Cmpt operator&&
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    Cmpt ddProd = vs1.v_[0]*vs2.v_[0];
    for (direction i=1; i<Ncmpts; ++i)
    {
        ddProd += vs1.v_[i]*vs2.v_[i];
    }
    return ddProd;
}


template<class Form, class Cmpt, direction Ncmpts>
inline bool operator==
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    bool eq = true;
    for (direction i=0; i<Ncmpts; ++i)
    {
        if (!(eq &= (equal(vs1.v_[i], vs2.v_[i])))) break;
    }
    return eq;
}


template<class Form, class Cmpt, direction Ncmpts>
inline bool operator!=
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    return !(vs1 == vs2);
}


template<class Form, class Cmpt, direction Ncmpts>
inline bool operator>
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    bool gt = true;
    for (direction i=0; i<Ncmpts; ++i)
    {
        if (!(gt &= vs1.v_[i] > vs2.v_[i])) break;
    }
    return gt;
}


template<class Form, class Cmpt, direction Ncmpts>
inline bool operator<
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    bool lt = true;
    for (direction i=0; i<Ncmpts; ++i)
    {
        if (!(lt &= vs1.v_[i] < vs2.v_[i])) break;
    }
    return lt;
}


template<class Form, class Cmpt, direction Ncmpts>
inline bool operator>=
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    return !(vs1 < vs2);
}


template<class Form, class Cmpt, direction Ncmpts>
inline bool operator<=
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs1,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs2
)
{
    return !(vs1 > vs2);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#include "IOstreams.hpp"

#include <sstream>

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Form, class Cmpt, CML::direction Ncmpts>
CML::VectorSpace<Form, Cmpt, Ncmpts>::VectorSpace
(
    Istream& is
)
{
    // Read beginning of VectorSpace<Cmpt>
    is.readBegin("VectorSpace<Form, Cmpt, Ncmpts>");

    for (direction i=0; i<Ncmpts; i++)
    {
        is >> v_[i];
    }

    // Read end of VectorSpace<Cmpt>
    is.readEnd("VectorSpace<Form, Cmpt, Ncmpts>");

    // Check state of Istream
    is.check("VectorSpace<Form, Cmpt, Ncmpts>::VectorSpace(Istream&)");
}


template<class Form, class Cmpt, CML::direction Ncmpts>
CML::word CML::name
(
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    std::ostringstream buf;

    buf << '(' << vs.v_[0];

    for (direction i=1; i<Ncmpts; i++)
    {
        buf << ',' << vs.v_[i];
    }

    buf << ')';

    return buf.str();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Form, class Cmpt, CML::direction Ncmpts>
CML::Istream& CML::operator>>
(
    Istream& is,
    VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    // Read beginning of VectorSpace<Cmpt, Ncmpts>
    is.readBegin("VectorSpace<Form, Cmpt, Ncmpts>");

    for (direction i=0; i<Ncmpts; i++)
    {
        is >> vs.v_[i];
    }

    // Read end of VectorSpace<Cmpt, Ncmpts>
    is.readEnd("VectorSpace<Form, Cmpt, Ncmpts>");

    // Check state of Istream
    is.check("operator>>(Istream&, VectorSpace<Form, Cmpt, Ncmpts>&)");

    return is;
}


template<class Form, class Cmpt, CML::direction Ncmpts>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const VectorSpace<Form, Cmpt, Ncmpts>& vs
)
{
    os << token::BEGIN_LIST << vs.v_[0];

    for (direction i=1; i<Ncmpts; i++)
    {
        os << token::SPACE << vs.v_[i];
    }

    os << token::END_LIST;

    // Check state of Ostream
    os.check("operator<<(Ostream&, const VectorSpace<Form, Cmpt, Ncmpts>&)");

    return os;
}

#endif

// ************************************************************************* //
