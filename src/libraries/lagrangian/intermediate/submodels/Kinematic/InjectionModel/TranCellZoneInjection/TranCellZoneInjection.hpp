/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::TranCellZoneInjection

Description
    Injection positions specified by a particle number density within a cell set

    - User specifies
      - Number density of particles in cell set (effective)
      - Total mass to inject
      - Injection duration
      - Initial parcel velocity
      - Injection volume flow rate
    - Parcel diameters obtained by distribution model

SourceFiles
    TranCellZoneInjection.cpp

\*---------------------------------------------------------------------------*/

#ifndef TranCellZoneInjection_HPP
#define TranCellZoneInjection_HPP

#include "InjectionModel.hpp"
#include "distributionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class TimeDataEntry;

class distributionModel;

/*---------------------------------------------------------------------------*\
                     Class TranCellZoneInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class TranCellZoneInjection
:
    public InjectionModel<CloudType>
{
    // Private data

        //- Name of cell zone
        const word cellZoneName_;

        //- Number density
        const scalar numberDensity_;

        //- Field of parcel positions
        List<vector> positions_;

        //- List of cell labels corresponding to injector positions
        labelList injectorCells_;

        //- List of tetFace labels corresponding to injector positions
        labelList injectorTetFaces_;

        //- List of tetPt labels corresponding to injector positions
        labelList injectorTetPts_;

        //- Injection duration [s]
        scalar duration_;

        //- Number of parcels to introduce per second []
        const label parcelsPerSecond_;

        //- Initial parcel velocity
        const vector U0_;

        //- Flow rate profile relative to SOI []
        const TimeDataEntry<scalar> flowRateProfile_;

        //- Parcel size distribution model
        const autoPtr<distributionModel> sizeDistribution_;


    // Private Member Functions

        //- Set the parcel injection positions
        void setPositions(const labelList& cellZoneCells);


public:

    //- Runtime type information
    TypeName("TranCellZoneInjection");


    // Constructors

        //- Construct from dictionary
        TranCellZoneInjection
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        TranCellZoneInjection(const TranCellZoneInjection<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new TranCellZoneInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~TranCellZoneInjection();


    // Member Functions

        //- Set injector locations when mesh is updated
        virtual void updateMesh();

        //- Return the end-of-injection time
        scalar timeEnd() const;

        //- Number of parcels to introduce relative to SOI
        virtual label parcelsToInject(const scalar time0, const scalar time1);

        //- Volume of parcels to introduce relative to SOI
        virtual scalar volumeToInject(const scalar time0, const scalar time1);


        // Injection geometry

            //- Set the injection position and owner cell, tetFace and tetPt
            virtual void setPositionAndCell
            (
                const label parcelI,
                const label nParcels,
                const scalar time,
                vector& position,
                label& cellOwner,
                label& tetFacei,
                label& tetPti
            );

            //- Set the parcel properties
            virtual void setProperties
            (
                const label parcelI,
                const label nParcels,
                const scalar time,
                typename CloudType::parcelType& parcel
            );

            //- Flag to identify whether model fully describes the parcel
            virtual bool fullyDescribed() const;

            //- Return flag to identify whether or not injection of parcelI is
            //  permitted
            virtual bool validInjection(const label parcelI);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "TimeDataEntry.hpp"
#include "mathematicalConstants.hpp"
#include "polyMeshTetDecomposition.hpp"
#include "globalIndex.hpp"
#include "Pstream.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
void CML::TranCellZoneInjection<CloudType>::setPositions
(
    const labelList& cellZoneCells
)
{
    const fvMesh& mesh = this->owner().mesh();
    const scalarField& V = mesh.V();
    const label nCells = cellZoneCells.size();
    Random& rnd = this->owner().rndGen();

    DynamicList<vector> positions(nCells);          // initial size only
    DynamicList<label> injectorCells(nCells);       // initial size only
    DynamicList<label> injectorTetFaces(nCells);    // initial size only
    DynamicList<label> injectorTetPts(nCells);      // initial size only

    scalar newParticlesTotal = 0.0;
    label addParticlesTotal = 0;

    forAll(cellZoneCells, i)
    {
        const label cellI = cellZoneCells[i];

        // Calc number of particles to add
        const scalar newParticles = V[cellI]*numberDensity_;
        newParticlesTotal += newParticles;
        label addParticles = floor(newParticles);
        addParticlesTotal += addParticles;

        const scalar diff = newParticlesTotal - addParticlesTotal;
        if (diff > 1)
        {
            label corr = floor(diff);
            addParticles += corr;
            addParticlesTotal += corr;
        }

        // Construct cell tet indices
        const List<tetIndices> cellTetIs =
            polyMeshTetDecomposition::cellTetIndices(mesh, cellI);

        // Construct cell tet volume fractions
        scalarList cTetVFrac(cellTetIs.size(), 0.0);
        for (label tetI = 1; tetI < cellTetIs.size() - 1; tetI++)
        {
            cTetVFrac[tetI] =
                cTetVFrac[tetI-1] + cellTetIs[tetI].tet(mesh).mag()/V[cellI];
        }
        cTetVFrac.last() = 1.0;

        // Set new particle position and cellId
        for (label pI = 0; pI < addParticles; pI++)
        {
            const scalar volFrac = rnd.sample01<scalar>();
            label tetI = 0;
            forAll(cTetVFrac, vfI)
            {
                if (cTetVFrac[vfI] > volFrac)
                {
                    tetI = vfI;
                    break;
                }
            }
            positions.append(cellTetIs[tetI].tet(mesh).randomPoint(rnd));

            injectorCells.append(cellI);
            injectorTetFaces.append(cellTetIs[tetI].face());
            injectorTetPts.append(cellTetIs[tetI].tetPt());
        }
    }

    // Parallel operation manipulations
    globalIndex globalPositions(positions.size());
    List<vector> allPositions(globalPositions.size(), point::max);
    List<label> allInjectorCells(globalPositions.size(), -1);
    List<label> allInjectorTetFaces(globalPositions.size(), -1);
    List<label> allInjectorTetPts(globalPositions.size(), -1);

    // Gather all positions on to all processors
    SubList<vector>
    (
        allPositions,
        globalPositions.localSize(Pstream::myProcNo()),
        globalPositions.offset(Pstream::myProcNo())
    ).assign(positions);

    Pstream::listCombineGather(allPositions, minEqOp<point>());
    Pstream::listCombineScatter(allPositions);

    // Gather local cell tet and tet-point Ids, but leave non-local ids set -1
    SubList<label>
    (
        allInjectorCells,
        globalPositions.localSize(Pstream::myProcNo()),
        globalPositions.offset(Pstream::myProcNo())
    ).assign(injectorCells);
    SubList<label>
    (
        allInjectorTetFaces,
        globalPositions.localSize(Pstream::myProcNo()),
        globalPositions.offset(Pstream::myProcNo())
    ).assign(injectorTetFaces);
    SubList<label>
    (
        allInjectorTetPts,
        globalPositions.localSize(Pstream::myProcNo()),
        globalPositions.offset(Pstream::myProcNo())
    ).assign(injectorTetPts);

    // Transfer data
    positions_.transfer(allPositions);
    injectorCells_.transfer(allInjectorCells);
    injectorTetFaces_.transfer(allInjectorTetFaces);
    injectorTetPts_.transfer(allInjectorTetPts);


    if (debug)
    {
        OFstream points("points.obj");
        forAll(positions_, i)
        {
            meshTools::writeOBJ(points, positions_[i]);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::TranCellZoneInjection<CloudType>::TranCellZoneInjection
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    InjectionModel<CloudType>(dict, owner, modelName, typeName),
    cellZoneName_(this->coeffDict().lookup("cellZone")),
    numberDensity_(readScalar(this->coeffDict().lookup("numberDensity"))),
    positions_(),
    injectorCells_(),
    injectorTetFaces_(),
    injectorTetPts_(),
    duration_(readScalar(this->coeffDict().lookup("duration"))),
    parcelsPerSecond_
    (
        readScalar(this->coeffDict().lookup("parcelsPerSecond"))
    ),
    U0_(this->coeffDict().lookup("U0")),
    flowRateProfile_
    (
        TimeDataEntry<scalar>
        (
            owner.db().time(),
            "flowRateProfile",
            this->coeffDict()
        )
    ),
    sizeDistribution_
    (
        distributionModel::New
        (
            this->coeffDict().subDict("sizeDistribution"),
            owner.rndGen()
        )
    )
{
    updateMesh();
}


template<class CloudType>
CML::TranCellZoneInjection<CloudType>::TranCellZoneInjection
(
    const TranCellZoneInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    cellZoneName_(im.cellZoneName_),
    numberDensity_(im.numberDensity_),
    positions_(im.positions_),
    injectorCells_(im.injectorCells_),
    injectorTetFaces_(im.injectorTetFaces_),
    injectorTetPts_(im.injectorTetPts_),
    duration_(im.duration_),
    parcelsPerSecond_(im.parcelsPerSecond_),
    U0_(im.U0_),
    flowRateProfile_(im.flowRateProfile_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::TranCellZoneInjection<CloudType>::~TranCellZoneInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::TranCellZoneInjection<CloudType>::updateMesh()
{
    // Set/cache the injector cells
    const fvMesh& mesh = this->owner().mesh();
    const label zoneI = mesh.cellZones().findZoneID(cellZoneName_);

    if (zoneI < 0)
    {
        FatalErrorInFunction
            << "Unknown cell zone name: " << cellZoneName_
            << ". Valid cell zones are: " << mesh.cellZones().names()
            << nl << exit(FatalError);
    }

    const labelList& cellZoneCells = mesh.cellZones()[zoneI];
    const label nCells = cellZoneCells.size();
    const scalar nCellsTotal = returnReduce(nCells, sumOp<label>());
    const scalar VCells = sum(scalarField(mesh.V(), cellZoneCells));
    const scalar VCellsTotal = returnReduce(VCells, sumOp<scalar>());
    Info<< "    cell zone size      = " << nCellsTotal << endl;
    Info<< "    cell zone volume    = " << VCellsTotal << endl;

    if ((nCellsTotal == 0) || (VCellsTotal*numberDensity_ < 1))
    {
        WarningInFunction
            << "Number of particles to be added to cellZone " << cellZoneName_
            << " is zero" << endl;
    }
    else
    {
        setPositions(cellZoneCells);

        Info<< "    number density      = " << numberDensity_ << nl
            << "    number of positions = " << positions_.size() << endl;

    }

    // Determine volume of particles to inject
    this->volumeTotal_ = flowRateProfile_.integrate(0.0, duration_);
    Info << "    total volume        = " << this->volumeTotal_ <<endl;
}


template<class CloudType>
CML::scalar CML::TranCellZoneInjection<CloudType>::timeEnd() const
{
    return this->SOI_ + duration_;
}


template<class CloudType>
CML::label CML::TranCellZoneInjection<CloudType>::parcelsToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((time0 >= 0.0) && (time0 < duration_))
    {
        // Number of parcels per second set equal to number of available positions
        scalar nParcels = (time1 - time0)*parcelsPerSecond_;

        Random& rnd = this->owner().rndGen();

        label nParcelsToInject = floor(nParcels);

        // Inject an additional parcel with a probability based on the
        // remainder after the floor function
        if
        (
            nParcelsToInject > 0
         && (nParcels - scalar(nParcelsToInject) > rnd.globalScalar01())
        )
        {
            ++nParcelsToInject;
        }

        return nParcelsToInject;
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
CML::scalar CML::TranCellZoneInjection<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((time0 >= 0.0) && (time0 < duration_))
    {
        return flowRateProfile_.integrate(time0, time1);
    }
    else
    {
        return 0.0;
    }
}


template<class CloudType>
void CML::TranCellZoneInjection<CloudType>::setPositionAndCell
(
    const label parcelI,
    const label nParcels,
    const scalar time,
    vector& position,
    label& cellOwner,
    label& tetFacei,
    label& tetPti
)
{
    if (positions_.size() > 0)
    {
        // Randomly select a position from the pre-calculated positions
        Random& rnd = this->owner().rndGen();

        label celli = rnd.sampleAB<label>(0, positions_.size());

        cellOwner = injectorCells_[celli];
        tetFacei = injectorTetFaces_[celli];
        tetPti = injectorTetPts_[celli];
        position = positions_[celli];
    }
    else
    {
        cellOwner = -1;
        tetFacei = -1;
        tetPti = -1;
        // dummy position
        position = pTraits<vector>::max;
    }
}


template<class CloudType>
void CML::TranCellZoneInjection<CloudType>::setProperties
(
    const label parcelI,
    const label,
    const scalar,
    typename CloudType::parcelType& parcel
)
{
    // set particle velocity
    parcel.U() = U0_;

    // set particle diameter
    parcel.d() = sizeDistribution_->sample();
}


template<class CloudType>
bool CML::TranCellZoneInjection<CloudType>::fullyDescribed() const
{
    return false;
}


template<class CloudType>
bool CML::TranCellZoneInjection<CloudType>::validInjection(const label)
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
