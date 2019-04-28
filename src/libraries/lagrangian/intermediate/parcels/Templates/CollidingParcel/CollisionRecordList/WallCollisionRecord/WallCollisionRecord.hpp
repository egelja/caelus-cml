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
    CML::WallCollisionRecord

Description
    Record of a collision between the particle holding the record and
    a wall face at the position relative to the centre of the particle.


\*---------------------------------------------------------------------------*/

#ifndef WallCollisionRecord_H
#define WallCollisionRecord_H

#include "vector.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
template<class Type>
class WallCollisionRecord;

template<class Type>
inline bool operator==
(
    const WallCollisionRecord<Type>&,
    const WallCollisionRecord<Type>&
);

template<class Type>
inline bool operator!=
(
    const WallCollisionRecord<Type>&,
    const WallCollisionRecord<Type>&
);

template<class Type>
Istream& operator>>(Istream&, WallCollisionRecord<Type>&);

template<class Type>
Ostream& operator<<(Ostream&, const WallCollisionRecord<Type>&);


/*---------------------------------------------------------------------------*\
                         Class WallCollisionRecord Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class WallCollisionRecord
{
    // Private data

        // //- Recording whether or not this record has been accessed
        bool accessed_;

        //- The position of wall impact relative to the particle centre
        vector pRel_;

        //- Collision data, stored as if the storing particle was the
        //  first particle (particle A) in the collision.
        Type data_;


public:

    // Static data members

        //- Tolerance for detecting seriously erroneous wall matches
        static const scalar errorCosAngle;


    // Constructors

        //- Construct null
        WallCollisionRecord();

        //- Construct from components
        WallCollisionRecord
        (
            bool accessed,
            const vector& pRel,
            const Type& data = pTraits<Type>::zero
        );

        //- Construct from Istream
        WallCollisionRecord(Istream&);

        //- Construct as copy
        WallCollisionRecord(const WallCollisionRecord&);


    //- Destructor
    ~WallCollisionRecord();


    // Member Functions


        // Access

            //- Return the pRel data
            inline const vector& pRel() const;

            //- Return access to the collision data
            inline const Type& collisionData() const;

            //- Return access to the collision data
            inline Type& collisionData();


        // Check

            inline bool match(const vector& pRel, scalar radius);

            //- Return the accessed status of the record
            inline bool accessed() const;


        // Edit

            //- Set the accessed property of the record to accessed
            inline void setAccessed();

            //- Set the accessed property of the record to unaccessed
            inline void setUnaccessed();


    // Member Operators

        void operator=(const WallCollisionRecord&);


    // Friend Operators

        friend bool operator== <Type>
        (
            const WallCollisionRecord<Type>&,
            const WallCollisionRecord<Type>&
        );

        friend bool operator!= <Type>
        (
            const WallCollisionRecord<Type>&,
            const WallCollisionRecord<Type>&
        );


    // IOstream Operators

        friend Istream& operator>> <Type>
        (
            Istream&,
            WallCollisionRecord<Type>&
        );

        friend Ostream& operator<< <Type>
        (
            Ostream&,
            const WallCollisionRecord<Type>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
inline bool CML::WallCollisionRecord<Type>::match
(
    const vector& pRel,
    scalar radius
)
{
    scalar magpRel_= mag(pRel_);

    scalar magpRel = mag(pRel);

    // Using the new data as the acceptance criterion
    scalar cosAcceptanceAngle = magpRel/radius;

    if (cosAcceptanceAngle > errorCosAngle)
    {
        Info<< "pRel_ " << pRel_ << " " << magpRel_ << nl
            << "pRel " << pRel << " " << magpRel << nl
            << "unit vector dot product " << (pRel & pRel_)/(magpRel_*magpRel)
            << nl << "cosAcceptanceAngle " << cosAcceptanceAngle
            << endl;

        FatalErrorInFunction
            << "Problem with matching WallCollisionRecord." << nl
            << "The given radius, " << radius << ", is smaller than distance "
            << "to the relative position of the WallInteractionSite, "
            << magpRel << nl
            << abort(FatalError);
    }

    // Are the test and recorded pRel (relative position vectors)
    // aligned to within the calculated tolerance?
    bool matched = (pRel & pRel_)/(magpRel_*magpRel) > cosAcceptanceAngle;

    if (matched)
    {
        pRel_ = pRel;
    }

    return matched;
}


template<class Type>
inline const CML::vector&
CML::WallCollisionRecord<Type>::pRel() const
{
    return pRel_;
}


template<class Type>
inline const Type&
CML::WallCollisionRecord<Type>::collisionData() const
{
    return data_;
}


template<class Type>
inline Type& CML::WallCollisionRecord<Type>::collisionData()
{
    return data_;
}


template<class Type>
inline bool CML::WallCollisionRecord<Type>::accessed() const
{
    return accessed_;
}


template<class Type>
inline void CML::WallCollisionRecord<Type>::setAccessed()
{
    accessed_ = true;
}


template<class Type>
inline void CML::WallCollisionRecord<Type>::setUnaccessed()
{
    accessed_ = false;
}


// * * * * * * * * * * * * * * Friend Operators * * * * * * * * * * * * * * //

template<class Type>
inline bool CML::operator==
(
    const WallCollisionRecord<Type>& a,
    const WallCollisionRecord<Type>& b
)
{
    return
    (
        a.accessed_ == b.accessed_
     && a.pRel_ == b.pRel_
     && a.data_ == b.data_
    );
}


template<class Type>
inline bool CML::operator!=
(
    const WallCollisionRecord<Type>& a,
    const WallCollisionRecord<Type>& b
)
{
    return !(a == b);
}


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class Type>
const CML::scalar CML::WallCollisionRecord<Type>::errorCosAngle(1.0 + 1e-6);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::WallCollisionRecord<Type>::WallCollisionRecord()
:
    accessed_(false),
    pRel_(),
    data_(Zero)
{}


template<class Type>
CML::WallCollisionRecord<Type>::WallCollisionRecord
(
    bool accessed,
    const vector& pRel,
    const Type& data
)
:
    accessed_(accessed),
    pRel_(pRel),
    data_(data)
{}


template<class Type>
CML::WallCollisionRecord<Type>::WallCollisionRecord
(
    const WallCollisionRecord<Type>& wCR
)
:
    accessed_(wCR.accessed_),
    pRel_(wCR.pRel_),
    data_(wCR.data_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::WallCollisionRecord<Type>::~WallCollisionRecord()
{}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

template<class Type>
void CML::WallCollisionRecord<Type>::operator=
(
    const WallCollisionRecord<Type>& rhs
)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "Attempted assignment to self"
            << abort(FatalError);
    }

    accessed_ = rhs.accessed_;
    pRel_ = rhs.pRel_;
    data_ = rhs.data_;
}


// * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * * * //
// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::WallCollisionRecord<Type>::WallCollisionRecord(Istream& is)
:
    accessed_(is),
    pRel_(is),
    data_(is)
{
    // Check state of Istream
    is.check
    (
        "CML::WallCollisionRecord<Type>::WallCollisionRecord(CML::Istream&)"
    );
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Istream& CML::operator>>(Istream& is, WallCollisionRecord<Type>& wCR)
{
    is  >> wCR.accessed_ >> wCR.pRel_ >> wCR.data_;

    // Check state of Istream
    is.check
    (
        "CML::Istream&"
        "CML::operator>>(CML::Istream&, CML::WallCollisionRecord<Type>&)"
    );

    return is;
}


template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const WallCollisionRecord<Type>& wCR
)
{
    os  << wCR.accessed_
        << token::SPACE << wCR.pRel_
        << token::SPACE << wCR.data_;

    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, "
        "const CML::WallCollisionRecord<Type>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
