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
    CML::CollisionRecordList

Description


\*---------------------------------------------------------------------------*/

#ifndef CollisionRecordList_H
#define CollisionRecordList_H

#include "DynamicList.hpp"
#include "PairCollisionRecord.hpp"
#include "WallCollisionRecord.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
template<class PairType, class WallType>
class CollisionRecordList;

template<class PairType, class WallType>
inline bool operator==
(
    const CollisionRecordList<PairType, WallType>&,
    const CollisionRecordList<PairType, WallType>&
);

template<class PairType, class WallType>
inline bool operator!=
(
    const CollisionRecordList<PairType, WallType>&,
    const CollisionRecordList<PairType, WallType>&
);

template<class PairType, class WallType>
Istream& operator>>(Istream&, CollisionRecordList<PairType, WallType>&);

template<class PairType, class WallType>
Ostream& operator<<(Ostream&, const CollisionRecordList<PairType, WallType>&);


/*---------------------------------------------------------------------------*\
                     Class CollisionRecordList Declaration
\*---------------------------------------------------------------------------*/

template<class PairType, class WallType>
class CollisionRecordList
{
    // Private data

        //- List of active pair collisions
        DynamicList<PairCollisionRecord<PairType> > pairRecords_;

        //- List of active wall collisions
        DynamicList<WallCollisionRecord<WallType> > wallRecords_;


public:

    // Constructors

        //- Construct null
        CollisionRecordList();

        //- Construct from Istream
        CollisionRecordList(Istream&);

        //- Construct from component fields (for IO)
        CollisionRecordList
        (
            const labelField& pairAccessed,
            const labelField& pairOrigProcOfOther,
            const labelField& pairOrigIdOfOther,
            const Field<PairType>& pairData,
            const labelField& wallAccessed,
            const vectorField& wallPRel,
            const Field<WallType>& wallData
        );

    //- Destructor
    ~CollisionRecordList();


    // Member Functions

        //- Return the active pair collisions
        inline const DynamicList<PairCollisionRecord<PairType> >&
            pairRecords() const;

        //- Return the active wall collisions
        inline const DynamicList<WallCollisionRecord<WallType> >&
            wallRecords() const;

        // Fields representing the data from each record, i.e if the
        // records 0-N containing each data members {a, b, c, d...}
        // are organised:
        //
        // a0 b0 c0 d0 ...
        // a1 b1 c1 d1 ...
        // a2 b2 c2 d2 ...
        // ...
        // aN bN cN dN ...
        //
        // Then these field return, for example, (c0, c1, c2,... cN)

        //- Return field of pair accessed from each record, used for
        //  field IO
        labelField pairAccessed() const;

        //- Return field of pair origProcOfOther from each record,
        //  used for field IO
        labelField pairOrigProcOfOther() const;

        //- Return field of pair origIdOfOther from each record, used
        //  for field IO
        labelField pairOrigIdOfOther() const;

        //- Return field of pair data from each record, used for field IO
        Field<PairType> pairData() const;

        //- Return field of wall accessed from each record, used for field IO
        labelField wallAccessed() const;

        //- Return field of wall pRel from each record, used for field IO
        vectorField wallPRel() const;

        //- Return field of wall data from each record, used for field IO
        Field<WallType> wallData() const;

        //- Enquires if the proc and id pair of the other particle are
        //  present in the records.  If so, return non-const access to
        //  the PairCollisionRecord (hence the data) and mark the
        //  PairCollisionRecord as accessed this step, if not, create
        //  the record and return access to it.
        PairCollisionRecord<PairType>& matchPairRecord
        (
            label origProcOfOther,
            label origIdOfOther
        );

        //- Enquire if the specified record exists without modifying
        //  its accessed status
        bool checkPairRecord(label origProcOfOther, label origIdOfOther);

        //- Enquires if the position of wall impact relative to the
        //  particle centre is present in the records.  If so, return
        //  access to the WallCollisionRecord (hence the data) and
        //  mark the WallCollisionRecord as accesses this step, if
        //  not, create the record and return access to it.
        WallCollisionRecord<WallType>& matchWallRecord
        (
            const vector& pRel,
            scalar radius
        );

        //- Enquire if the specified record exists without modifying
        //  its accessed status
        bool checkWallRecord(const vector& pRel, scalar radius);

        //- Update the collision records, deleting any records not
        //  marked as having been accessed, then mark all records as
        //  not accessed ready for the next evaluation
        void update();


        // Member Operators

            void operator=(const CollisionRecordList&);


        // Friend Operators

            friend bool operator== <PairType, WallType>
            (
                const CollisionRecordList<PairType, WallType>&,
                const CollisionRecordList<PairType, WallType>&
            );

            friend bool operator!= <PairType, WallType>
            (
                const CollisionRecordList<PairType, WallType>&,
                const CollisionRecordList<PairType, WallType>&
            );


        // IOstream Operators

            friend Istream& operator>> <PairType, WallType>
            (
                Istream&,
                CollisionRecordList<PairType, WallType>&
            );

            friend Ostream& operator<< <PairType, WallType>
            (
                Ostream&,
                const CollisionRecordList<PairType, WallType>&
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class PairType, class WallType>
const CML::DynamicList<CML::PairCollisionRecord<PairType> >&
CML::CollisionRecordList<PairType, WallType>::pairRecords() const
{
    return pairRecords_;
}


template<class PairType, class WallType>
const CML::DynamicList<CML::WallCollisionRecord<WallType> >&
CML::CollisionRecordList<PairType, WallType>::wallRecords() const
{
    return wallRecords_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class PairType, class WallType>
CML::CollisionRecordList<PairType, WallType>::CollisionRecordList()
:
    pairRecords_(),
    wallRecords_()
{}


template<class PairType, class WallType>
CML::CollisionRecordList<PairType, WallType>::CollisionRecordList(Istream& is)
:
    pairRecords_(is),
    wallRecords_(is)
{
    // Check state of Istream
    is.check
    (
        "CML::CollisionRecordList<PairType, WallType>::"
        "CollisionRecordList(CML::Istream&)"
    );
}


template<class PairType, class WallType>
CML::CollisionRecordList<PairType, WallType>::CollisionRecordList
(
    const labelField& pairAccessed,
    const labelField& pairOrigProcOfOther,
    const labelField& pairOrigIdOfOther,
    const Field<PairType>& pairData,
    const labelField& wallAccessed,
    const vectorField& wallPRel,
    const Field<WallType>& wallData
)
:
    pairRecords_(),
    wallRecords_()
{
    label nPair = pairAccessed.size();

    if
    (
        pairOrigProcOfOther.size() != nPair
     || pairOrigIdOfOther.size() != nPair
     || pairData.size() != nPair
    )
    {
        FatalErrorInFunction
            << "Pair field size mismatch." << nl
            << pairAccessed << nl
            << pairOrigProcOfOther << nl
            << pairOrigIdOfOther << nl
            << pairData << nl
            << abort(FatalError);
    }

    forAll(pairAccessed, i)
    {
        pairRecords_.append
        (
            PairCollisionRecord<PairType>
            (
                pairAccessed[i],
                pairOrigProcOfOther[i],
                pairOrigIdOfOther[i],
                pairData[i]
            )
        );
    }

    label nWall = wallAccessed.size();

    if (wallPRel.size() != nWall || wallData.size() != nWall)
    {
        FatalErrorInFunction
            << "Wall field size mismatch." << nl
            << wallAccessed << nl
            << wallPRel << nl
            << wallData << nl
            << abort(FatalError);
    }

    forAll(wallAccessed, i)
    {
        wallRecords_.append
        (
            WallCollisionRecord<WallType>
            (
                wallAccessed[i],
                wallPRel[i],
                wallData[i]
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * /

template<class PairType, class WallType>
CML::CollisionRecordList<PairType, WallType>::~CollisionRecordList()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class PairType, class WallType>
CML::labelField
CML::CollisionRecordList<PairType, WallType>::pairAccessed() const
{
    labelField f(pairRecords_.size());

    forAll(pairRecords_, i)
    {
        f[i] = pairRecords_[i].accessed();
    }

    return f;
}


template<class PairType, class WallType>
CML::labelField
CML::CollisionRecordList<PairType, WallType>::pairOrigProcOfOther() const
{
    labelField f(pairRecords_.size());

    forAll(pairRecords_, i)
    {
        f[i] = pairRecords_[i].origProcOfOther();
    }

    return f;
}


template<class PairType, class WallType>
CML::labelField
CML::CollisionRecordList<PairType, WallType>::pairOrigIdOfOther() const
{
    labelField f(pairRecords_.size());

    forAll(pairRecords_, i)
    {
        f[i] = pairRecords_[i].origIdOfOther();
    }

    return f;
}


template<class PairType, class WallType>
CML::Field<PairType>
CML::CollisionRecordList<PairType, WallType>::pairData() const
{
    Field<PairType> f(pairRecords_.size());

    forAll(pairRecords_, i)
    {
        f[i] = pairRecords_[i].collisionData();
    }

    return f;
}


template<class PairType, class WallType>
CML::labelField
CML::CollisionRecordList<PairType, WallType>::wallAccessed() const
{
    labelField f(wallRecords_.size());

    forAll(wallRecords_, i)
    {
        f[i] = wallRecords_[i].accessed();
    }

    return f;
}


template<class PairType, class WallType>
CML::vectorField
CML::CollisionRecordList<PairType, WallType>::wallPRel() const
{
    vectorField f(wallRecords_.size());

    forAll(wallRecords_, i)
    {
        f[i] = wallRecords_[i].pRel();
    }

    return f;
}


template<class PairType, class WallType>
CML::Field<WallType>
CML::CollisionRecordList<PairType, WallType>::wallData() const
{
    Field<WallType> f(wallRecords_.size());

    forAll(wallRecords_, i)
    {
        f[i] = wallRecords_[i].collisionData();
    }

    return f;
}


template<class PairType, class WallType>
CML::PairCollisionRecord<PairType>&
CML::CollisionRecordList<PairType, WallType>::matchPairRecord
(
    label origProcOfOther,
    label origIdOfOther
)
{
    // Returning the first record that matches the particle
    // identifiers.  Two records with the same identification is not
    // supported.

    forAll(pairRecords_, i)
    {
        PairCollisionRecord<PairType>& pCR = pairRecords_[i];

        if (pCR.match(origProcOfOther, origIdOfOther))
        {
            pCR.setAccessed();

            return pCR;
        }
    }

    // Record not found, create a new one and return it as the last
    // member of the list.  Setting the status of the record to be accessed
    // on construction.

    pairRecords_.append
    (
        PairCollisionRecord<PairType>(true, origProcOfOther, origIdOfOther)
    );

    return pairRecords_.last();
}


template<class PairType, class WallType>
bool CML::CollisionRecordList<PairType, WallType>::checkPairRecord
(
    label origProcOfOther,
    label origIdOfOther
)
{
    forAll(pairRecords_, i)
    {
        PairCollisionRecord<PairType>& pCR = pairRecords_[i];

        if (pCR.match(origProcOfOther, origIdOfOther))
        {
            return true;
        }
    }

    return false;
}


template<class PairType, class WallType>
CML::WallCollisionRecord<WallType>&
CML::CollisionRecordList<PairType, WallType>::matchWallRecord
(
    const vector& pRel,
    scalar radius
)
{
    // Returning the first record that matches the relative position.
    // Two records with the same relative position is not supported.

    forAll(wallRecords_, i)
    {
        WallCollisionRecord<WallType>& wCR = wallRecords_[i];

        if (wCR.match(pRel, radius))
        {
            wCR.setAccessed();

            return wCR;
        }
    }

    // Record not found, create a new one and return it as the last
    // member of the list.  Setting the status of the record to be accessed
    // on construction.

    wallRecords_.append(WallCollisionRecord<WallType>(true, pRel));

    return wallRecords_.last();
}


template<class PairType, class WallType>
bool CML::CollisionRecordList<PairType, WallType>::checkWallRecord
(
    const vector& pRel,
    scalar radius
)
{
    forAll(wallRecords_, i)
    {
        WallCollisionRecord<WallType>& wCR = wallRecords_[i];

        if (wCR.match(pRel, radius))
        {
            return true;
        }
    }

    return false;
}


template<class PairType, class WallType>
void CML::CollisionRecordList<PairType, WallType>::update()
{
    {
        DynamicList<PairCollisionRecord<PairType> > updatedRecords;

        forAll(pairRecords_, i)
        {
            if (pairRecords_[i].accessed())
            {
                pairRecords_[i].setUnaccessed();

                updatedRecords.append(pairRecords_[i]);
            }
        }

        pairRecords_ = updatedRecords;
    }

    {
        DynamicList<WallCollisionRecord<WallType> > updatedRecords;

        forAll(wallRecords_, i)
        {
            if (wallRecords_[i].accessed())
            {
                wallRecords_[i].setUnaccessed();

                updatedRecords.append(wallRecords_[i]);
            }
        }

        wallRecords_ = updatedRecords;
    }
}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

template<class PairType, class WallType>
void CML::CollisionRecordList<PairType, WallType>::operator=
(
    const CollisionRecordList<PairType, WallType>& rhs
)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "Attempted assignment to self"
            << abort(FatalError);
    }

    pairRecords_ = rhs.pairRecords_;
    wallRecords_ = rhs.wallRecords_;
}


// * * * * * * * * * * * * * * Friend Operators * * * * * * * * * * * * * * //

template<class PairType, class WallType>
inline bool CML::operator==
(
    const CollisionRecordList<PairType, WallType>& a,
    const CollisionRecordList<PairType, WallType>& b
)
{
    return
    (
        a.pairRecords_ == b.pairRecords_
     && a.wallRecords_ == b.wallRecords_
    );
}


template<class PairType, class WallType>
inline bool CML::operator!=
(
    const CollisionRecordList<PairType, WallType>& a,
    const CollisionRecordList<PairType, WallType>& b
)
{
    return !(a == b);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class PairType, class WallType>
CML::Istream& CML::operator>>
(
    Istream& is,
    CollisionRecordList<PairType, WallType>& cRL
)
{
    is  >> cRL.pairRecords_ >> cRL.wallRecords_;

    // Check state of Istream
    is.check
    (
        "CML::Istream& CML::operator>>"
        "(CML::Istream&, CML::CollisionRecordList<PairType, WallType>&)"
    );

    return is;
}


template<class PairType, class WallType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const CollisionRecordList<PairType, WallType>& cRL
)
{
    os  << cRL.pairRecords_ << cRL.wallRecords_;

    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, "
        "const CML::CollisionRecordList<PairType, WallType>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
