/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    CML::Field

Description
    Generic templated field type.

SourceFiles
    FieldFunctions.hpp
    FieldFunctionsM.hpp
    FieldMapper.hpp
    FieldM.hpp


\*---------------------------------------------------------------------------*/

#ifndef Field_H
#define Field_H

#include "tmp.hpp"
#include "direction.hpp"
#include "VectorSpace.hpp"
#include "scalarList.hpp"
#include "labelList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Type>
class Field;

//- Pre-declare related SubField type
template<class Type>
class SubField;

template<class Type>
Ostream& operator<<(Ostream&, const Field<Type>&);

template<class Type>
Ostream& operator<<(Ostream&, const tmp<Field<Type> >&);

class FieldMapper;
class dictionary;

/*---------------------------------------------------------------------------*\
                           Class Field Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Field
:
    public refCount,
    public List<Type>
{

public:

    //- Component type
    typedef typename pTraits<Type>::cmptType cmptType;

    //- Declare type of subField
    typedef SubField<Type> subField;


    // Static data members

        static const char* const typeName;


    // Static Member Functions

        //- Return a null field
        inline static const Field<Type>& null()
        {
            return NullSingletonRef< Field<Type> >();
        }


    // Constructors

        //- Construct null
        //  Used for temporary fields which are initialised after construction
        Field();

        //- Construct given size
        //  Used for temporary fields which are initialised after construction
        explicit Field(const label);

        //- Construct given size and initial value
        Field(const label, const Type&);

        //- Construct given size and initialed to zero
        Field(const label, const zero);

        //- Construct as copy of a UList\<Type\>
        explicit Field(const UList<Type>&);

        //- Construct by transferring the List contents
        explicit Field(const Xfer<List<Type> >&);

        //- Construct by 1 to 1 mapping from the given field
        Field
        (
            const UList<Type>& mapF,
            const labelUList& mapAddressing
        );

        //- Construct by 1 to 1 mapping from the given tmp field
        Field
        (
            const tmp<Field<Type> >& tmapF,
            const labelUList& mapAddressing
        );

        //- Construct by interpolative mapping from the given field
        Field
        (
            const UList<Type>& mapF,
            const labelListList& mapAddressing,
            const scalarListList& weights
        );

        //- Construct by interpolative mapping from the given tmp field
        Field
        (
            const tmp<Field<Type> >& tmapF,
            const labelListList& mapAddressing,
            const scalarListList& weights
        );

        //- Construct by mapping from the given field
        Field
        (
            const UList<Type>& mapF,
            const FieldMapper& map,
            const bool applyFlip = true
        );

        //- Construct by mapping from the given field
        Field
        (
            const UList<Type>& mapF,
            const FieldMapper& map,
            const Type& defaultValue,
            const bool applyFlip = true
        );

        //- Construct by mapping from the given field
        Field
        (
            const UList<Type>& mapF,
            const FieldMapper& map,
            const UList<Type>& defaultValues,
            const bool applyFlip = true
        );

        //- Construct by mapping from the given tmp field
        Field
        (
            const tmp<Field<Type> >& tmapF,
            const FieldMapper& map,
            const bool applyFlip = true
        );

        //- Construct by mapping from the given tmp field. Supplied uniform
        //  value for unmapped items
        Field
        (
            const tmp<Field<Type> >& tmapF,
            const FieldMapper& map,
            const Type& defaultValue,
            const bool applyFlip = true
        );

        //- Construct by mapping from the given tmp field. Supplied values
        //  for unmapped items
        Field
        (
            const tmp<Field<Type> >& tmapF,
            const FieldMapper& map,
            const UList<Type>& defaultValues,
            const bool applyFlip = true
        );

        //- Construct as copy
        Field(const Field<Type>&);

        //- Construct as copy or re-use as specified.
        Field(Field<Type>&, bool reUse);

        //- Construct by transferring the Field contents
        Field(const Xfer<Field<Type> >&);

#ifdef __INTEL_COMPILER
        //- Construct as copy of subField
        Field(const typename Field<Type>::subField&);
#endif

        //- Construct as copy of tmp<Field>
#       ifdef ConstructFromTmp
        Field(const tmp<Field<Type> >&);
#       endif

        //- Construct from Istream
        Field(Istream&);

        //- Construct from a dictionary entry
        Field(const word& keyword, const dictionary&, const label size);

        //- Clone
        tmp<Field<Type> > clone() const;

        //- Return a pointer to a new calculatedFvPatchFieldField created on
        //  freestore without setting patchField values
        template<class Type2>
        static tmp<Field<Type> > NewCalculatedType(const Field<Type2>& f)
        {
            return tmp<Field<Type> >(new Field<Type>(f.size()));
        }


    // Member Functions

        //- 1 to 1 map from the given field
        void map
        (
            const UList<Type>& mapF,
            const labelUList& mapAddressing
        );

        //- 1 to 1 map from the given tmp field
        void map
        (
            const tmp<Field<Type> >& tmapF,
            const labelUList& mapAddressing
        );

        //- Interpolative map from the given field
        void map
        (
            const UList<Type>& mapF,
            const labelListList& mapAddressing,
            const scalarListList& weights
        );

        //- Interpolative map from the given tmp field
        void map
        (
            const tmp<Field<Type> >& tmapF,
            const labelListList& mapAddressing,
            const scalarListList& weights
        );

        //- Map from the given field
        void map
        (
            const UList<Type>& mapF,
            const FieldMapper& map,
            const bool applyFlip = true
        );

        //- Map from the given tmp field
        void map
        (
            const tmp<Field<Type> >& tmapF,
            const FieldMapper& map,
            const bool applyFlip = true
        );

        //- Map from self
        void autoMap
        (
            const FieldMapper& map,
            const bool applyFlip = true
        );

        //- 1 to 1 reverse-map from the given field
        void rmap
        (
            const UList<Type>& mapF,
            const labelUList& mapAddressing
        );

        //- 1 to 1 reverse-map from the given tmp field
        void rmap
        (
            const tmp<Field<Type> >& tmapF,
            const labelUList& mapAddressing
        );

        //- Interpolative reverse map from the given field
        void rmap
        (
            const UList<Type>& mapF,
            const labelUList& mapAddressing,
            const UList<scalar>& weights
        );

        //- Interpolative reverse map from the given tmp field
        void rmap
        (
            const tmp<Field<Type> >& tmapF,
            const labelUList& mapAddressing,
            const UList<scalar>& weights
        );

        //- Negate this field
        void negate();

        //- Return a component field of the field
        tmp<Field<cmptType> > component(const direction) const;

        //- Replace a component field of the field
        void replace(const direction, const UList<cmptType>&);

        //- Replace a component field of the field
        void replace(const direction, const tmp<Field<cmptType> >&);

        //- Replace a component field of the field
        void replace(const direction, const cmptType&);

        template<class VSForm>
        VSForm block(const label start) const;

        //- Return the field transpose (only defined for second rank tensors)
        tmp<Field<Type> > T() const;

        //- Write the field as a dictionary entry
        void writeEntry(const word& keyword, Ostream& os) const;


    // Member operators

        void operator=(const Field<Type>&);
        void operator=(const UList<Type>&);
        void operator=(const SubField<Type>&);
        void operator=(const tmp<Field<Type> >&);
        void operator=(const Type&);
        void operator=(const zero);

        template<class Form, class Cmpt, direction nCmpt>
        void operator=(const VectorSpace<Form,Cmpt,nCmpt>&);

        void operator+=(const UList<Type>&);
        void operator+=(const tmp<Field<Type> >&);

        void operator-=(const UList<Type>&);
        void operator-=(const tmp<Field<Type> >&);

        void operator*=(const UList<scalar>&);
        void operator*=(const tmp<Field<scalar> >&);

        void operator/=(const UList<scalar>&);
        void operator/=(const tmp<Field<scalar> >&);

        void operator+=(const Type&);
        void operator-=(const Type&);

        void operator*=(const scalar&);
        void operator/=(const scalar&);


    // IOstream operators

        friend Ostream& operator<< <Type>
        (Ostream&, const Field<Type>&);

        friend Ostream& operator<< <Type>
        (Ostream&, const tmp<Field<Type> >&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "FieldFunctions.hpp"

#include "FieldMapper.hpp"
#include "FieldM.hpp"
#include "dictionary.hpp"
#include "contiguous.hpp"
#include "mapDistributeBase.hpp"
#include "flipOp.hpp"

// * * * * * * * * * * * * * * * Static Members  * * * * * * * * * * * * * * //

template<class Type>
const char* const CML::Field<Type>::typeName("Field");


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::Field<Type>::Field()
:
    List<Type>()
{}


template<class Type>
CML::Field<Type>::Field(const label size)
:
    List<Type>(size)
{}


template<class Type>
CML::Field<Type>::Field(const label size, const Type& t)
:
    List<Type>(size, t)
{}


template<class Type>
CML::Field<Type>::Field(const label size, const zero)
:
    List<Type>(size, Zero)
{}


template<class Type>
CML::Field<Type>::Field
(
    const UList<Type>& mapF,
    const labelUList& mapAddressing
)
:
    List<Type>(mapAddressing.size())
{
    map(mapF, mapAddressing);
}


template<class Type>
CML::Field<Type>::Field
(
    const tmp<Field<Type> >& tmapF,
    const labelUList& mapAddressing
)
:
    List<Type>(mapAddressing.size())
{
    map(tmapF, mapAddressing);
}


template<class Type>
CML::Field<Type>::Field
(
    const UList<Type>& mapF,
    const labelListList& mapAddressing,
    const scalarListList& mapWeights
)
:
    List<Type>(mapAddressing.size())
{
    map(mapF, mapAddressing, mapWeights);
}


template<class Type>
CML::Field<Type>::Field
(
    const tmp<Field<Type> >& tmapF,
    const labelListList& mapAddressing,
    const scalarListList& mapWeights
)
:
    List<Type>(mapAddressing.size())
{
    map(tmapF, mapAddressing, mapWeights);
}


template<class Type>
CML::Field<Type>::Field
(
    const UList<Type>& mapF,
    const FieldMapper& mapper,
    const bool applyFlip
)
:
    List<Type>(mapper.size())
{
    map(mapF, mapper, applyFlip);
}


template<class Type>
CML::Field<Type>::Field
(
    const UList<Type>& mapF,
    const FieldMapper& mapper,
    const Type& defaultValue,
    const bool applyFlip
)
:
    List<Type>(mapper.size(), defaultValue)
{
    map(mapF, mapper, applyFlip);
}


template<class Type>
CML::Field<Type>::Field
(
    const UList<Type>& mapF,
    const FieldMapper& mapper,
    const UList<Type>& defaultValues,
    const bool applyFlip
)
:
    List<Type>(defaultValues)
{
    map(mapF, mapper, applyFlip);
}


template<class Type>
CML::Field<Type>::Field
(
    const tmp<Field<Type> >& tmapF,
    const FieldMapper& mapper,
    const bool applyFlip
)
:
    List<Type>(mapper.size())
{
    map(tmapF, mapper, applyFlip);
}


template<class Type>
CML::Field<Type>::Field
(
    const tmp<Field<Type> >& tmapF,
    const FieldMapper& mapper,
    const Type& defaultValue,
    const bool applyFlip
)
:
    List<Type>(mapper.size(), defaultValue)
{
    map(tmapF, mapper, applyFlip);
}


template<class Type>
CML::Field<Type>::Field
(
    const tmp<Field<Type> >& tmapF,
    const FieldMapper& mapper,
    const UList<Type>& defaultValues,
    const bool applyFlip
)
:
    List<Type>(defaultValues)
{
    map(tmapF, mapper, applyFlip);
}


template<class Type>
CML::Field<Type>::Field(const Field<Type>& f)
:
    refCount(),
    List<Type>(f)
{}


template<class Type>
CML::Field<Type>::Field(Field<Type>& f, bool reUse)
:
    List<Type>(f, reUse)
{}


template<class Type>
CML::Field<Type>::Field(const Xfer<List<Type> >& f)
:
    List<Type>(f)
{}


template<class Type>
CML::Field<Type>::Field(const Xfer<Field<Type> >& f)
:
    List<Type>(f)
{}


#ifdef __INTEL_COMPILER
template<class Type>
CML::Field<Type>::Field(const typename Field<Type>::subField& sf)
:
    List<Type>(sf)
{}
#endif


template<class Type>
CML::Field<Type>::Field(const UList<Type>& list)
:
    List<Type>(list)
{}


// Construct as copy of tmp<Field>
#ifdef ConstructFromTmp
template<class Type>
CML::Field<Type>::Field(const tmp<Field<Type> >& tf)
:
    List<Type>(const_cast<Field<Type>&>(tf()), tf.isTmp())
{
    const_cast<Field<Type>&>(tf()).resetRefCount();
}
#endif


template<class Type>
CML::Field<Type>::Field(Istream& is)
:
    List<Type>(is)
{}


template<class Type>
CML::Field<Type>::Field
(
    const word& keyword,
    const dictionary& dict,
    const label s
)
{
    if (s)
    {
        ITstream& is = dict.lookup(keyword);

        // Read first token
        token firstToken(is);

        if (firstToken.isWord())
        {
            if (firstToken.wordToken() == "uniform")
            {
                this->setSize(s);
                operator=(pTraits<Type>(is));
            }
            else if (firstToken.wordToken() == "nonuniform")
            {
                is >> static_cast<List<Type>&>(*this);
                if (this->size() != s)
                {
                    FatalIOErrorInFunction(dict)
                        << "size " << this->size()
                        << " is not equal to the given value of " << s
                        << exit(FatalIOError);
                }
            }
            else
            {
                FatalIOErrorInFunction(dict)
                    << "expected keyword 'uniform' or 'nonuniform', found "
                    << firstToken.wordToken()
                    << exit(FatalIOError);
            }
        }
        else
        {
            if (is.version() == 2.0)
            {
                IOWarningInFunction(dict)
                    << "expected keyword 'uniform' or 'nonuniform', "
                       "assuming deprecated Field format from "
                       "CML version 2.0." << endl;

                this->setSize(s);

                is.putBack(firstToken);
                operator=(pTraits<Type>(is));
            }
            else
            {
                FatalIOErrorInFunction(dict)
                    << "expected keyword 'uniform' or 'nonuniform', found "
                    << firstToken.info()
                    << exit(FatalIOError);
            }
        }
    }
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::Field<Type>::clone() const
{
    return tmp<Field<Type> >(new Field<Type>(*this));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::Field<Type>::map
(
    const UList<Type>& mapF,
    const labelUList& mapAddressing
)
{
    Field<Type>& f = *this;

    if (f.size() != mapAddressing.size())
    {
        f.setSize(mapAddressing.size());
    }

    if (mapF.size() > 0)
    {
        forAll(f, i)
        {
            label mapI = mapAddressing[i];

            if (mapI >= 0)
            {
                f[i] = mapF[mapI];
            }
        }
    }
}


template<class Type>
void CML::Field<Type>::map
(
    const tmp<Field<Type> >& tmapF,
    const labelUList& mapAddressing
)
{
    map(tmapF(), mapAddressing);
    tmapF.clear();
}


template<class Type>
void CML::Field<Type>::map
(
    const UList<Type>& mapF,
    const labelListList& mapAddressing,
    const scalarListList& mapWeights
)
{
    Field<Type>& f = *this;

    if (f.size() != mapAddressing.size())
    {
        f.setSize(mapAddressing.size());
    }

    if (mapWeights.size() != mapAddressing.size())
    {
        FatalErrorInFunction
            << "Weights and addressing map have different sizes.  Weights size: "
            << mapWeights.size() << " map size: " << mapAddressing.size()
            << abort(FatalError);
    }

    forAll(f, i)
    {
        const labelList&  localAddrs   = mapAddressing[i];
        const scalarList& localWeights = mapWeights[i];

        f[i] = pTraits<Type>::zero;

        forAll(localAddrs, j)
        {
            f[i] += localWeights[j]*mapF[localAddrs[j]];
        }
    }
}


template<class Type>
void CML::Field<Type>::map
(
    const tmp<Field<Type> >& tmapF,
    const labelListList& mapAddressing,
    const scalarListList& mapWeights
)
{
    map(tmapF(), mapAddressing, mapWeights);
    tmapF.clear();
}


template<class Type>
void CML::Field<Type>::map
(
    const UList<Type>& mapF,
    const FieldMapper& mapper,
    const bool applyFlip
)
{
    if (mapper.distributed())
    {
        // Fetch remote parts of mapF
        const mapDistributeBase& distMap = mapper.distributeMap();
        Field<Type> newMapF(mapF);

        if (applyFlip)
        {
            distMap.distribute(newMapF);
        }
        else
        {
            distMap.distribute(newMapF, noOp());
        }

        if (mapper.direct() && notNull(mapper.directAddressing()))
        {
            map(newMapF, mapper.directAddressing());
        }
        else if (!mapper.direct())
        {
            map(newMapF, mapper.addressing(), mapper.weights());
        }
        else if (mapper.direct() && isNull(mapper.directAddressing()))
        {
            // Special case, no local mapper. Assume ordering already correct
            // from distribution. Note: this behaviour is different compared
            // to local mapper.
            this->transfer(newMapF);
            this->setSize(mapper.size());
        }
    }
    else
    {
        if
        (
            mapper.direct()
         && notNull(mapper.directAddressing())
         && mapper.directAddressing().size()
        )
        {
            map(mapF, mapper.directAddressing());
        }
        else if (!mapper.direct() && mapper.addressing().size())
        {
            map(mapF, mapper.addressing(), mapper.weights());
        }
    }
}


template<class Type>
void CML::Field<Type>::map
(
    const tmp<Field<Type> >& tmapF,
    const FieldMapper& mapper,
    const bool applyFlip
)
{
    map(tmapF(), mapper, applyFlip);
    tmapF.clear();
}


template<class Type>
void CML::Field<Type>::autoMap
(
    const FieldMapper& mapper,
    const bool applyFlip
)
{
    if (mapper.distributed())
    {
        // Fetch remote parts of *this
        const mapDistributeBase& distMap = mapper.distributeMap();
        Field<Type> fCpy(*this);

        if (applyFlip)
        {
            distMap.distribute(fCpy);
        }
        else
        {
            distMap.distribute(fCpy, noOp());
        }

        if
        (
            (mapper.direct()
         && notNull(mapper.directAddressing()))
         || !mapper.direct()
        )
        {
            this->map(fCpy, mapper);
        }
        else if (mapper.direct() && isNull(mapper.directAddressing()))
        {
            // Special case, no local mapper. Assume ordering already correct
            // from distribution. Note: this behaviour is different compared
            // to local mapper.
            this->transfer(fCpy);
            this->setSize(mapper.size());
        }
    }
    else
    {
        if
        (
            (
                mapper.direct()
             && notNull(mapper.directAddressing())
             && mapper.directAddressing().size()
            )
         || (!mapper.direct() && mapper.addressing().size())
        )
        {
            Field<Type> fCpy(*this);
            map(fCpy, mapper);
        }
        else
        {
            this->setSize(mapper.size());
        }
    }
}


template<class Type>
void CML::Field<Type>::rmap
(
    const UList<Type>& mapF,
    const labelUList& mapAddressing
)
{
    Field<Type>& f = *this;

    forAll(mapF, i)
    {
        label mapI = mapAddressing[i];

        if (mapI >= 0)
        {
            f[mapI] = mapF[i];
        }
    }
}


template<class Type>
void CML::Field<Type>::rmap
(
    const tmp<Field<Type> >& tmapF,
    const labelUList& mapAddressing
)
{
    rmap(tmapF(), mapAddressing);
    tmapF.clear();
}


template<class Type>
void CML::Field<Type>::rmap
(
    const UList<Type>& mapF,
    const labelUList& mapAddressing,
    const UList<scalar>& mapWeights
)
{
    Field<Type>& f = *this;

    f = pTraits<Type>::zero;

    forAll(mapF, i)
    {
        f[mapAddressing[i]] += mapF[i]*mapWeights[i];
    }
}


template<class Type>
void CML::Field<Type>::rmap
(
    const tmp<Field<Type> >& tmapF,
    const labelUList& mapAddressing,
    const UList<scalar>& mapWeights
)
{
    rmap(tmapF(), mapAddressing, mapWeights);
    tmapF.clear();
}


template<class Type>
void CML::Field<Type>::negate()
{
    TFOR_ALL_F_OP_OP_F(Type, *this, =, -, Type, *this)
}


template<class Type>
CML::tmp<CML::Field<typename CML::Field<Type>::cmptType> >
CML::Field<Type>::component
(
    const direction d
) const
{
    tmp<Field<cmptType> > Component(new Field<cmptType>(this->size()));
    ::CML::component(Component(), *this, d);
    return Component;
}


template<class Type>
void CML::Field<Type>::replace
(
    const direction d,
    const UList<cmptType>& sf
)
{
    TFOR_ALL_F_OP_FUNC_S_F(Type, *this, ., replace, const direction, d,
        cmptType, sf)
}


template<class Type>
void CML::Field<Type>::replace
(
    const direction d,
    const tmp<Field<cmptType> >& tsf
)
{
    replace(d, tsf());
    tsf.clear();
}


template<class Type>
void CML::Field<Type>::replace
(
    const direction d,
    const cmptType& c
)
{
    TFOR_ALL_F_OP_FUNC_S_S(Type, *this, ., replace, const direction, d,
        cmptType, c)
}


template<class Type>
template<class VSForm>
VSForm CML::Field<Type>::block(const label start) const
{
    VSForm vs;
    for (direction i=0; i<VSForm::nComponents; i++)
    {
        vs[i] = this->operator[](start + i);
    }
    return vs;
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::Field<Type>::T() const
{
    tmp<Field<Type> > transpose(new Field<Type>(this->size()));
    ::CML::T(transpose(), *this);
    return transpose;
}


template<class Type>
void CML::Field<Type>::writeEntry(const word& keyword, Ostream& os) const
{
    os.writeKeyword(keyword);

    bool uniform = false;

    if (this->size() && contiguous<Type>())
    {
        uniform = true;

        forAll(*this, i)
        {
            if (this->operator[](i) != this->operator[](0))
            {
                uniform = false;
                break;
            }
        }
    }

    if (uniform)
    {
        os << "uniform " << this->operator[](0) << token::END_STATEMENT;
    }
    else
    {
        os << "nonuniform ";
        List<Type>::writeEntry(os);
        os << token::END_STATEMENT;
    }

    os << endl;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void CML::Field<Type>::operator=(const Field<Type>& rhs)
{
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    List<Type>::operator=(rhs);
}


template<class Type>
void CML::Field<Type>::operator=(const SubField<Type>& rhs)
{
    List<Type>::operator=(rhs);
}


template<class Type>
void CML::Field<Type>::operator=(const UList<Type>& rhs)
{
    List<Type>::operator=(rhs);
}


template<class Type>
void CML::Field<Type>::operator=(const tmp<Field>& rhs)
{
    if (this == &(rhs()))
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    // This is dodgy stuff, don't try it at home.
    Field* fieldPtr = rhs.ptr();
    List<Type>::transfer(*fieldPtr);
    delete fieldPtr;
}


template<class Type>
void CML::Field<Type>::operator=(const Type& t)
{
    List<Type>::operator=(t);
}


template<class Type>
void CML::Field<Type>::operator=(const zero)
{
    List<Type>::operator=(Zero);
}


template<class Type>
template<class Form, class Cmpt, CML::direction nCmpt>
void CML::Field<Type>::operator=(const VectorSpace<Form,Cmpt,nCmpt>& vs)
{
    TFOR_ALL_F_OP_S(Type, *this, =, (VectorSpace<Form,Cmpt,nCmpt>), vs)
}


#define COMPUTED_ASSIGNMENT(TYPE, op)                                          \
                                                                               \
template<class Type>                                                           \
void CML::Field<Type>::operator op(const UList<TYPE>& f)                       \
{                                                                              \
    TFOR_ALL_F_OP_F(Type, *this, op, TYPE, f)                                  \
}                                                                              \
                                                                               \
template<class Type>                                                           \
void CML::Field<Type>::operator op(const tmp<Field<TYPE> >& tf)                \
{                                                                              \
    operator op(tf());                                                         \
    tf.clear();                                                                \
}                                                                              \
                                                                               \
template<class Type>                                                           \
void CML::Field<Type>::operator op(const TYPE& t)                              \
{                                                                              \
    TFOR_ALL_F_OP_S(Type, *this, op, TYPE, t)                                  \
}

COMPUTED_ASSIGNMENT(Type, +=)
COMPUTED_ASSIGNMENT(Type, -=)
COMPUTED_ASSIGNMENT(scalar, *=)
COMPUTED_ASSIGNMENT(scalar, /=)

#undef COMPUTED_ASSIGNMENT


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<(Ostream& os, const Field<Type>& f)
{
    os  << static_cast<const List<Type>&>(f);
    return os;
}


template<class Type>
CML::Ostream& CML::operator<<(Ostream& os, const tmp<Field<Type> >& tf)
{
    os  << tf();
    tf.clear();
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
#include "PstreamReduceOps.hpp"
#include "FieldReuseFunctions.hpp"

#define TEMPLATE template<class Type>
#include "FieldM.hpp"
#include "FieldReuseFunctions.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define UNARY_FUNCTION(ReturnType, Type, Func)                                \
                                                                              \
TEMPLATE                                                                      \
void Func(Field<ReturnType>& res, const UList<Type>& f)                       \
{                                                                             \
    TFOR_ALL_F_OP_FUNC_F(ReturnType, res, =, ::CML::Func, Type, f)           \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func(const UList<Type>& f)                            \
{                                                                             \
    tmp<Field<ReturnType> > tRes(new Field<ReturnType>(f.size()));            \
    Func(tRes(), f);                                                          \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func(const tmp<Field<Type> >& tf)                     \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type>::New(tf);       \
    Func(tRes(), tf());                                                       \
    reuseTmp<ReturnType, Type>::clear(tf);                                    \
    return tRes;                                                              \
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define UNARY_OPERATOR(ReturnType, Type, Op, OpFunc)                          \
                                                                              \
TEMPLATE                                                                      \
void OpFunc(Field<ReturnType>& res, const UList<Type>& f)                     \
{                                                                             \
    TFOR_ALL_F_OP_OP_F(ReturnType, res, =, Op, Type, f)                       \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op(const UList<Type>& f)                     \
{                                                                             \
    tmp<Field<ReturnType> > tRes(new Field<ReturnType>(f.size()));            \
    OpFunc(tRes(), f);                                                        \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op(const tmp<Field<Type> >& tf)              \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type>::New(tf);       \
    OpFunc(tRes(), tf());                                                     \
    reuseTmp<ReturnType, Type>::clear(tf);                                    \
    return tRes;                                                              \
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_FUNCTION(ReturnType, Type1, Type2, Func)                       \
                                                                              \
TEMPLATE                                                                      \
void Func                                                                     \
(                                                                             \
    Field<ReturnType>& res,                                                   \
    const UList<Type1>& f1,                                                   \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    TFOR_ALL_F_OP_FUNC_F_F                                                    \
    (                                                                         \
        ReturnType, res, =, ::CML::Func, Type1, f1, Type2, f2                \
    )                                                                         \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
    const UList<Type1>& f1,                                                   \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes(new Field<ReturnType>(f1.size()));           \
    Func(tRes(), f1, f2);                                                     \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
    const UList<Type1>& f1,                                                   \
    const tmp<Field<Type2> >& tf2                                             \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type2>::New(tf2);     \
    Func(tRes(), f1, tf2());                                                  \
    reuseTmp<ReturnType, Type2>::clear(tf2);                                  \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
    const tmp<Field<Type1> >& tf1,                                            \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type1>::New(tf1);     \
    Func(tRes(), tf1(), f2);                                                  \
    reuseTmp<ReturnType, Type1>::clear(tf1);                                  \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
    const tmp<Field<Type1> >& tf1,                                            \
    const tmp<Field<Type2> >& tf2                                             \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes =                                            \
        reuseTmpTmp<ReturnType, Type1, Type1, Type2>::New(tf1, tf2);          \
    Func(tRes(), tf1(), tf2());                                               \
    reuseTmpTmp<ReturnType, Type1, Type1, Type2>::clear(tf1, tf2);            \
    return tRes;                                                              \
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_TYPE_FUNCTION_SF(ReturnType, Type1, Type2, Func)               \
                                                                              \
TEMPLATE                                                                      \
void Func                                                                     \
(                                                                             \
    Field<ReturnType>& res,                                                   \
    const Type1& s1,                                                          \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    TFOR_ALL_F_OP_FUNC_S_F                                                    \
    (                                                                         \
        ReturnType, res, =, ::CML::Func, Type1, s1, Type2, f2                \
    )                                                                         \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
    const Type1& s1,                                                          \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes(new Field<ReturnType>(f2.size()));           \
    Func(tRes(), s1, f2);                                                     \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
    const Type1& s1,                                                          \
    const tmp<Field<Type2> >& tf2                                             \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type2>::New(tf2);     \
    Func(tRes(), s1, tf2());                                                  \
    reuseTmp<ReturnType, Type2>::clear(tf2);                                  \
    return tRes;                                                              \
}


#define BINARY_TYPE_FUNCTION_FS(ReturnType, Type1, Type2, Func)               \
                                                                              \
TEMPLATE                                                                      \
void Func                                                                     \
(                                                                             \
    Field<ReturnType>& res,                                                   \
    const UList<Type1>& f1,                                                   \
    const Type2& s2                                                           \
)                                                                             \
{                                                                             \
    TFOR_ALL_F_OP_FUNC_F_S                                                    \
    (                                                                         \
        ReturnType, res, =, ::CML::Func, Type1, f1, Type2, s2                \
    )                                                                         \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
    const UList<Type1>& f1,                                                   \
    const Type2& s2                                                           \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes(new Field<ReturnType>(f1.size()));           \
    Func(tRes(), f1, s2);                                                     \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
    const tmp<Field<Type1> >& tf1,                                            \
    const Type2& s2                                                           \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type1>::New(tf1);     \
    Func(tRes(), tf1(), s2);                                                  \
    reuseTmp<ReturnType, Type1>::clear(tf1);                                  \
    return tRes;                                                              \
}


#define BINARY_TYPE_FUNCTION(ReturnType, Type1, Type2, Func)                  \
    BINARY_TYPE_FUNCTION_SF(ReturnType, Type1, Type2, Func)                   \
    BINARY_TYPE_FUNCTION_FS(ReturnType, Type1, Type2, Func)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_OPERATOR(ReturnType, Type1, Type2, Op, OpFunc)                 \
                                                                              \
TEMPLATE                                                                      \
void OpFunc                                                                   \
(                                                                             \
    Field<ReturnType>& res,                                                   \
    const UList<Type1>& f1,                                                   \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    TFOR_ALL_F_OP_F_OP_F(ReturnType, res, =, Type1, f1, Op, Type2, f2)        \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
    const UList<Type1>& f1,                                                   \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes(new Field<ReturnType>(f1.size()));           \
    OpFunc(tRes(), f1, f2);                                                   \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
    const UList<Type1>& f1,                                                   \
    const tmp<Field<Type2> >& tf2                                             \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type2>::New(tf2);     \
    OpFunc(tRes(), f1, tf2());                                                \
    reuseTmp<ReturnType, Type2>::clear(tf2);                                  \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
    const tmp<Field<Type1> >& tf1,                                            \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type1>::New(tf1);     \
    OpFunc(tRes(), tf1(), f2);                                                \
    reuseTmp<ReturnType, Type1>::clear(tf1);                                  \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
    const tmp<Field<Type1> >& tf1,                                            \
    const tmp<Field<Type2> >& tf2                                             \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes =                                            \
        reuseTmpTmp<ReturnType, Type1, Type1, Type2>::New(tf1, tf2);          \
    OpFunc(tRes(), tf1(), tf2());                                             \
    reuseTmpTmp<ReturnType, Type1, Type1, Type2>::clear(tf1, tf2);            \
    return tRes;                                                              \
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_TYPE_OPERATOR_SF(ReturnType, Type1, Type2, Op, OpFunc)         \
                                                                              \
TEMPLATE                                                                      \
void OpFunc                                                                   \
(                                                                             \
    Field<ReturnType>& res,                                                   \
    const Type1& s1,                                                          \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    TFOR_ALL_F_OP_S_OP_F(ReturnType, res, =, Type1, s1, Op, Type2, f2)        \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
    const Type1& s1,                                                          \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes(new Field<ReturnType>(f2.size()));           \
    OpFunc(tRes(), s1, f2);                                                   \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
    const Type1& s1,                                                          \
    const tmp<Field<Type2> >& tf2                                             \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type2>::New(tf2);     \
    OpFunc(tRes(), s1, tf2());                                                \
    reuseTmp<ReturnType, Type2>::clear(tf2);                                  \
    return tRes;                                                              \
}


#define BINARY_TYPE_OPERATOR_FS(ReturnType, Type1, Type2, Op, OpFunc)         \
                                                                              \
TEMPLATE                                                                      \
void OpFunc                                                                   \
(                                                                             \
    Field<ReturnType>& res,                                                   \
    const UList<Type1>& f1,                                                   \
    const Type2& s2                                                           \
)                                                                             \
{                                                                             \
    TFOR_ALL_F_OP_F_OP_S(ReturnType, res, =, Type1, f1, Op, Type2, s2)        \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
    const UList<Type1>& f1,                                                   \
    const Type2& s2                                                           \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes(new Field<ReturnType>(f1.size()));           \
    OpFunc(tRes(), f1, s2);                                                   \
    return tRes;                                                              \
}                                                                             \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
    const tmp<Field<Type1> >& tf1,                                            \
    const Type2& s2                                                           \
)                                                                             \
{                                                                             \
    tmp<Field<ReturnType> > tRes = reuseTmp<ReturnType, Type1>::New(tf1);     \
    OpFunc(tRes(), tf1(), s2);                                                \
    reuseTmp<ReturnType, Type1>::clear(tf1);                                  \
    return tRes;                                                              \
}


#define BINARY_TYPE_OPERATOR(ReturnType, Type1, Type2, Op, OpFunc)            \
    BINARY_TYPE_OPERATOR_SF(ReturnType, Type1, Type2, Op, OpFunc)             \
    BINARY_TYPE_OPERATOR_FS(ReturnType, Type1, Type2, Op, OpFunc)


// ************************************************************************* //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/* * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * */

template<class Type>
void component
(
    Field<typename Field<Type>::cmptType>& res,
    const UList<Type>& f,
    const direction d
)
{
    typedef typename Field<Type>::cmptType cmptType;
    TFOR_ALL_F_OP_F_FUNC_S
    (
        cmptType, res, =, Type, f, .component, const direction, d
    )
}


template<class Type>
void T(Field<Type>& res, const UList<Type>& f)
{
    TFOR_ALL_F_OP_F_FUNC(Type, res, =, Type, f, T)
}


template<class Type, direction r>
void pow
(
    Field<typename powProduct<Type, r>::type>& res,
    const UList<Type>& vf
)
{
    typedef typename powProduct<Type, r>::type powProductType;
    TFOR_ALL_F_OP_FUNC_F_S
    (
        powProductType, res, =, pow, Type, vf, powProductType,
        pTraits<powProductType>::zero
    )
}

template<class Type, direction r>
tmp<Field<typename powProduct<Type, r>::type> >
pow
(
    const UList<Type>& f,
    typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;
    tmp<Field<powProductType> > tRes
    (
        new Field<powProductType>(f.size())
    );
    pow<Type, r>(tRes(), f);
    return tRes;
}

template<class Type, direction r>
tmp<Field<typename powProduct<Type, r>::type> >
pow
(
    const tmp<Field<Type> >& tf,
    typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;
    tmp<Field<powProductType> > tRes = reuseTmp<powProductType, Type>::New(tf);
    pow<Type, r>(tRes(), tf());
    reuseTmp<powProductType, Type>::clear(tf);
    return tRes;
}


template<class Type>
void sqr
(
    Field<typename outerProduct<Type, Type>::type>& res,
    const UList<Type>& vf
)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;
    TFOR_ALL_F_OP_FUNC_F(outerProductType, res, =, sqr, Type, vf)
}

template<class Type>
tmp<Field<typename outerProduct<Type, Type>::type> >
sqr(const UList<Type>& f)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;
    tmp<Field<outerProductType> > tRes
    (
        new Field<outerProductType>(f.size())
    );
    sqr(tRes(), f);
    return tRes;
}

template<class Type>
tmp<Field<typename outerProduct<Type, Type>::type> >
sqr(const tmp<Field<Type> >& tf)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;
    tmp<Field<outerProductType> > tRes =
        reuseTmp<outerProductType, Type>::New(tf);
    sqr(tRes(), tf());
    reuseTmp<outerProductType, Type>::clear(tf);
    return tRes;
}


template<class Type>
void magSqr(Field<scalar>& res, const UList<Type>& f)
{
    TFOR_ALL_F_OP_FUNC_F(scalar, res, =, magSqr, Type, f)
}

template<class Type>
tmp<Field<scalar> > magSqr(const UList<Type>& f)
{
    tmp<Field<scalar> > tRes(new Field<scalar>(f.size()));
    magSqr(tRes(), f);
    return tRes;
}

template<class Type>
tmp<Field<scalar> > magSqr(const tmp<Field<Type> >& tf)
{
    tmp<Field<scalar> > tRes = reuseTmp<scalar, Type>::New(tf);
    magSqr(tRes(), tf());
    reuseTmp<scalar, Type>::clear(tf);
    return tRes;
}


template<class Type>
void mag(Field<scalar>& res, const UList<Type>& f)
{
    TFOR_ALL_F_OP_FUNC_F(scalar, res, =, mag, Type, f)
}

template<class Type>
tmp<Field<scalar> > mag(const UList<Type>& f)
{
    tmp<Field<scalar> > tRes(new Field<scalar>(f.size()));
    mag(tRes(), f);
    return tRes;
}

template<class Type>
tmp<Field<scalar> > mag(const tmp<Field<Type> >& tf)
{
    tmp<Field<scalar> > tRes = reuseTmp<scalar, Type>::New(tf);
    mag(tRes(), tf());
    reuseTmp<scalar, Type>::clear(tf);
    return tRes;
}


template<class Type>
void cmptMax(Field<typename Field<Type>::cmptType>& res, const UList<Type>& f)
{
    typedef typename Field<Type>::cmptType cmptType;
    TFOR_ALL_F_OP_FUNC_F(cmptType, res, =, cmptMax, Type, f)
}

template<class Type>
tmp<Field<typename Field<Type>::cmptType> > cmptMax(const UList<Type>& f)
{
    typedef typename Field<Type>::cmptType cmptType;
    tmp<Field<cmptType> > tRes(new Field<cmptType>(f.size()));
    cmptMax(tRes(), f);
    return tRes;
}

template<class Type>
tmp<Field<typename Field<Type>::cmptType> > cmptMax(const tmp<Field<Type> >& tf)
{
    typedef typename Field<Type>::cmptType cmptType;
    tmp<Field<cmptType> > tRes = reuseTmp<cmptType, Type>::New(tf);
    cmptMax(tRes(), tf());
    reuseTmp<cmptType, Type>::clear(tf);
    return tRes;
}


template<class Type>
void cmptMin(Field<typename Field<Type>::cmptType>& res, const UList<Type>& f)
{
    typedef typename Field<Type>::cmptType cmptType;
    TFOR_ALL_F_OP_FUNC_F(cmptType, res, =, cmptMin, Type, f)
}

template<class Type>
tmp<Field<typename Field<Type>::cmptType> > cmptMin(const UList<Type>& f)
{
    typedef typename Field<Type>::cmptType cmptType;
    tmp<Field<cmptType> > tRes(new Field<cmptType>(f.size()));
    cmptMin(tRes(), f);
    return tRes;
}

template<class Type>
tmp<Field<typename Field<Type>::cmptType> > cmptMin(const tmp<Field<Type> >& tf)
{
    typedef typename Field<Type>::cmptType cmptType;
    tmp<Field<cmptType> > tRes = reuseTmp<cmptType, Type>::New(tf);
    cmptMin(tRes(), tf());
    reuseTmp<cmptType, Type>::clear(tf);
    return tRes;
}


template<class Type>
void cmptAv(Field<typename Field<Type>::cmptType>& res, const UList<Type>& f)
{
    typedef typename Field<Type>::cmptType cmptType;
    TFOR_ALL_F_OP_FUNC_F(cmptType, res, =, cmptAv, Type, f)
}

template<class Type>
tmp<Field<typename Field<Type>::cmptType> > cmptAv(const UList<Type>& f)
{
    typedef typename Field<Type>::cmptType cmptType;
    tmp<Field<cmptType> > tRes(new Field<cmptType>(f.size()));
    cmptAv(tRes(), f);
    return tRes;
}

template<class Type>
tmp<Field<typename Field<Type>::cmptType> > cmptAv(const tmp<Field<Type> >& tf)
{
    typedef typename Field<Type>::cmptType cmptType;
    tmp<Field<cmptType> > tRes = reuseTmp<cmptType, Type>::New(tf);
    cmptAv(tRes(), tf());
    reuseTmp<cmptType, Type>::clear(tf);
    return tRes;
}


template<class Type>
void cmptMag(Field<Type>& res, const UList<Type>& f)
{
    TFOR_ALL_F_OP_FUNC_F(Type, res, =, cmptMag, Type, f)
}

template<class Type>
tmp<Field<Type> > cmptMag(const UList<Type>& f)
{
    tmp<Field<Type> > tRes(new Field<Type>(f.size()));
    cmptMag(tRes(), f);
    return tRes;
}

template<class Type>
tmp<Field<Type> > cmptMag(const tmp<Field<Type> >& tf)
{
    tmp<Field<Type> > tRes = reuseTmp<Type, Type>::New(tf);
    cmptMag(tRes(), tf());
    reuseTmp<Type, Type>::clear(tf);
    return tRes;
}


#define TMP_UNARY_FUNCTION(ReturnType, Func)                                  \
                                                                              \
template<class Type>                                                          \
ReturnType Func(const tmp<Field<Type> >& tf1)                                 \
{                                                                             \
    ReturnType res = Func(tf1());                                             \
    tf1.clear();                                                              \
    return res;                                                               \
}

template<class Type>
Type max(const UList<Type>& f)
{
    if (f.size())
    {
        Type Max(f[0]);
        TFOR_ALL_S_OP_FUNC_F_S(Type, Max, =, max, Type, f, Type, Max)
        return Max;
    }
    else
    {
        return pTraits<Type>::min;
    }
}

TMP_UNARY_FUNCTION(Type, max)

template<class Type>
Type min(const UList<Type>& f)
{
    if (f.size())
    {
        Type Min(f[0]);
        TFOR_ALL_S_OP_FUNC_F_S(Type, Min, =, min, Type, f, Type, Min)
        return Min;
    }
    else
    {
        return pTraits<Type>::max;
    }
}

TMP_UNARY_FUNCTION(Type, min)

template<class Type>
Type sum(const UList<Type>& f)
{
    if (f.size())
    {
        Type Sum = pTraits<Type>::zero;
        TFOR_ALL_S_OP_F(Type, Sum, +=, Type, f)
        return Sum;
    }
    else
    {
        return pTraits<Type>::zero;
    }
}

TMP_UNARY_FUNCTION(Type, sum)


template<class Type>
scalar sumProd(const UList<Type>& f1, const UList<Type>& f2)
{
    if (f1.size() && (f1.size() == f2.size()))
    {
        scalar SumProd = 0.0;
        TFOR_ALL_S_OP_F_OP_F(scalar, SumProd, +=, Type, f1, &&, Type, f2)
        return SumProd;
    }
    else
    {
        return 0.0;
    }
}


template<class Type>
Type sumCmptProd(const UList<Type>& f1, const UList<Type>& f2)
{
    if (f1.size() && (f1.size() == f2.size()))
    {
        Type SumProd = pTraits<Type>::zero;
        TFOR_ALL_S_OP_FUNC_F_F
        (
            Type,
            SumProd,
            +=,
            cmptMultiply,
            Type,
            f1,
            Type,
            f2
        )
        return SumProd;
    }
    else
    {
        return pTraits<Type>::zero;
    }
}


template<class Type>
scalar sumSqr(const UList<Type>& f)
{
    if (f.size())
    {
        scalar SumSqr = 0.0;
        TFOR_ALL_S_OP_FUNC_F(scalar, SumSqr, +=, sqr, Type, f)
        return SumSqr;
    }
    else
    {
        return 0.0;
    }
}

TMP_UNARY_FUNCTION(scalar, sumSqr)

template<class Type>
scalar sumMag(const UList<Type>& f)
{
    if (f.size())
    {
        scalar SumMag = 0.0;
        TFOR_ALL_S_OP_FUNC_F(scalar, SumMag, +=, mag, Type, f)
        return SumMag;
    }
    else
    {
        return 0.0;
    }
}

TMP_UNARY_FUNCTION(scalar, sumMag)


template<class Type>
Type sumCmptMag(const UList<Type>& f)
{
    if (f.size())
    {
        Type SumMag = pTraits<Type>::zero;
        TFOR_ALL_S_OP_FUNC_F(scalar, SumMag, +=, cmptMag, Type, f)
        return SumMag;
    }
    else
    {
        return pTraits<Type>::zero;
    }
}

TMP_UNARY_FUNCTION(Type, sumCmptMag)

template<class Type>
Type average(const UList<Type>& f)
{
    if (f.size())
    {
        Type avrg = sum(f)/f.size();

        return avrg;
    }
    else
    {
        WarningInFunction
            << "empty field, returning zero" << endl;

        return pTraits<Type>::zero;
    }
}

TMP_UNARY_FUNCTION(Type, average)


#define G_UNARY_FUNCTION(ReturnType, gFunc, Func, rFunc)                      \
                                                                              \
template<class Type>                                                          \
ReturnType gFunc(const UList<Type>& f)                                        \
{                                                                             \
    ReturnType res = Func(f);                                                 \
    reduce(res, rFunc##Op<Type>());                                           \
    return res;                                                               \
}                                                                             \
TMP_UNARY_FUNCTION(ReturnType, gFunc)

G_UNARY_FUNCTION(Type, gMax, max, max)
G_UNARY_FUNCTION(Type, gMin, min, min)
G_UNARY_FUNCTION(Type, gSum, sum, sum)
G_UNARY_FUNCTION(scalar, gSumSqr, sumSqr, sum)
G_UNARY_FUNCTION(scalar, gSumMag, sumMag, sum)
G_UNARY_FUNCTION(Type, gSumCmptMag, sumCmptMag, sum)

#undef G_UNARY_FUNCTION

template<class Type>
scalar gSumProd
(
    const UList<Type>& f1,
    const UList<Type>& f2
)
{
    scalar SumProd = sumProd(f1, f2);
    reduce(SumProd, sumOp<scalar>());
    return SumProd;
}

template<class Type>
Type gSumCmptProd
(
    const UList<Type>& f1,
    const UList<Type>& f2
)
{
    Type SumProd = sumCmptProd(f1, f2);
    reduce(SumProd, sumOp<Type>());
    return SumProd;
}

template<class Type>
Type gAverage
(
    const UList<Type>& f
)
{
    label n = f.size();
    reduce(n, sumOp<label>());

    if (n > 0)
    {
        Type avrg = gSum(f)/n;

        return avrg;
    }
    else
    {
        WarningInFunction
            << "empty field, returning zero." << endl;

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

BINARY_OPERATOR(Type, Type, scalar, *, multiply)
BINARY_OPERATOR(Type, scalar, Type, *, multiply)
BINARY_OPERATOR(Type, Type, scalar, /, divide)

BINARY_TYPE_OPERATOR_SF(Type, scalar, Type, *, multiply)
BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, *, multiply)

BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, /, divide)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define PRODUCT_OPERATOR(product, Op, OpFunc)                                 \
                                                                              \
template<class Type1, class Type2>                                            \
void OpFunc                                                                   \
(                                                                             \
    Field<typename product<Type1, Type2>::type>& res,                         \
    const UList<Type1>& f1,                                                   \
    const UList<Type2>& f2                                                    \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
    TFOR_ALL_F_OP_F_OP_F(productType, res, =, Type1, f1, Op, Type2, f2)       \
}                                                                             \
                                                                              \
template<class Type1, class Type2>                                            \
tmp<Field<typename product<Type1, Type2>::type> >                             \
operator Op(const UList<Type1>& f1, const UList<Type2>& f2)                   \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
    tmp<Field<productType> > tRes(new Field<productType>(f1.size()));         \
    OpFunc(tRes(), f1, f2);                                                   \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type1, class Type2>                                            \
tmp<Field<typename product<Type1, Type2>::type> >                             \
operator Op(const UList<Type1>& f1, const tmp<Field<Type2> >& tf2)            \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
    tmp<Field<productType> > tRes = reuseTmp<productType, Type2>::New(tf2);   \
    OpFunc(tRes(), f1, tf2());                                                \
    reuseTmp<productType, Type2>::clear(tf2);                                 \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type1, class Type2>                                            \
tmp<Field<typename product<Type1, Type2>::type> >                             \
operator Op(const tmp<Field<Type1> >& tf1, const UList<Type2>& f2)            \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
    tmp<Field<productType> > tRes = reuseTmp<productType, Type1>::New(tf1);   \
    OpFunc(tRes(), tf1(), f2);                                                \
    reuseTmp<productType, Type1>::clear(tf1);                                 \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type1, class Type2>                                            \
tmp<Field<typename product<Type1, Type2>::type> >                             \
operator Op(const tmp<Field<Type1> >& tf1, const tmp<Field<Type2> >& tf2)     \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
    tmp<Field<productType> > tRes =                                           \
        reuseTmpTmp<productType, Type1, Type1, Type2>::New(tf1, tf2);         \
    OpFunc(tRes(), tf1(), tf2());                                             \
    reuseTmpTmp<productType, Type1, Type1, Type2>::clear(tf1, tf2);           \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, class Form, class Cmpt, direction nCmpt>                 \
void OpFunc                                                                   \
(                                                                             \
    Field<typename product<Type, Form>::type>& res,                           \
    const UList<Type>& f1,                                                    \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                    \
)                                                                             \
{                                                                             \
    typedef typename product<Type, Form>::type productType;                   \
    TFOR_ALL_F_OP_F_OP_S                                                      \
        (productType, res, =,Type, f1, Op, Form, static_cast<const Form&>(vs))\
}                                                                             \
                                                                              \
template<class Type, class Form, class Cmpt, direction nCmpt>                 \
tmp<Field<typename product<Type, Form>::type> >                               \
operator Op(const UList<Type>& f1, const VectorSpace<Form,Cmpt,nCmpt>& vs)    \
{                                                                             \
    typedef typename product<Type, Form>::type productType;                   \
    tmp<Field<productType> > tRes(new Field<productType>(f1.size()));         \
    OpFunc(tRes(), f1, static_cast<const Form&>(vs));                         \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type, class Form, class Cmpt, direction nCmpt>                 \
tmp<Field<typename product<Type, Form>::type> >                               \
operator Op                                                                   \
(                                                                             \
    const tmp<Field<Type> >& tf1,                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                    \
)                                                                             \
{                                                                             \
    typedef typename product<Type, Form>::type productType;                   \
    tmp<Field<productType> > tRes = reuseTmp<productType, Type>::New(tf1);    \
    OpFunc(tRes(), tf1(), static_cast<const Form&>(vs));                      \
    reuseTmp<productType, Type>::clear(tf1);                                  \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Form, class Cmpt, direction nCmpt, class Type>                 \
void OpFunc                                                                   \
(                                                                             \
    Field<typename product<Form, Type>::type>& res,                           \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                   \
    const UList<Type>& f1                                                     \
)                                                                             \
{                                                                             \
    typedef typename product<Form, Type>::type productType;                   \
    TFOR_ALL_F_OP_S_OP_F                                                      \
        (productType, res, =,Form,static_cast<const Form&>(vs), Op, Type, f1) \
}                                                                             \
                                                                              \
template<class Form, class Cmpt, direction nCmpt, class Type>                 \
tmp<Field<typename product<Form, Type>::type> >                               \
operator Op(const VectorSpace<Form,Cmpt,nCmpt>& vs, const UList<Type>& f1)    \
{                                                                             \
    typedef typename product<Form, Type>::type productType;                   \
    tmp<Field<productType> > tRes(new Field<productType>(f1.size()));         \
    OpFunc(tRes(), static_cast<const Form&>(vs), f1);                         \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Form, class Cmpt, direction nCmpt, class Type>                 \
tmp<Field<typename product<Form, Type>::type> >                               \
operator Op                                                                   \
(                                                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& vs, const tmp<Field<Type> >& tf1      \
)                                                                             \
{                                                                             \
    typedef typename product<Form, Type>::type productType;                   \
    tmp<Field<productType> > tRes = reuseTmp<productType, Type>::New(tf1);    \
    OpFunc(tRes(), static_cast<const Form&>(vs), tf1());                      \
    reuseTmp<productType, Type>::clear(tf1);                                  \
    return tRes;                                                              \
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
#endif

// ************************************************************************* //
