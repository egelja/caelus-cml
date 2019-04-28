/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::FieldField

Description
    Generic field type.

SourceFiles
    FieldField.cpp

\*---------------------------------------------------------------------------*/

#ifndef FieldField_H
#define FieldField_H

#include "tmp.hpp"
#include "PtrList.hpp"
#include "scalar.hpp"
#include "direction.hpp"
#include "VectorSpace.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<template<class> class Field, class Type>
class FieldField;

template<template<class> class Field, class Type>
Ostream& operator<<
(
    Ostream&,
    const FieldField<Field, Type>&
);

template<template<class> class Field, class Type>
Ostream& operator<<
(
    Ostream&,
    const tmp<FieldField<Field, Type> >&
);


/*---------------------------------------------------------------------------*\
                           Class FieldField Declaration
\*---------------------------------------------------------------------------*/

template<template<class> class Field, class Type>
class FieldField
:
    public refCount,
    public PtrList<Field<Type> >
{

public:

    //- Component type
    typedef typename pTraits<Type>::cmptType cmptType;


    // Constructors

        //- Construct null
        //  Used for temporary fields which are initialised after construction
        FieldField();

        //- Construct given size
        //  Used for temporary fields which are initialised after construction
        explicit FieldField(const label);

        //- Construct using the Field sizes from the given FieldField
        //  and the given Field type.
        //  Used for temporary fields which are initialised after construction
        FieldField(const word&, const FieldField<Field, Type>&);

        //- Construct as copy
        FieldField(const FieldField<Field, Type>&);

        //- Construct as copy or re-use as specified.
        FieldField(FieldField<Field, Type>&, bool reUse);

        //- Construct as copy of a PtrList<Field, Type>
        FieldField(const PtrList<Field<Type> >&);

        //- Construct as copy of tmp<FieldField>
#       ifdef ConstructFromTmp
        FieldField(const tmp<FieldField<Field, Type> >&);
#       endif

        //- Construct from Istream
        FieldField(Istream&);

        //- Clone
        tmp<FieldField<Field, Type> > clone() const;

        //- Return a pointer to a new calculatedFvPatchFieldField created on
        //  freestore without setting patchField values
        template<class Type2>
        static tmp<FieldField<Field, Type> > NewCalculatedType
        (
            const FieldField<Field, Type2>& ff
        )

#       ifdef __INTEL_COMPILER
        {
            FieldField<Field, Type>* nffPtr
            (
                new FieldField<Field, Type>(ff.size())
            );

            forAll(*nffPtr, i)
            {
                nffPtr->set(i, Field<Type>::NewCalculatedType(ff[i]).ptr());
            }

            return tmp<FieldField<Field, Type> >(nffPtr);
        }
#       else
        ;
#       endif

    // Member functions

        //- Negate this field
        void negate();

        //- Return a component field of the field
        tmp<FieldField<Field, cmptType> > component(const direction) const;

        //- Replace a component field of the field
        void replace(const direction, const FieldField<Field, cmptType>&);

        //- Replace a component field of the field
        void replace(const direction, const cmptType&);

        //- Return the field transpose (only defined for second rank tensors)
        tmp<FieldField<Field, Type> > T() const;


    // Member operators

        void operator=(const FieldField<Field, Type>&);
        void operator=(const tmp<FieldField<Field, Type> >&);
        void operator=(const Type&);

        void operator+=(const FieldField<Field, Type>&);
        void operator+=(const tmp<FieldField<Field, Type> >&);

        void operator-=(const FieldField<Field, Type>&);
        void operator-=(const tmp<FieldField<Field, Type> >&);

        void operator*=(const FieldField<Field, scalar>&);
        void operator*=(const tmp<FieldField<Field, scalar> >&);

        void operator/=(const FieldField<Field, scalar>&);
        void operator/=(const tmp<FieldField<Field, scalar> >&);

        void operator+=(const Type&);
        void operator-=(const Type&);

        void operator*=(const scalar&);
        void operator/=(const scalar&);


    // IOstream operators

        friend Ostream& operator<< <Field, Type>
        (
            Ostream&,
            const FieldField<Field, Type>&
        );

        friend Ostream& operator<< <Field, Type>
        (
            Ostream&,
            const tmp<FieldField<Field, Type> >&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "FieldFieldFunctions.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

#ifdef FULLDEBUG

template<template<class> class Field, class Type1, class Type2>
void checkFields
(
    const FieldField<Field, Type1>& f1,
    const FieldField<Field, Type2>& f2,
    const char* op
)
{
    if (f1.size() != f2.size())
    {
        FatalErrorInFunction
            << "    incompatible fields"
            << " FieldField<" << pTraits<Type1>::typeName
            << "> f1(" << f1.size() << ')'
            << " and FieldField<" << pTraits<Type2>::typeName
            << "> f2(" << f2.size() << ')'
            << endl << " for operation " << op
            << abort(FatalError);
    }
}

template<template<class> class Field, class Type1, class Type2, class Type3>
void checkFields
(
    const FieldField<Field, Type1>& f1,
    const FieldField<Field, Type2>& f2,
    const FieldField<Field, Type3>& f3,
    const char* op
)
{
    if (f1.size() != f2.size() || f1.size() != f3.size())
    {
        FatalErrorInFunction
            << "    incompatible fields"
            << " FieldField<" << pTraits<Type1>::typeName
            << "> f1(" << f1.size() << ')'
            << ", FieldField<" <<pTraits<Type2>::typeName
            << "> f2(" << f2.size() << ')'
            << " and FieldField<"<<pTraits<Type3>::typeName
            << "> f3("<<f3.size() << ')'
            << endl << "    for operation " << op
            << abort(FatalError);
    }
}

#else

template<template<class> class Field, class Type1, class Type2>
void checkFields
(
    const FieldField<Field, Type1>&,
    const FieldField<Field, Type2>&,
    const char* op
)
{}

template<template<class> class Field, class Type1, class Type2, class Type3>
void checkFields
(
    const FieldField<Field, Type1>&,
    const FieldField<Field, Type2>&,
    const FieldField<Field, Type3>&,
    const char* op
)
{}

#endif


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<template<class> class Field, class Type>
FieldField<Field, Type>::FieldField()
:
    PtrList<Field<Type> >()
{}


template<template<class> class Field, class Type>
FieldField<Field, Type>::FieldField(const label size)
:
    PtrList<Field<Type> >(size)
{}


template<template<class> class Field, class Type>
FieldField<Field, Type>::FieldField
(
    const word& type,
    const FieldField<Field, Type>& ff
)
:
    PtrList<Field<Type> >(ff.size())
{
    forAll(*this, i)
    {
        set(i, Field<Type>::New(type, ff[i]));
    }
}


template<template<class> class Field, class Type>
FieldField<Field, Type>::FieldField(const FieldField<Field, Type>& f)
:
    refCount(),
    PtrList<Field<Type> >(f)
{}


template<template<class> class Field, class Type>
FieldField<Field, Type>::FieldField(FieldField<Field, Type>& f, bool reUse)
:
    refCount(),
    PtrList<Field<Type> >(f, reUse)
{}


template<template<class> class Field, class Type>
FieldField<Field, Type>::FieldField(const PtrList<Field<Type> >& tl)
:
    PtrList<Field<Type> >(tl)
{}


// Construct as copy of tmp<FieldField>
#ifdef ConstructFromTmp
template<template<class> class Field, class Type>
FieldField<Field, Type>::FieldField(const tmp<FieldField<Field, Type> >& tf)
:
    PtrList<Field<Type> >
    (
        const_cast<FieldField<Field, Type>&>(tf()),
        tf.isTmp()
    )
{
    const_cast<FieldField<Field, Type>&>(tf()).resetRefCount();
}
#endif


template<template<class> class Field, class Type>
FieldField<Field, Type>::FieldField(Istream& is)
:
    PtrList<Field<Type> >(is)
{}


template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > FieldField<Field, Type>::clone() const
{
    return tmp<FieldField<Field, Type> >(new FieldField<Field, Type>(*this));
}


#ifndef __INTEL_COMPILER
template<template<class> class Field, class Type>
template<class Type2>
tmp<FieldField<Field, Type> > FieldField<Field, Type>::NewCalculatedType
(
    const FieldField<Field, Type2>& ff
)
{
    FieldField<Field, Type>* nffPtr
    (
        new FieldField<Field, Type>(ff.size())
    );

    forAll(*nffPtr, i)
    {
        nffPtr->set(i, Field<Type>::NewCalculatedType(ff[i]).ptr());
    }

    return tmp<FieldField<Field, Type> >(nffPtr);
}
#endif


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<template<class> class Field, class Type>
void FieldField<Field, Type>::negate()
{
    forAll(*this, i)
    {
        this->operator[](i).negate();
    }
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> >
FieldField<Field, Type>::component
(
    const direction d
) const
{
    tmp<FieldField<Field, cmptType> > Component
    (
        FieldField<Field, typename FieldField<Field, Type>::cmptType>::
            NewCalculatedType(*this)
    );

    ::CML::component(Component(), *this, d);

    return Component;
}


template<template<class> class Field, class Type>
void FieldField<Field, Type>::replace
(
    const direction d,
    const FieldField<Field, cmptType>& sf
)
{
    forAll(*this, i)
    {
        this->operator[](i).replace(d, sf[i]);
    }
}


template<template<class> class Field, class Type>
void FieldField<Field, Type>::replace
(
    const direction d,
    const cmptType& s
)
{
    forAll(*this, i)
    {
        this->operator[](i).replace(d, s);
    }
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > FieldField<Field, Type>::T() const
{
    tmp<FieldField<Field, Type> > transpose
    (
        FieldField<Field, Type>::NewCalculatedType(*this)
    );

    ::CML::T(transpose(), *this);
    return transpose;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<template<class> class Field, class Type>
void FieldField<Field, Type>::operator=(const FieldField<Field, Type>& f)
{
    if (this == &f)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    forAll(*this, i)
    {
        this->operator[](i) = f[i];
    }
}


template<template<class> class Field, class Type>
void FieldField<Field, Type>::operator=(const tmp<FieldField>& tf)
{
    if (this == &(tf()))
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    // This is dodgy stuff, don't try this at home.
    FieldField* fieldPtr = tf.ptr();
    PtrList<Field<Type> >::transfer(*fieldPtr);
    delete fieldPtr;
}


template<template<class> class Field, class Type>
void FieldField<Field, Type>::operator=(const Type& t)
{
    forAll(*this, i)
    {
        this->operator[](i) = t;
    }
}


#define COMPUTED_ASSIGNMENT(TYPE, op)                                         \
                                                                              \
template<template<class> class Field, class Type>                             \
void FieldField<Field, Type>::operator op(const FieldField<Field, TYPE>& f)   \
{                                                                             \
    forAll(*this, i)                                                          \
    {                                                                         \
        this->operator[](i) op f[i];                                          \
    }                                                                         \
}                                                                             \
                                                                              \
template<template<class> class Field, class Type>                             \
void FieldField<Field, Type>::operator op                                     \
(                                                                             \
    const tmp<FieldField<Field, TYPE> >& tf                                   \
)                                                                             \
{                                                                             \
    operator op(tf());                                                        \
    tf.clear();                                                               \
}                                                                             \
                                                                              \
template<template<class> class Field, class Type>                             \
void FieldField<Field, Type>::operator op(const TYPE& t)                      \
{                                                                             \
    forAll(*this, i)                                                          \
    {                                                                         \
        this->operator[](i) op t;                                             \
    }                                                                         \
}

COMPUTED_ASSIGNMENT(Type, +=)
COMPUTED_ASSIGNMENT(Type, -=)
COMPUTED_ASSIGNMENT(scalar, *=)
COMPUTED_ASSIGNMENT(scalar, /=)

#undef COMPUTED_ASSIGNMENT


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<template<class> class Field, class Type>
Ostream& operator<<(Ostream& os, const FieldField<Field, Type>& f)
{
    os << static_cast<const PtrList<Field<Type> >&>(f);
    return os;
}


template<template<class> class Field, class Type>
Ostream& operator<<(Ostream& os, const tmp<FieldField<Field, Type> >& tf)
{
    os << tf();
    tf.clear();
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "PstreamReduceOps.hpp"
#include "FieldFieldReuseFunctions.hpp"

#define TEMPLATE template<template<class> class Field, class Type>
#include "FieldFieldFunctionsM.hxx"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/* * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * */

template<template<class> class Field, class Type>
void component
(
    FieldField<Field, typename FieldField<Field, Type>::cmptType>& sf,
    const FieldField<Field, Type>& f,
    const direction d
)
{
    forAll(sf, i)
    {
        component(sf[i], f[i], d);
    }
}


template<template<class> class Field, class Type>
void T(FieldField<Field, Type>& f1, const FieldField<Field, Type>& f2)
{
    forAll(f1, i)
    {
        T(f1[i], f2[i]);
    }
}


template<template<class> class Field, class Type, direction r>
void pow
(
    FieldField<Field, typename powProduct<Type, r>::type>& f,
    const FieldField<Field, Type>& vf
)
{
    forAll(f, i)
    {
        pow(f[i], vf[i]);
    }
}

template<template<class> class Field, class Type, direction r>
tmp<FieldField<Field, typename powProduct<Type, r>::type> >
pow
(
    const FieldField<Field, Type>& f, typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;
    tmp<FieldField<Field, powProductType> > tRes
    (
        FieldField<Field, powProductType>::NewCalculatedType(f)
    );
    pow<Type, r>(tRes(), f);
    return tRes;
}

template<template<class> class Field, class Type, direction r>
tmp<FieldField<Field, typename powProduct<Type, r>::type> >
pow
(
    const tmp<FieldField<Field, Type> >& tf, typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;
    tmp<FieldField<Field, powProductType> > tRes
    (
        reuseTmpFieldField<Field, powProductType, Type>::New(tf)
    );
    pow<Type, r>(tRes(), tf());
    reuseTmpFieldField<Field, powProductType, Type>::clear(tf);
    return tRes;
}


template<template<class> class Field, class Type>
void sqr
(
    FieldField<Field, typename outerProduct<Type, Type>::type>& f,
    const FieldField<Field, Type>& vf
)
{
    forAll(f, i)
    {
        sqr(f[i], vf[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename outerProduct<Type, Type>::type> >
sqr(const FieldField<Field, Type>& f)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;
    tmp<FieldField<Field, outerProductType> > tRes
    (
        FieldField<Field, outerProductType>::NewCalculatedType(f)
    );
    sqr(tRes(), f);
    return tRes;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename outerProduct<Type, Type>::type> >
sqr(const tmp<FieldField<Field, Type> >& tf)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;
    tmp<FieldField<Field, outerProductType> > tRes
    (
        reuseTmpFieldField<Field, outerProductType, Type>::New(tf)
    );
    sqr(tRes(), tf());
    reuseTmpFieldField<Field, outerProductType, Type>::clear(tf);
    return tRes;
}


template<template<class> class Field, class Type>
void magSqr(FieldField<Field, scalar>& sf, const FieldField<Field, Type>& f)
{
    forAll(sf, i)
    {
        magSqr(sf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, scalar> > magSqr(const FieldField<Field, Type>& f)
{
    tmp<FieldField<Field, scalar> > tRes
    (
        FieldField<Field, scalar>::NewCalculatedType(f)
    );

    magSqr(tRes(), f);
    return tRes;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, scalar> > magSqr(const tmp<FieldField<Field, Type> >& tf)
{
    tmp<FieldField<Field, scalar> > tRes
    (
        reuseTmpFieldField<Field, scalar, Type>::New(tf)
    );

    magSqr(tRes(), tf());
    reuseTmpFieldField<Field, scalar, Type>::clear(tf);
    return tRes;
}


template<template<class> class Field, class Type>
void mag(FieldField<Field, scalar>& sf, const FieldField<Field, Type>& f)
{
    forAll(sf, i)
    {
        mag(sf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, scalar> > mag(const FieldField<Field, Type>& f)
{
    tmp<FieldField<Field, scalar> > tRes
    (
        FieldField<Field, scalar>::NewCalculatedType(f)
    );

    mag(tRes(), f);
    return tRes;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, scalar> > mag(const tmp<FieldField<Field, Type> >& tf)
{
    tmp<FieldField<Field, scalar> > tRes
    (
        reuseTmpFieldField<Field, scalar, Type>::New(tf)
    );

    mag(tRes(), tf());
    reuseTmpFieldField<Field, scalar, Type>::clear(tf);
    return tRes;
}


template<template<class> class Field, class Type>
void cmptMax
(
    FieldField<Field, typename FieldField<Field, Type>::cmptType>& cf,
    const FieldField<Field, Type>& f
)
{
    forAll(cf, i)
    {
        cmptMax(cf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptMax
(
    const FieldField<Field, Type>& f
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;
    tmp<FieldField<Field, cmptType> > tRes
    (
        FieldField<Field, cmptType>::NewCalculatedType(f)
    );
    cmptMax(tRes(), f);
    return tRes;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptMax
(
    const tmp<FieldField<Field, Type> >& tf
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;
    tmp<FieldField<Field, cmptType> > tRes
    (
        reuseTmpFieldField<Field, cmptType, Type>::New(tf)
    );
    cmptMax(tRes(), tf());
    reuseTmpFieldField<Field, cmptType, Type>::clear(tf);
    return tRes;
}


template<template<class> class Field, class Type>
void cmptMin
(
    FieldField<Field, typename FieldField<Field, Type>::cmptType>& cf,
    const FieldField<Field, Type>& f
)
{
    forAll(cf, i)
    {
        cmptMin(cf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptMin
(
    const FieldField<Field, Type>& f
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;
    tmp<FieldField<Field, cmptType> > tRes
    (
        FieldField<Field, cmptType>::NewCalculatedType(f)
    );
    cmptMin(tRes(), f);
    return tRes;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptMin
(
    const tmp<FieldField<Field, Type> >& tf
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;
    tmp<FieldField<Field, cmptType> > tRes
    (
        reuseTmpFieldField<Field, cmptType, Type>::New(tf)
    );
    cmptMin(tRes(), tf());
    reuseTmpFieldField<Field, cmptType, Type>::clear(tf);
    return tRes;
}


template<template<class> class Field, class Type>
void cmptAv
(
    FieldField<Field, typename FieldField<Field, Type>::cmptType>& cf,
    const FieldField<Field, Type>& f
)
{
    forAll(cf, i)
    {
        cmptAv(cf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptAv
(
    const FieldField<Field, Type>& f
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;
    tmp<FieldField<Field, cmptType> > tRes
    (
        FieldField<Field, cmptType>::NewCalculatedType(f)
    );
    cmptAv(tRes(), f);
    return tRes;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptAv
(
    const tmp<FieldField<Field, Type> >& tf
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;
    tmp<FieldField<Field, cmptType> > tRes
    (
        reuseTmpFieldField<Field, cmptType, Type>::New(tf)
    );
    cmptAv(tRes(), tf());
    reuseTmpFieldField<Field, cmptType, Type>::clear(tf);
    return tRes;
}


template<template<class> class Field, class Type>
void cmptMag
(
    FieldField<Field, Type>& cf,
    const FieldField<Field, Type>& f
)
{
    forAll(cf, i)
    {
        cmptMag(cf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > cmptMag
(
    const FieldField<Field, Type>& f
)
{
    tmp<FieldField<Field, Type> > tRes
    (
        FieldField<Field, Type>::NewCalculatedType(f)
    );
    cmptMag(tRes(), f);
    return tRes;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > cmptMag
(
    const tmp<FieldField<Field, Type> >& tf
)
{
    tmp<FieldField<Field, Type> > tRes
    (
        reuseTmpFieldField<Field, Type, Type>::New(tf)
    );
    cmptMag(tRes(), tf());
    reuseTmpFieldField<Field, Type, Type>::clear(tf);
    return tRes;
}


#define TMP_UNARY_FUNCTION(returnType, func)                                  \
                                                                              \
template<template<class> class Field, class Type>                             \
returnType func(const tmp<FieldField<Field, Type> >& tf1)                     \
{                                                                             \
    returnType res = func(tf1());                                             \
    tf1.clear();                                                              \
    return res;                                                               \
}

template<template<class> class Field, class Type>
Type max(const FieldField<Field, Type>& f)
{
    label i = 0;
    while (i < f.size() && !f[i].size()) i++;

    if (i < f.size())
    {
        Type Max(max(f[i]));

        for (label j=i+1; j<f.size(); j++)
        {
            if (f[j].size())
            {
                Max = max(max(f[j]), Max);
            }
        }

        return Max;
    }
    else
    {
        return pTraits<Type>::min;
    }
}

TMP_UNARY_FUNCTION(Type, max)

template<template<class> class Field, class Type>
Type min(const FieldField<Field, Type>& f)
{
    label i = 0;
    while (i < f.size() && !f[i].size()) i++;

    if (i < f.size())
    {
        label i = 0;
        while (!f[i].size()) i++;

        Type Min(min(f[i]));

        for (label j=i+1; j<f.size(); j++)
        {
            if (f[j].size())
            {
                Min = min(min(f[j]), Min);
            }
        }

        return Min;
    }
    else
    {
        return pTraits<Type>::max;
    }
}

TMP_UNARY_FUNCTION(Type, min)

template<template<class> class Field, class Type>
Type sum(const FieldField<Field, Type>& f)
{
    if (f.size())
    {
        Type Sum = pTraits<Type>::zero;

        forAll(f, i)
        {
            Sum += sum(f[i]);
        }

        return Sum;
    }
    else
    {
        return pTraits<Type>::zero;
    }
}

TMP_UNARY_FUNCTION(Type, sum)

template<template<class> class Field, class Type>
scalar sumMag(const FieldField<Field, Type>& f)
{
    if (f.size())
    {
        scalar SumMag = 0.0;

        forAll(f, i)
        {
            SumMag += sumMag(f[i]);
        }

        return SumMag;
    }
    else
    {
        return 0.0;
    }
}

TMP_UNARY_FUNCTION(scalar, sumMag)

template<template<class> class Field, class Type>
Type average(const FieldField<Field, Type>& f)
{
    if (f.size())
    {
        label n = 0;

        forAll(f, i)
        {
            n += f[i].size();
        }

        if (n == 0)
        {
            WarningInFunction
                << "empty fieldField, returning zero" << endl;

            return pTraits<Type>::zero;
        }

        Type avrg = sum(f)/n;

        return avrg;
    }
    else
    {
        WarningInFunction
            << "empty fieldField, returning zero" << endl;

        return pTraits<Type>::zero;
    }
}

TMP_UNARY_FUNCTION(Type, average)


#include "PstreamReduceOps.hpp"

#define G_UNARY_FUNCTION(returnType, gFunc, func, rFunc)                      \
                                                                              \
template<template<class> class Field, class Type>                             \
returnType gFunc(const FieldField<Field, Type>& f)                            \
{                                                                             \
    returnType res = func(f);                                                 \
    reduce(res, rFunc##Op<Type>());                                           \
    return res;                                                               \
}                                                                             \
TMP_UNARY_FUNCTION(returnType, gFunc)

G_UNARY_FUNCTION(Type, gMax, max, max)
G_UNARY_FUNCTION(Type, gMin, min, min)
G_UNARY_FUNCTION(Type, gSum, sum, sum)
G_UNARY_FUNCTION(scalar, gSumMag, sumMag, sum)

#undef G_UNARY_FUNCTION


template<template<class> class Field, class Type>
Type gAverage(const FieldField<Field, Type>& f)
{
    label n = 0;

    forAll(f, i)
    {
        n += f[i].size();
    }

    reduce(n, sumOp<label>());

    if (n > 0)
    {
        Type avrg = gSum(f)/n;

        return avrg;
    }
    else
    {
        WarningInFunction
            << "empty fieldField, returning zero" << endl;

        return pTraits<Type>::zero;
    }
}

TMP_UNARY_FUNCTION(Type, gAverage)

#undef TMP_UNARY_FUNCTION


BINARY_FUNCTION(Type, Type, Type, max)
BINARY_FUNCTION(Type, Type, Type, min)
BINARY_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_FUNCTION(Type, Type, Type, cmptDivide)

BINARY_TYPE_FUNCTION(Type, Type, Type, max)
BINARY_TYPE_FUNCTION(Type, Type, Type, min)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptDivide)


/* * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * */

UNARY_OPERATOR(Type, Type, -, negate)

#ifndef __INTEL_COMPILER
BINARY_OPERATOR(Type, Type, scalar, *, multiply)
BINARY_OPERATOR(Type, scalar, Type, *, multiply)
#endif

BINARY_OPERATOR(Type, Type, scalar, /, divide)

BINARY_TYPE_OPERATOR_SF(Type, scalar, Type, *, multiply)
BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, *, multiply)

BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, /, divide)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define PRODUCT_OPERATOR(product, op, opFunc)                                  \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
void opFunc                                                                    \
(                                                                              \
    FieldField<Field1, typename product<Type1, Type2>::type>& f,               \
    const FieldField<Field1, Type1>& f1,                                       \
    const FieldField<Field2, Type2>& f2                                        \
)                                                                              \
{                                                                              \
    forAll(f, i)                                                               \
    {                                                                          \
        opFunc(f[i], f1[i], f2[i]);                                            \
    }                                                                          \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
tmp<FieldField<Field1, typename product<Type1, Type2>::type> >                 \
operator op                                                                    \
(                                                                              \
    const FieldField<Field1, Type1>& f1,                                       \
    const FieldField<Field2, Type2>& f2                                        \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field1, productType> > tRes                                 \
    (                                                                          \
        FieldField<Field1, productType>::NewCalculatedType(f1)                 \
    );                                                                         \
    opFunc(tRes(), f1, f2);                                                    \
    return tRes;                                                               \
}                                                                              \
                                                                               \
template<template<class> class Field, class Type1, class Type2>                \
tmp<FieldField<Field, typename product<Type1, Type2>::type> >                  \
operator op                                                                    \
(                                                                              \
    const FieldField<Field, Type1>& f1,                                        \
    const tmp<FieldField<Field, Type2> >& tf2                                  \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field, productType> > tRes                                  \
    (                                                                          \
        reuseTmpFieldField<Field, productType, Type2>::New(tf2)                \
    );                                                                         \
    opFunc(tRes(), f1, tf2());                                                 \
    reuseTmpFieldField<Field, productType, Type2>::clear(tf2);                 \
    return tRes;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
tmp<FieldField<Field, typename product<Type1, Type2>::type> >                  \
operator op                                                                    \
(                                                                              \
    const FieldField<Field1, Type1>& f1,                                       \
    const tmp<FieldField<Field2, Type2> >& tf2                                 \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field1, productType> > tRes                                 \
    (                                                                          \
        FieldField<Field1, productType>::NewCalculatedType(f1)                 \
    );                                                                         \
    opFunc(tRes(), f1, tf2());                                                 \
    tf2.clear();                                                               \
    return tRes;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
tmp<FieldField<Field1, typename product<Type1, Type2>::type> >                 \
operator op                                                                    \
(                                                                              \
    const tmp<FieldField<Field1, Type1> >& tf1,                                \
    const FieldField<Field2, Type2>& f2                                        \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field1, productType> > tRes                                 \
    (                                                                          \
        reuseTmpFieldField<Field1, productType, Type1>::New(tf1)               \
    );                                                                         \
    opFunc(tRes(), tf1(), f2);                                                 \
    reuseTmpFieldField<Field1, productType, Type1>::clear(tf1);                \
    return tRes;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
tmp<FieldField<Field1, typename product<Type1, Type2>::type> >                 \
operator op                                                                    \
(                                                                              \
    const tmp<FieldField<Field1, Type1> >& tf1,                                \
    const tmp<FieldField<Field2, Type2> >& tf2                                 \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field1, productType> > tRes                                 \
    (                                                                          \
        reuseTmpTmpFieldField<Field1, productType, Type1, Type1, Type2>::New   \
        (tf1, tf2)                                                             \
    );                                                                         \
    opFunc(tRes(), tf1(), tf2());                                              \
    reuseTmpTmpFieldField<Field1, productType, Type1, Type1, Type2>::clear     \
    (tf1, tf2);                                                                \
    return tRes;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Type,                                                                \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt                                                            \
>                                                                              \
void opFunc                                                                    \
(                                                                              \
    FieldField<Field, typename product<Type, Form>::type>& f,                  \
    const FieldField<Field, Type>& f1,                                         \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                     \
)                                                                              \
{                                                                              \
    forAll(f, i)                                                               \
    {                                                                          \
        opFunc(f[i], f1[i], vs);                                               \
    }                                                                          \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Type,                                                                \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt                                                            \
>                                                                              \
tmp<FieldField<Field, typename product<Type, Form>::type> >                    \
operator op                                                                    \
(                                                                              \
    const FieldField<Field, Type>& f1,                                         \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                     \
)                                                                              \
{                                                                              \
    typedef typename product<Type, Form>::type productType;                    \
    tmp<FieldField<Field, productType> > tRes                                  \
    (                                                                          \
        FieldField<Field, productType>::NewCalculatedType(f1)                  \
    );                                                                         \
    opFunc(tRes(), f1, static_cast<const Form&>(vs));                          \
    return tRes;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Type,                                                                \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt                                                            \
>                                                                              \
tmp<FieldField<Field, typename product<Type, Form>::type> >                    \
operator op                                                                    \
(                                                                              \
    const tmp<FieldField<Field, Type> >& tf1,                                  \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                     \
)                                                                              \
{                                                                              \
    typedef typename product<Type, Form>::type productType;                    \
    tmp<FieldField<Field, productType> > tRes                                  \
    (                                                                          \
        reuseTmpFieldField<Field, productType, Type>::New(tf1)                 \
    );                                                                         \
    opFunc(tRes(), tf1(), static_cast<const Form&>(vs));                       \
    reuseTmpFieldField<Field, productType, Type>::clear(tf1);                  \
    return tRes;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt,                                                           \
    class Type                                                                 \
>                                                                              \
void opFunc                                                                    \
(                                                                              \
    FieldField<Field, typename product<Form, Type>::type>& f,                  \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                    \
    const FieldField<Field, Type>& f1                                          \
)                                                                              \
{                                                                              \
    forAll(f, i)                                                               \
    {                                                                          \
        opFunc(f[i], vs, f1[i]);                                               \
    }                                                                          \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt,                                                           \
    class Type                                                                 \
>                                                                              \
tmp<FieldField<Field, typename product<Form, Type>::type> >                    \
operator op                                                                    \
(                                                                              \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                    \
    const FieldField<Field, Type>& f1                                          \
)                                                                              \
{                                                                              \
    typedef typename product<Form, Type>::type productType;                    \
    tmp<FieldField<Field, productType> > tRes                                  \
    (                                                                          \
        FieldField<Field, productType>::NewCalculatedType(f1)                  \
    );                                                                         \
    opFunc(tRes(), static_cast<const Form&>(vs), f1);                          \
    return tRes;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt,                                                           \
    class Type                                                                 \
>                                                                              \
tmp<FieldField<Field, typename product<Form, Type>::type> >                    \
operator op                                                                    \
(                                                                              \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                    \
    const tmp<FieldField<Field, Type> >& tf1                                   \
)                                                                              \
{                                                                              \
    typedef typename product<Form, Type>::type productType;                    \
    tmp<FieldField<Field, productType> > tRes                                  \
    (                                                                          \
        reuseTmpFieldField<Field, productType, Type>::New(tf1)                 \
    );                                                                         \
    opFunc(tRes(), static_cast<const Form&>(vs), tf1());                       \
    reuseTmpFieldField<Field, productType, Type>::clear(tf1);                  \
    return tRes;                                                               \
}

PRODUCT_OPERATOR(typeOfSum, +, add)
PRODUCT_OPERATOR(typeOfSum, -, subtract)

PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)

#undef PRODUCT_OPERATOR


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "undefFieldFunctionsM.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
