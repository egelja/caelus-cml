/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::CollidingParcel

Description
    Wrapper around kinematic parcel types to add collision modelling


\*---------------------------------------------------------------------------*/

#ifndef CollidingParcel_H
#define CollidingParcel_H

#include "particle.hpp"

#include "CollisionRecordList.hpp"
#include "labelFieldIOField.hpp"
#include "vectorFieldIOField.hpp"
#include "IOstreams.hpp"
#include "IOField.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

typedef CollisionRecordList<vector, vector> collisionRecordList;
typedef vectorFieldCompactIOField pairDataFieldCompactIOField;
typedef vectorFieldCompactIOField wallDataFieldCompactIOField;

template<class ParcelType>
class CollidingParcel;

// Forward declaration of friend functions

template<class ParcelType>
Ostream& operator<<
(
    Ostream&,
    const CollidingParcel<ParcelType>&
);

/*---------------------------------------------------------------------------*\
                         Class CollidingParcel Declaration
\*---------------------------------------------------------------------------*/

template<class ParcelType>
class CollidingParcel
:
    public ParcelType
{
protected:

    // Protected data

        //- Particle collision records
        collisionRecordList collisionRecords_;


public:

    // Static data members

        //- String representation of properties
        static string propHeader;

        //- Runtime type information
        TypeName("CollidingParcel");


    // Constructors

        //- Construct from owner, position, and cloud owner
        //  Other properties initialised as null
        inline CollidingParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            const label tetFaceI,
            const label tetPtI
        );

        //- Construct from components
        inline CollidingParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            const label tetFaceI,
            const label tetPtI,
            const label typeId,
            const scalar nParticle0,
            const scalar d0,
            const scalar dTarget0,
            const vector& U0,
            const vector& f0,
            const vector& angularMomentum0,
            const vector& torque0,
            const typename ParcelType::constantProperties& constProps
        );

        //- Construct from Istream
        CollidingParcel
        (
            const polyMesh& mesh,
            Istream& is,
            bool readFields = true
        );

        //- Construct as a copy
        CollidingParcel(const CollidingParcel& p);

        //- Construct as a copy
        CollidingParcel(const CollidingParcel& p, const polyMesh& mesh);

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone() const
        {
            return autoPtr<particle>(new CollidingParcel(*this));
        }

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone(const polyMesh& mesh) const
        {
            return autoPtr<particle>(new CollidingParcel(*this, mesh));
        }

        //- Factory class to read-construct particles used for
        //  parallel transfer
        class iNew
        {
            const polyMesh& mesh_;

        public:

            iNew(const polyMesh& mesh)
            :
                mesh_(mesh)
            {}

            autoPtr<CollidingParcel<ParcelType> > operator()(Istream& is) const
            {
                return autoPtr<CollidingParcel<ParcelType> >
                (
                    new CollidingParcel<ParcelType>(mesh_, is, true)
                );
            }
        };


    // Member Functions

        // Access

            //- Return const access to the collision records
            inline const collisionRecordList& collisionRecords() const;

            //- Return access to collision records
            inline collisionRecordList& collisionRecords();


        // Tracking

            //- Move the parcel
            template<class TrackData>
            bool move(TrackData& td, const scalar trackTime);


       // I-O

            //- Read
            template<class CloudType>
            static void readFields(CloudType& c);

            //- Write
            template<class CloudType>
            static void writeFields(const CloudType& c);


    // Ostream Operator

        friend Ostream& operator<< <ParcelType>
        (
            Ostream&,
            const CollidingParcel<ParcelType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
inline CML::CollidingParcel<ParcelType>::CollidingParcel
(
    const polyMesh& owner,
    const vector& position,
    const label cellI,
    const label tetFaceI,
    const label tetPtI
)
:
    ParcelType(owner, position, cellI, tetFaceI, tetPtI),
    collisionRecords_()
{}


template<class ParcelType>
inline CML::CollidingParcel<ParcelType>::CollidingParcel
(
    const polyMesh& owner,
    const vector& position,
    const label cellI,
    const label tetFaceI,
    const label tetPtI,
    const label typeId,
    const scalar nParticle0,
    const scalar d0,
    const scalar dTarget0,
    const vector& U0,
    const vector& f0,
    const vector& angularMomentum0,
    const vector& torque0,
    const typename ParcelType::constantProperties& constProps
)
:
    ParcelType
    (
        owner,
        position,
        cellI,
        tetFaceI,
        tetPtI,
        typeId,
        nParticle0,
        d0,
        dTarget0,
        U0,
        f0,
        angularMomentum0,
        torque0,
        constProps
    ),
    collisionRecords_()
{}


// * * * * * * * CollidingParcel Member Functions  * * * * * * * //


template<class ParcelType>
inline const CML::collisionRecordList&
CML::CollidingParcel<ParcelType>::collisionRecords() const
{
    return collisionRecords_;
}


template<class ParcelType>
inline CML::collisionRecordList&
CML::CollidingParcel<ParcelType>::collisionRecords()
{
    return collisionRecords_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::CollidingParcel<ParcelType>::CollidingParcel
(
    const CollidingParcel<ParcelType>& p
)
:
    ParcelType(p),
    collisionRecords_(p.collisionRecords_)
{}


template<class ParcelType>
CML::CollidingParcel<ParcelType>::CollidingParcel
(
    const CollidingParcel<ParcelType>& p,
    const polyMesh& mesh
)
:
    ParcelType(p, mesh),
    collisionRecords_(p.collisionRecords_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackData>
bool CML::CollidingParcel<ParcelType>::move
(
    TrackData& td,
    const scalar trackTime
)
{
    typename TrackData::cloudType::parcelType& p =
        static_cast<typename TrackData::cloudType::parcelType&>(*this);

    td.switchProcessor = false;
    td.keepParticle = true;

    const polyMesh& mesh = td.cloud().pMesh();
    const polyBoundaryMesh& pbMesh = mesh.boundaryMesh();
    const scalarField& V = mesh.cellVolumes();

    switch (td.part())
    {
        case TrackData::tpVelocityHalfStep:
        {
            // First and last leapfrog velocity adjust part, required
            // before and after tracking and force calculation

            p.U() += 0.5*trackTime*p.f()/p.mass();

            p.angularMomentum() += 0.5*trackTime*p.torque();

            break;
        }

        case TrackData::tpLinearTrack:
        {
            scalar tEnd = (1.0 - p.stepFraction())*trackTime;
            const scalar dtMax = tEnd;

            while (td.keepParticle && !td.switchProcessor && tEnd > ROOTVSMALL)
            {
                // Apply correction to position for reduced-D cases
                meshTools::constrainToMeshCentre(mesh, p.position());

                // Set the Lagrangian time-step
                scalar dt = min(dtMax, tEnd);

                // Remember which cell the parcel is in since this
                // will change if a face is hit
                const label cellI = p.cell();

                const scalar magU = mag(p.U());
                if (p.active() && magU > ROOTVSMALL)
                {
                    const scalar d = dt*magU;
                    const scalar maxCo = td.cloud().solution().maxCo();
                    const scalar dCorr = min(d, maxCo*cbrt(V[cellI]));
                    dt *=
                        dCorr/d
                       *p.trackToFace(p.position() + dCorr*p.U()/magU, td);
                }

                tEnd -= dt;
                p.stepFraction() = 1.0 - tEnd/trackTime;

                // Avoid problems with extremely small timesteps
                if (dt > ROOTVSMALL)
                {
                    // Update cell based properties
                    p.setCellValues(td, dt, cellI);

                    if (td.cloud().solution().cellValueSourceCorrection())
                    {
                        p.cellValueSourceCorrection(td, dt, cellI);
                    }

                    p.calc(td, dt, cellI);
                }

                if (p.onBoundary() && td.keepParticle)
                {
                    if (isA<processorPolyPatch>(pbMesh[p.patch(p.face())]))
                    {
                        td.switchProcessor = true;
                    }
                }

                p.age() += dt;
            }

            break;
        }

        case TrackData::tpRotationalTrack:
        {
            notImplemented("TrackData::tpRotationalTrack");

            break;
        }

        default:
        {
            FatalErrorIn
            (
                "CollidingParcel<ParcelType>::move(TrackData&, const scalar)"
            )   << td.part() << " is an invalid part of the tracking method."
                << abort(FatalError);
        }
    }

    return td.keepParticle;
}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ParcelType>
CML::string CML::CollidingParcel<ParcelType>::propHeader =
    ParcelType::propHeader
  + " collisionRecordsPairAccessed"
  + " collisionRecordsPairOrigProcOfOther"
  + " collisionRecordsPairOrigIdOfOther"
  + " (collisionRecordsPairData)"
  + " collisionRecordsWallAccessed"
  + " collisionRecordsWallPRel"
  + " (collisionRecordsWallData)";


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::CollidingParcel<ParcelType>::CollidingParcel
(
    const polyMesh& mesh,
    Istream& is,
    bool readFields
)
:
    ParcelType(mesh, is, readFields),
    collisionRecords_()
{
    if (readFields)
    {
        is >> collisionRecords_;
    }

    // Check state of Istream
    is.check
    (
        "CollidingParcel<ParcelType>::Collisions"
        "(const polyMesh&, Istream&, bool)"
    );
}


template<class ParcelType>
template<class CloudType>
void CML::CollidingParcel<ParcelType>::readFields(CloudType& c)
{
    if (!c.size())
    {
        return;
    }

    ParcelType::readFields(c);

    labelFieldCompactIOField collisionRecordsPairAccessed
    (
        c.fieldIOobject("collisionRecordsPairAccessed", IOobject::MUST_READ)
    );
    c.checkFieldFieldIOobject(c, collisionRecordsPairAccessed);

    labelFieldCompactIOField collisionRecordsPairOrigProcOfOther
    (
        c.fieldIOobject
        (
            "collisionRecordsPairOrigProcOfOther",
            IOobject::MUST_READ
        )
    );
    c.checkFieldFieldIOobject(c, collisionRecordsPairOrigProcOfOther);

    labelFieldCompactIOField collisionRecordsPairOrigIdOfOther
    (
        c.fieldIOobject
        (
            "collisionRecordsPairOrigIdOfOther",
            IOobject::MUST_READ
        )
    );
    c.checkFieldFieldIOobject(c, collisionRecordsPairOrigProcOfOther);

    pairDataFieldCompactIOField collisionRecordsPairData
    (
        c.fieldIOobject("collisionRecordsPairData", IOobject::MUST_READ)
    );
    c.checkFieldFieldIOobject(c, collisionRecordsPairData);

    labelFieldCompactIOField collisionRecordsWallAccessed
    (
        c.fieldIOobject("collisionRecordsWallAccessed", IOobject::MUST_READ)
    );
    c.checkFieldFieldIOobject(c, collisionRecordsWallAccessed);

    vectorFieldCompactIOField collisionRecordsWallPRel
    (
        c.fieldIOobject("collisionRecordsWallPRel", IOobject::MUST_READ)
    );
    c.checkFieldFieldIOobject(c, collisionRecordsWallPRel);

    wallDataFieldCompactIOField collisionRecordsWallData
    (
        c.fieldIOobject("collisionRecordsWallData", IOobject::MUST_READ)
    );
    c.checkFieldFieldIOobject(c, collisionRecordsWallData);

    label i = 0;

    forAllIter(typename CloudType, c, iter)
    {
        CollidingParcel<ParcelType>& p = iter();

        p.collisionRecords_ = collisionRecordList
        (
            collisionRecordsPairAccessed[i],
            collisionRecordsPairOrigProcOfOther[i],
            collisionRecordsPairOrigIdOfOther[i],
            collisionRecordsPairData[i],
            collisionRecordsWallAccessed[i],
            collisionRecordsWallPRel[i],
            collisionRecordsWallData[i]
        );

        i++;
    }
}


template<class ParcelType>
template<class CloudType>
void CML::CollidingParcel<ParcelType>::writeFields(const CloudType& c)
{
    ParcelType::writeFields(c);

    label np =  c.size();

    labelFieldCompactIOField collisionRecordsPairAccessed
    (
        c.fieldIOobject("collisionRecordsPairAccessed", IOobject::NO_READ),
        np
    );
    labelFieldCompactIOField collisionRecordsPairOrigProcOfOther
    (
        c.fieldIOobject
        (
            "collisionRecordsPairOrigProcOfOther",
            IOobject::NO_READ
        ),
        np
    );
    labelFieldCompactIOField collisionRecordsPairOrigIdOfOther
    (
        c.fieldIOobject("collisionRecordsPairOrigIdOfOther", IOobject::NO_READ),
        np
    );
    pairDataFieldCompactIOField collisionRecordsPairData
    (
        c.fieldIOobject("collisionRecordsPairData", IOobject::NO_READ),
        np
    );
    labelFieldCompactIOField collisionRecordsWallAccessed
    (
        c.fieldIOobject("collisionRecordsWallAccessed", IOobject::NO_READ),
        np
    );
    vectorFieldCompactIOField collisionRecordsWallPRel
    (
        c.fieldIOobject("collisionRecordsWallPRel", IOobject::NO_READ),
        np
    );
    wallDataFieldCompactIOField collisionRecordsWallData
    (
        c.fieldIOobject("collisionRecordsWallData", IOobject::NO_READ),
        np
    );

    label i = 0;

    forAllConstIter(typename CloudType, c, iter)
    {
        const CollidingParcel<ParcelType>& p = iter();

        collisionRecordsPairAccessed[i] = p.collisionRecords().pairAccessed();
        collisionRecordsPairOrigProcOfOther[i] =
            p.collisionRecords().pairOrigProcOfOther();
        collisionRecordsPairOrigIdOfOther[i] =
            p.collisionRecords().pairOrigIdOfOther();
        collisionRecordsPairData[i] = p.collisionRecords().pairData();
        collisionRecordsWallAccessed[i] = p.collisionRecords().wallAccessed();
        collisionRecordsWallPRel[i] = p.collisionRecords().wallPRel();
        collisionRecordsWallData[i] = p.collisionRecords().wallData();

        i++;
    }

    collisionRecordsPairAccessed.write();
    collisionRecordsPairOrigProcOfOther.write();
    collisionRecordsPairOrigIdOfOther.write();
    collisionRecordsPairData.write();
    collisionRecordsWallAccessed.write();
    collisionRecordsWallPRel.write();
    collisionRecordsWallData.write();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class ParcelType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const CollidingParcel<ParcelType>& p
)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const ParcelType&>(p)
            << token::SPACE << p.collisionRecords();
    }
    else
    {
        os  << static_cast<const ParcelType&>(p)
            << p.collisionRecords();
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const CollidingParcel<ParcelType>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
