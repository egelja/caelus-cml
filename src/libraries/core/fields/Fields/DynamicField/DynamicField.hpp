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
    CML::DynamicField

Description
    Dynamically sized Field.

SourceFiles
    DynamicFieldI.hpp
    DynamicField.cpp

\*---------------------------------------------------------------------------*/

#ifndef DynamicField_H
#define DynamicField_H

#include "Field.hpp"
#include "StaticAssert.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
class DynamicField;

template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Ostream& operator<<
(
    Ostream&,
    const DynamicField<Type, SizeInc, SizeMult, SizeDiv>&
);

template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Istream& operator>>
(
    Istream&,
    DynamicField<Type, SizeInc, SizeMult, SizeDiv>&
);


/*---------------------------------------------------------------------------*\
                           Class DynamicField Declaration
\*---------------------------------------------------------------------------*/

template<class Type, unsigned SizeInc=0, unsigned SizeMult=2, unsigned SizeDiv=1>
class DynamicField
:
    public Field<Type>
{
    //- Avoid invalid sizing parameters
    StaticAssert((SizeInc || SizeMult) && SizeDiv);

    // Private data

        //- The capacity (allocated size) of the underlying field.
        label capacity_;


public:

    // Static Member Functions

        //- Return a null field
        inline static const DynamicField<Type, SizeInc, SizeMult, SizeDiv>& null()
        {
            return *reinterpret_cast
            <
                DynamicField<Type, SizeInc, SizeMult, SizeDiv>*
            >(0);
        }


    // Constructors

        //- Construct null
        inline DynamicField();

        //- Construct given size.
        explicit inline DynamicField(const label);

        //- Construct from UList. Size set to UList size.
        //  Also constructs from DynamicField with different sizing parameters.
        explicit inline DynamicField(const UList<Type>&);

        //- Construct by transferring the parameter contents
        explicit inline DynamicField(const Xfer<List<Type> >&);

        //- Construct by 1 to 1 mapping from the given field
        inline DynamicField
        (
            const UList<Type>& mapF,
            const labelList& mapAddressing
        );

        //- Construct by interpolative mapping from the given field
        inline DynamicField
        (
            const UList<Type>& mapF,
            const labelListList& mapAddressing,
            const scalarListList& weights
        );

        //- Construct by mapping from the given field
        inline DynamicField
        (
            const UList<Type>& mapF,
            const FieldMapper& map
        );

        //- Construct copy
        inline DynamicField(const DynamicField<Type, SizeInc, SizeMult, SizeDiv>&);

        //- Construct by transferring the Field contents
        inline DynamicField
        (
            const Xfer<DynamicField<Type, SizeInc, SizeMult, SizeDiv> >&
        );

        //- Construct from Istream. Size set to size of list read.
        explicit DynamicField(Istream&);

        //- Clone
        tmp<DynamicField<Type, SizeInc, SizeMult, SizeDiv> > clone() const;


    // Member Functions

        // Access

            //- Size of the underlying storage.
            inline label capacity() const;

        // Edit

            //- Alter the size of the underlying storage.
            //  The addressed size will be truncated if needed to fit, but will
            //  remain otherwise untouched.
            //  Use this or reserve() in combination with append().
            inline void setCapacity(const label);

            //- Alter the addressed list size.
            //  New space will be allocated if required.
            //  Use this to resize the list prior to using the operator[] for
            //  setting values (as per List usage).
            inline void setSize(const label);

            //- Alter the addressed list size and fill new space with a
            //  constant.
            inline void setSize(const label, const Type&);

            //- Alter the addressed list size.
            //  New space will be allocated if required.
            //  Use this to resize the list prior to using the operator[] for
            //  setting values (as per List usage).
            inline void resize(const label);

            //- Alter the addressed list size and fill new space with a
            //  constant.
            inline void resize(const label, const Type&);

            //- Reserve allocation space for at least this size.
            //  Never shrinks the allocated size, use setCapacity() for that.
            inline void reserve(const label);

            //- Clear the addressed list, i.e. set the size to zero.
            //  Allocated size does not change
            inline void clear();

            //- Clear the list and delete storage.
            inline void clearStorage();

            //- Shrink the allocated space to the number of elements used.
            //  Returns a reference to the DynamicField.
            inline DynamicField<Type, SizeInc, SizeMult, SizeDiv>& shrink();

            //- Transfer contents to the Xfer container as a plain List
            inline Xfer<List<Type> > xfer();


        // Member Operators

            //- Append an element at the end of the list
            inline DynamicField<Type, SizeInc, SizeMult, SizeDiv>& append
            (
                const Type&
            );

            //- Append a List at the end of this list
            inline DynamicField<Type, SizeInc, SizeMult, SizeDiv>& append
            (
                const UList<Type>&
            );

            //- Remove and return the top element
            inline Type remove();

            //- Return non-const access to an element, resizing list if
            //  necessary
            inline Type& operator()(const label);

            //- Assignment of all addressed entries to the given value
            inline void operator=(const Type&);

            //- Assignment from DynamicField
            inline void operator=
            (
                const DynamicField<Type, SizeInc, SizeMult, SizeDiv>&
            );

            //- Assignment from UList
            inline void operator=(const UList<Type>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField()
:
    Field<Type>(0),
    capacity_(Field<Type>::size())
{}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField
(
    const label nElem
)
:
    Field<Type>(nElem),
    capacity_(Field<Type>::size())
{
    // we could also enforce SizeInc granularity when (!SizeMult || !SizeDiv)
    Field<Type>::size(0);
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField
(
    const UList<Type>& lst
)
:
    Field<Type>(lst),
    capacity_(Field<Type>::size())
{}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField
(
    const Xfer<List<Type> >& lst
)
:
    Field<Type>(lst),
    capacity_(Field<Type>::size())
{}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField
(
    const UList<Type>& mapF,
    const labelList& mapAddressing
)
:
    Field<Type>(mapF, mapAddressing),
    capacity_(Field<Type>::size())
{}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField
(
    const UList<Type>& mapF,
    const labelListList& mapAddressing,
    const scalarListList& weights
)
:
    Field<Type>(mapF, mapAddressing, weights),
    capacity_(Field<Type>::size())
{}


//- Construct by mapping from the given field
template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField
(
    const UList<Type>& mapF,
    const FieldMapper& map
)
:
    Field<Type>(mapF, map),
    capacity_(Field<Type>::size())
{}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField
(
    const DynamicField<Type, SizeInc, SizeMult, SizeDiv>& lst
)
:
    Field<Type>(lst),
    capacity_(lst.capacity())
{}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField
(
    const Xfer<DynamicField<Type, SizeInc, SizeMult, SizeDiv> >& lst
)
:
    Field<Type>(lst),
    capacity_(Field<Type>::size())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::label CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::capacity()
const
{
    return capacity_;
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::setCapacity
(
    const label nElem
)
{
    label nextFree = Field<Type>::size();
    capacity_ = nElem;

    if (nextFree > capacity_)
    {
        // truncate addressed sizes too
        nextFree = capacity_;
    }
    // we could also enforce SizeInc granularity when (!SizeMult || !SizeDiv)

    Field<Type>::setSize(capacity_);
    Field<Type>::size(nextFree);
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::reserve
(
    const label nElem
)
{
    // allocate more capacity?
    if (nElem > capacity_)
    {
// TODO: convince the compiler that division by zero does not occur
//        if (SizeInc && (!SizeMult || !SizeDiv))
//        {
//            // resize with SizeInc as the granularity
//            capacity_ = nElem;
//            unsigned pad = SizeInc - (capacity_ % SizeInc);
//            if (pad != SizeInc)
//            {
//                capacity_ += pad;
//            }
//        }
//        else
        {
            capacity_ = max
            (
                nElem,
                label(SizeInc + capacity_ * SizeMult / SizeDiv)
            );
        }

        // adjust allocated size, leave addressed size untouched
        label nextFree = Field<Type>::size();
        Field<Type>::setSize(capacity_);
        Field<Type>::size(nextFree);
    }
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::setSize
(
    const label nElem
)
{
    // allocate more capacity?
    if (nElem > capacity_)
    {
// TODO: convince the compiler that division by zero does not occur
//        if (SizeInc && (!SizeMult || !SizeDiv))
//        {
//            // resize with SizeInc as the granularity
//            capacity_ = nElem;
//            unsigned pad = SizeInc - (capacity_ % SizeInc);
//            if (pad != SizeInc)
//            {
//                capacity_ += pad;
//            }
//        }
//        else
        {
            capacity_ = max
            (
                nElem,
                label(SizeInc + capacity_ * SizeMult / SizeDiv)
            );
        }

        Field<Type>::setSize(capacity_);
    }

    // adjust addressed size
    Field<Type>::size(nElem);
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::setSize
(
    const label nElem,
    const Type& t
)
{
    label nextFree = Field<Type>::size();
    setSize(nElem);

    // set new elements to constant value
    while (nextFree < nElem)
    {
        this->operator[](nextFree++) = t;
    }
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::resize
(
    const label nElem
)
{
    this->setSize(nElem);
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::resize
(
    const label nElem,
    const Type& t
)
{
    this->setSize(nElem, t);
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::clear()
{
    Field<Type>::size(0);
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::clearStorage()
{
    Field<Type>::clear();
    capacity_ = 0;
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>&
CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::shrink()
{
    label nextFree = Field<Type>::size();
    if (capacity_ > nextFree)
    {
        // use the full list when resizing
        Field<Type>::size(capacity_);

        // the new size
        capacity_ = nextFree;
        Field<Type>::setSize(capacity_);
        Field<Type>::size(nextFree);
    }
    return *this;
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::Xfer<CML::List<Type> >
CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::xfer()
{
    return xferMoveTo< List<Type> >(*this);
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>&
CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::append
(
    const Type& t
)
{
    const label elemI = List<Type>::size();
    setSize(elemI + 1);

    this->operator[](elemI) = t;
    return *this;
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>&
CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::append
(
    const UList<Type>& lst
)
{
    if (this == &lst)
    {
        FatalErrorInFunction
            << "attempted appending to self" << abort(FatalError);
    }

    label nextFree = List<Type>::size();
    setSize(nextFree + lst.size());

    forAll(lst, elemI)
    {
        this->operator[](nextFree++) = lst[elemI];
    }
    return *this;
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline Type CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::remove()
{
    const label elemI = List<Type>::size() - 1;

    if (elemI < 0)
    {
        FatalErrorInFunction
            << "List is empty" << abort(FatalError);
    }

    const Type& val = List<Type>::operator[](elemI);

    List<Type>::size(elemI);

    return val;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline Type& CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::operator()
(
    const label elemI
)
{
    if (elemI >= Field<Type>::size())
    {
        setSize(elemI + 1);
    }

    return this->operator[](elemI);
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::operator=
(
    const Type& t
)
{
    UList<Type>::operator=(t);
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::operator=
(
    const DynamicField<Type, SizeInc, SizeMult, SizeDiv>& lst
)
{
    if (this == &lst)
    {
        FatalErrorInFunction
            << "attempted assignment to self" << abort(FatalError);
    }

    if (capacity_ >= lst.size())
    {
        // can copy w/o reallocating, match initial size to avoid reallocation
        Field<Type>::size(lst.size());
        Field<Type>::operator=(lst);
    }
    else
    {
        // make everything available for the copy operation
        Field<Type>::size(capacity_);

        Field<Type>::operator=(lst);
        capacity_ = Field<Type>::size();
    }
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::operator=
(
    const UList<Type>& lst
)
{
    if (capacity_ >= lst.size())
    {
        // can copy w/o reallocating, match initial size to avoid reallocation
        Field<Type>::size(lst.size());
        Field<Type>::operator=(lst);
    }
    else
    {
        // make everything available for the copy operation
        Field<Type>::size(capacity_);

        Field<Type>::operator=(lst);
        capacity_ = Field<Type>::size();
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::DynamicField(Istream& is)
:
    Field<Type>(is),
    capacity_(Field<Type>::size())
{}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
CML::tmp<CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv> >
CML::DynamicField<Type, SizeInc, SizeMult, SizeDiv>::clone() const
{
    return tmp<DynamicField<Type, SizeInc, SizeMult, SizeDiv> >
    (
        new DynamicField<Type, SizeInc, SizeMult, SizeDiv>(*this)
    );
}


// * * * * * * * * * * * * * * * IOstream Operator * * * * * * * * * * * * * //

template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const DynamicField<Type, SizeInc, SizeMult, SizeDiv>& lst
)
{
    os << static_cast<const Field<Type>&>(lst);
    return os;
}


template<class Type, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
CML::Istream& CML::operator>>
(
    Istream& is,
    DynamicField<Type, SizeInc, SizeMult, SizeDiv>& lst
)
{
    is >> static_cast<Field<Type>&>(lst);
    lst.capacity_ = lst.Field<Type>::size();

    return is;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
