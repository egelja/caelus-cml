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
    CML::PairCollisionRecord

Description

    Record of a collision between the particle holding the record and
    the particle with the stored id.

    The access status of the record is to be coded in the
    origProcOfOther data member.  The actual processor is offset by
    +1.  A negative value means that the record has not been accessed,
    positive means that it has.


\*---------------------------------------------------------------------------*/

#ifndef PairCollisionRecord_H
#define PairCollisionRecord_H

#include "label.hpp"
#include "vector.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
template<class Type>
class PairCollisionRecord;

template<class Type>
inline bool operator==
(
    const PairCollisionRecord<Type>&,
    const PairCollisionRecord<Type>&
);

template<class Type>
inline bool operator!=
(
    const PairCollisionRecord<Type>&,
    const PairCollisionRecord<Type>&
);

template<class Type>
Istream& operator>>(Istream&, PairCollisionRecord<Type>&);

template<class Type>
Ostream& operator<<(Ostream&, const PairCollisionRecord<Type>&);


/*---------------------------------------------------------------------------*\
                         Class PairCollisionRecord Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class PairCollisionRecord
{
    // Private data

        //- Originating processor id of other collision partner
        label origProcOfOther_;

        //- Local particle id on originating processor of other
        //  collision partner
        label origIdOfOther_;

        //- Collision data, stored as if the storing particle was the
        //  first particle (particle A) in the collision.
        Type data_;


public:

    // Constructors

        //- Construct null
        PairCollisionRecord();

        //- Construct from components
        PairCollisionRecord
        (
            bool accessed,
            label origProcOfOther,
            label origIdOfOther,
            const Type& data = pTraits<Type>::zero
        );

        //- Construct from Istream
        PairCollisionRecord(Istream&);

        //- Construct as copy
        PairCollisionRecord(const PairCollisionRecord&);


    //- Destructor
        ~PairCollisionRecord();


    // Member Functions


        // Access

            //- Return the origProcOfOther data
            inline label origProcOfOther() const;

            //- Return the origIdOfOther data
            inline label origIdOfOther() const;

            //- Return access to the collision data
            inline const Type& collisionData() const;

            //- Return access to the collision data
            inline Type& collisionData();


        // Check

            inline bool match
            (
                label queryOrigProcOfOther,
                label queryOrigIdOfOther
            ) const;

            //- Return the accessed status of the record
            inline bool accessed() const;


        // Edit

            //- Set the accessed property of the record to accessed
            inline void setAccessed();

            //- Set the accessed property of the record to unaccessed
            inline void setUnaccessed();


    // Member Operators

        void operator=(const PairCollisionRecord&);


    // Friend Operators

        friend bool operator== <Type>
        (
            const PairCollisionRecord<Type>&,
            const PairCollisionRecord<Type>&
        );

        friend bool operator!= <Type>
        (
            const PairCollisionRecord<Type>&,
            const PairCollisionRecord<Type>&
        );


    // IOstream Operators

        friend Istream& operator>> <Type>
        (
            Istream&,
            PairCollisionRecord<Type>&
        );

        friend Ostream& operator<< <Type>
        (
            Ostream&,
            const PairCollisionRecord<Type>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
inline bool CML::PairCollisionRecord<Type>::match
(
    label queryOrigProcOfOther,
    label queryOrigIdOfOther
) const
{
    return
    (
        queryOrigProcOfOther == origProcOfOther()
     && queryOrigIdOfOther == origIdOfOther()
    );
}


template<class Type>
inline CML::label CML::PairCollisionRecord<Type>::origProcOfOther() const
{
    return mag(origProcOfOther_) - 1;
}


template<class Type>
inline CML::label CML::PairCollisionRecord<Type>::origIdOfOther() const
{
    return origIdOfOther_;
}


template<class Type>
inline const Type&
CML::PairCollisionRecord<Type>::collisionData() const
{
    return data_;
}


template<class Type>
inline Type& CML::PairCollisionRecord<Type>::collisionData()
{
    return data_;
}


template<class Type>
inline bool CML::PairCollisionRecord<Type>::accessed() const
{
    return pos(origProcOfOther_);
}


template<class Type>
inline void CML::PairCollisionRecord<Type>::setAccessed()
{
    origProcOfOther_ = origProcOfOther() + 1;
}


template<class Type>
inline void CML::PairCollisionRecord<Type>::setUnaccessed()
{
    origProcOfOther_ = -(origProcOfOther() + 1);
}


// * * * * * * * * * * * * * * Friend Operators * * * * * * * * * * * * * * //

template<class Type>
inline bool CML::operator==
(
    const PairCollisionRecord<Type>& a,
    const PairCollisionRecord<Type>& b
)
{
    return
    (
        a.origProcOfOther_ == b.origProcOfOther_
     && a.origIdOfOther_ == b.origIdOfOther_
     && a.data_ == b.data_
    );
}


template<class Type>
inline bool CML::operator!=
(
    const PairCollisionRecord<Type>& a,
    const PairCollisionRecord<Type>& b
)
{
    return !(a == b);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::PairCollisionRecord<Type>::PairCollisionRecord()
:
    origProcOfOther_(0),
    origIdOfOther_(-1),
    data_(Zero)
{}


template<class Type>
CML::PairCollisionRecord<Type>::PairCollisionRecord
(
    bool accessed,
    label origProcOfOther,
    label origIdOfOther,
    const Type& data
)
:
    origProcOfOther_(origProcOfOther + 1),
    origIdOfOther_(origIdOfOther),
    data_(data)
{
    // Default assignment to origProcOfOther_ assumes accessed is true

    if (!accessed)
    {
        setUnaccessed();
    }
}


template<class Type>
CML::PairCollisionRecord<Type>::PairCollisionRecord
(
    const PairCollisionRecord<Type>& pCR
)
:
    origProcOfOther_(pCR.origProcOfOther_),
    origIdOfOther_(pCR.origIdOfOther_),
    data_(pCR.data_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::PairCollisionRecord<Type>::~PairCollisionRecord()
{}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

template<class Type>
void CML::PairCollisionRecord<Type>::operator=
(
    const PairCollisionRecord<Type>& rhs
)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "Attempted assignment to self"
            << abort(FatalError);
    }

    origProcOfOther_ = rhs.origProcOfOther_;
    origIdOfOther_ = rhs.origIdOfOther_;
    data_ = rhs.data_;
}


// * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * * * //
// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::PairCollisionRecord<Type>::PairCollisionRecord(Istream& is)
:
    origProcOfOther_(readLabel(is)),
    origIdOfOther_(readLabel(is)),
    data_(is)
{
    // Check state of Istream
    is.check
    (
        "CML::PairCollisionRecord<Type>::PairCollisionRecord(CML::Istream&)"
    );
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Istream& CML::operator>>(Istream& is, PairCollisionRecord<Type>& pCR)
{
    is  >> pCR.origProcOfOther_ >> pCR.origIdOfOther_ >> pCR.data_;

    // Check state of Istream
    is.check
    (
        "CML::Istream&"
        "CML::operator>>(CML::Istream&, CML::PairCollisionRecord<Type>&)"
    );

    return is;
}


template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const PairCollisionRecord<Type>& pCR
)
{
    os  << pCR.origProcOfOther_
        << token::SPACE << pCR.origIdOfOther_
        << token::SPACE << pCR.data_;

    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, "
        "const CML::PairCollisionRecord<Type>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
