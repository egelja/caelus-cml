/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::CellZoneInjection

Description
    Injection positions specified by a particle number density within a cell
    set.

    User specifies:
      - Number density of particles in cell set (effective)
      - Total mass to inject
      - Initial parcel velocity

    Properties:
      - Parcel diameters obtained by PDF model
      - All parcels introduced at SOI


\*---------------------------------------------------------------------------*/

#ifndef CellZoneInjection_H
#define CellZoneInjection_H

#include "InjectionModel.hpp"
#include "distributionModel.hpp"
#include "mathematicalConstants.hpp"
#include "polyMeshTetDecomposition.hpp"
#include "globalIndex.hpp"
#include "Pstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class CellZoneInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CellZoneInjection
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

        //- Field of parcel diameters
        scalarList diameters_;

        //- Initial parcel velocity
        const vector U0_;

        //- Parcel size distribution model
        const autoPtr<distributionModel> sizeDistribution_;


    // Private Member Functions

        //- Set the parcel injection positions
        void setPositions(const labelList& cellZoneCells);


public:

    //- Runtime type information
    TypeName("cellZoneInjection");


    // Constructors

        //- Construct from dictionary
        CellZoneInjection
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        CellZoneInjection(const CellZoneInjection<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new CellZoneInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~CellZoneInjection();


    // Member Functions

        //- Set injector locations when mesh is updated
        virtual void updateMesh();

        //- Return the end-of-injection time
        scalar timeEnd() const;

        //- Number of parcels to introduce relative to SOI
        label parcelsToInject(const scalar time0, const scalar time1);

        //- Volume of parcels to introduce relative to SOI
        scalar volumeToInject(const scalar time0, const scalar time1);


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


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
void CML::CellZoneInjection<CloudType>::setPositions
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
        const label celli = cellZoneCells[i];

        // Calc number of particles to add
        const scalar newParticles = V[celli]*numberDensity_;
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
            polyMeshTetDecomposition::cellTetIndices(mesh, celli);

        // Construct cell tet volume fractions
        scalarList cTetVFrac(cellTetIs.size(), 0.0);
        for (label tetI = 1; tetI < cellTetIs.size() - 1; tetI++)
        {
            cTetVFrac[tetI] =
                cTetVFrac[tetI-1] + cellTetIs[tetI].tet(mesh).mag()/V[celli];
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

            injectorCells.append(celli);
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
CML::CellZoneInjection<CloudType>::CellZoneInjection
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
    diameters_(),
    U0_(this->coeffDict().lookup("U0")),
    sizeDistribution_
    (
        distributionModel::New
        (
            this->coeffDict().subDict("sizeDistribution"), owner.rndGen()
        )
    )
{
    updateMesh();
}


template<class CloudType>
CML::CellZoneInjection<CloudType>::CellZoneInjection
(
    const CellZoneInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    cellZoneName_(im.cellZoneName_),
    numberDensity_(im.numberDensity_),
    positions_(im.positions_),
    injectorCells_(im.injectorCells_),
    injectorTetFaces_(im.injectorTetFaces_),
    injectorTetPts_(im.injectorTetPts_),
    diameters_(im.diameters_),
    U0_(im.U0_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::CellZoneInjection<CloudType>::~CellZoneInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::CellZoneInjection<CloudType>::updateMesh()
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
            << "    number of particles = " << positions_.size() << endl;

        // Construct parcel diameters
        diameters_.setSize(positions_.size());
        forAll(diameters_, i)
        {
            diameters_[i] = sizeDistribution_->sample();
        }
    }

    // Determine volume of particles to inject
    this->volumeTotal_ = sum(pow3(diameters_))*constant::mathematical::pi/6.0;
}


template<class CloudType>
CML::scalar CML::CellZoneInjection<CloudType>::timeEnd() const
{
    // Not used
    return this->SOI_;
}


template<class CloudType>
CML::label CML::CellZoneInjection<CloudType>::parcelsToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((0.0 >= time0) && (0.0 < time1))
    {
        return positions_.size();
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
CML::scalar CML::CellZoneInjection<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    // All parcels introduced at SOI
    if ((0.0 >= time0) && (0.0 < time1))
    {
        return this->volumeTotal_;
    }
    else
    {
        return 0.0;
    }
}


template<class CloudType>
void CML::CellZoneInjection<CloudType>::setPositionAndCell
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
    position = positions_[parcelI];
    cellOwner = injectorCells_[parcelI];
    tetFacei = injectorTetFaces_[parcelI];
    tetPti = injectorTetPts_[parcelI];
}


template<class CloudType>
void CML::CellZoneInjection<CloudType>::setProperties
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
    parcel.d() = diameters_[parcelI];
}


template<class CloudType>
bool CML::CellZoneInjection<CloudType>::fullyDescribed() const
{
    return false;
}


template<class CloudType>
bool CML::CellZoneInjection<CloudType>::validInjection(const label)
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
