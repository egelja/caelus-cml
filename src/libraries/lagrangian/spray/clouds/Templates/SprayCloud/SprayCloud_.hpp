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
    CML::SprayCloud

Description
    Templated base class for spray cloud

    - sub-models:
      - atomization model
      - break-up model

\*---------------------------------------------------------------------------*/

#ifndef SprayCloud__H
#define SprayCloud__H

#include "sprayCloud.hpp"
#include "AtomizationModel.hpp"
#include "BreakupModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
template<class CloudType>
class AtomizationModel;

template<class CloudType>
class BreakupModel;

/*---------------------------------------------------------------------------*\
                      Class SprayCloud Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class SprayCloud
:
    public CloudType,
    public sprayCloud
{
public:

    // Public typedefs

        //- Type of cloud this cloud was instantiated for
        typedef CloudType cloudType;

        //- Type of parcel the cloud was instantiated for
        typedef typename CloudType::particleType parcelType;

        //- Convenience typedef for this cloud type
        typedef SprayCloud<CloudType> sprayCloudType;


private:

    // Private data

        //- Cloud copy pointer
        autoPtr<SprayCloud<CloudType> > cloudCopyPtr_;

        //- Average parcel mass
        scalar averageParcelMass_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        SprayCloud(const SprayCloud&);

        //- Disallow default bitwise assignment
        void operator=(const SprayCloud&);


protected:

    // Protected data

        // References to the cloud sub-models

            //- Atomization model
            autoPtr<AtomizationModel<SprayCloud<CloudType> > >
                atomizationModel_;

            //- Break-up model
            autoPtr<BreakupModel<SprayCloud<CloudType> > > breakupModel_;


    // Protected Member Functions

        // Initialisation

            //- Set cloud sub-models
            void setModels();


        // Cloud evolution functions

            //- Reset state of cloud
            void cloudReset(SprayCloud<CloudType>& c);


public:

    // Constructors

        //- Construct given carrier gas fields
        SprayCloud
        (
            const word& cloudName,
            const volScalarField& rho,
            const volVectorField& U,
            const dimensionedVector& g,
            const SLGThermo& thermo,
            bool readFields = true
        );

        //- Copy constructor with new name
        SprayCloud(SprayCloud<CloudType>& c, const word& name);

        //- Copy constructor with new name - creates bare cloud
        SprayCloud
        (
            const fvMesh& mesh,
            const word& name,
            const SprayCloud<CloudType>& c
        );


        //- Construct and return clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > clone(const word& name)
        {
            return autoPtr<Cloud<parcelType> >
            (
                new SprayCloud(*this, name)
            );
        }

        //- Construct and return bare clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > cloneBare(const word& name) const
        {
            return autoPtr<Cloud<parcelType> >
            (
                new SprayCloud(this->mesh(), name, *this)
            );
        }


    //- Destructor
    virtual ~SprayCloud();


    // Member Functions

        // Access

            //- Return a reference to the cloud copy
            inline const SprayCloud& cloudCopy() const;

            //- Return const-access to the average parcel mass
            inline scalar averageParcelMass() const;


        // Check

            //- Penetration for fraction [0-1] of the current total mass
            inline scalar penetration(const scalar fraction) const;


            // Sub-models

                //- Return const-access to the atomization model
                inline const AtomizationModel<SprayCloud<CloudType> >&
                    atomization() const;

                //- Return reference to the atomization model
                inline AtomizationModel<SprayCloud<CloudType> >& atomization();

                //- Return const-access to the breakup model
                inline const BreakupModel<SprayCloud<CloudType> >&
                    breakup() const;

                //- Return reference to the breakup model
                inline BreakupModel<SprayCloud<CloudType> >& breakup();


        // Cloud evolution functions

            //- Set parcel thermo properties
            void setParcelThermoProperties
            (
                parcelType& parcel,
                const scalar lagrangianDt
            );

            //- Check parcel properties
            void checkParcelProperties
            (
                parcelType& parcel,
                const scalar lagrangianDt,
                const bool fullyDescribed
            );

            //- Store the current cloud state
            void storeState();

            //- Reset the current cloud to the previously stored state
            void restoreState();

            //- Evolve the spray (inject, move)
            void evolve();


        // I-O

            //- Print cloud information
            void info();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
inline const CML::SprayCloud<CloudType>&
CML::SprayCloud<CloudType>::cloudCopy() const
{
    return cloudCopyPtr_();
}


template<class CloudType>
inline const CML::AtomizationModel<CML::SprayCloud<CloudType> >&
CML::SprayCloud<CloudType>::atomization() const
{
    return atomizationModel_;
}


template<class CloudType>
inline CML::AtomizationModel<CML::SprayCloud<CloudType> >&
CML::SprayCloud<CloudType>::atomization()
{
    return atomizationModel_();
}


template<class CloudType>
inline const CML::BreakupModel<CML::SprayCloud<CloudType> >&
CML::SprayCloud<CloudType>::breakup() const
{
    return breakupModel_;
}


template<class CloudType>
inline CML::BreakupModel<CML::SprayCloud<CloudType> >&
CML::SprayCloud<CloudType>::breakup()
{
    return breakupModel_();
}


template<class CloudType>
inline CML::scalar CML::SprayCloud<CloudType>::averageParcelMass() const
{
    return averageParcelMass_;
}


template<class CloudType>
inline CML::scalar CML::SprayCloud<CloudType>::penetration
(
    const scalar fraction
) const
{
    if ((fraction < 0) || (fraction > 1))
    {
        FatalErrorInFunction
            << "fraction should be in the range 0 < fraction < 1"
            << exit(FatalError);
    }

    scalar distance = 0.0;

    const label nParcel = this->size();
    globalIndex globalParcels(nParcel);
    const label nParcelSum = globalParcels.size();

    if (nParcelSum == 0)
    {
        return distance;
    }

    // lists of parcels mass and distance from initial injection point
    List<List<scalar>> procMass(Pstream::nProcs());
    List<List<scalar>> procDist(Pstream::nProcs());

    List<scalar>& mass = procMass[Pstream::myProcNo()];
    List<scalar>& dist = procDist[Pstream::myProcNo()];

    mass.setSize(nParcel);
    dist.setSize(nParcel);

    label i = 0;
    scalar mSum = 0.0;
    forAllConstIter(typename SprayCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();
        scalar m = p.nParticle()*p.mass();
        scalar d = mag(p.position() - p.position0());
        mSum += m;

        mass[i] = m;
        dist[i] = d;

        i++;
    }

    // calculate total mass across all processors
    reduce(mSum, sumOp<scalar>());
    Pstream::gatherList(procMass);
    Pstream::gatherList(procDist);

    if (Pstream::master())
    {
        // flatten the mass lists
        List<scalar> allMass(nParcelSum, 0.0);
        SortableList<scalar> allDist(nParcelSum, 0.0);
        for (label proci = 0; proci < Pstream::nProcs(); proci++)
        {
            SubList<scalar>
            (
                allMass,
                globalParcels.localSize(proci),
                globalParcels.offset(proci)
            ) = procMass[proci];

            // flatten the distance list
            SubList<scalar>
            (
                allDist,
                globalParcels.localSize(proci),
                globalParcels.offset(proci)
            ) = procDist[proci];
        }

        // sort allDist distances into ascending order
        // note: allMass masses are left unsorted
        allDist.sort();

        if (nParcelSum > 1)
        {
            const scalar mLimit = fraction*mSum;
            const labelList& indices = allDist.indices();

            if (mLimit > (mSum - allMass[indices.last()]))
            {
                distance = allDist.last();
            }
            else
            {
                // assuming that 'fraction' is generally closer to 1 than 0,
                // loop through in reverse distance order
                const scalar mThreshold = (1.0 - fraction)*mSum;
                scalar mCurrent = 0.0;
                label i0 = 0;

                forAllReverse(indices, i)
                {
                    label indI = indices[i];

                    mCurrent += allMass[indI];

                    if (mCurrent > mThreshold)
                    {
                        i0 = i;
                        break;
                    }
                }

                if (i0 == indices.size() - 1)
                {
                    distance = allDist.last();
                }
                else
                {
                    // linearly interpolate to determine distance
                    scalar alpha = (mCurrent - mThreshold)/allMass[indices[i0]];
                    distance =
                        allDist[i0] + alpha*(allDist[i0+1] - allDist[i0]);
                }
            }
        }
        else
        {
            distance = allDist.first();
        }
    }

    Pstream::scatter(distance);

    return distance;
}
// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::SprayCloud<CloudType>::setModels()
{
    atomizationModel_.reset
    (
        AtomizationModel<SprayCloud<CloudType> >::New
        (
            this->subModelProperties(),
            *this
        ).ptr()
    );

    breakupModel_.reset
    (
        BreakupModel<SprayCloud<CloudType> >::New
        (
            this->subModelProperties(),
            *this
        ).ptr()
    );
}


template<class CloudType>
void CML::SprayCloud<CloudType>::cloudReset
(
    SprayCloud<CloudType>& c
)
{
    CloudType::cloudReset(c);

    atomizationModel_.reset(c.atomizationModel_.ptr());
    breakupModel_.reset(c.breakupModel_.ptr());
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::SprayCloud<CloudType>::SprayCloud
(
    const word& cloudName,
    const volScalarField& rho,
    const volVectorField& U,
    const dimensionedVector& g,
    const SLGThermo& thermo,
    bool readFields
)
:
    CloudType(cloudName, rho, U, g, thermo, false),
    sprayCloud(),
    cloudCopyPtr_(nullptr),
    averageParcelMass_(0.0),
    atomizationModel_(nullptr),
    breakupModel_(nullptr)
{
    if (this->solution().active())
    {
        setModels();

        averageParcelMass_ = this->injectors().averageParcelMass();

        if (readFields)
        {
            parcelType::readFields(*this, this->composition());
            this->deleteLostParticles();
        }

        Info << "Average parcel mass: " << averageParcelMass_ << endl;
    }

    if (this->solution().resetSourcesOnStartup())
    {
        CloudType::resetSourceTerms();
    }
}


template<class CloudType>
CML::SprayCloud<CloudType>::SprayCloud
(
    SprayCloud<CloudType>& c,
    const word& name
)
:
    CloudType(c, name),
    sprayCloud(),
    cloudCopyPtr_(nullptr),
    averageParcelMass_(c.averageParcelMass_),
    atomizationModel_(c.atomizationModel_->clone()),
    breakupModel_(c.breakupModel_->clone())
{}


template<class CloudType>
CML::SprayCloud<CloudType>::SprayCloud
(
    const fvMesh& mesh,
    const word& name,
    const SprayCloud<CloudType>& c
)
:
    CloudType(mesh, name, c),
    sprayCloud(),
    cloudCopyPtr_(nullptr),
    averageParcelMass_(0.0),
    atomizationModel_(nullptr),
    breakupModel_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::SprayCloud<CloudType>::~SprayCloud()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::SprayCloud<CloudType>::setParcelThermoProperties
(
    parcelType& parcel,
    const scalar lagrangianDt
)
{
    CloudType::setParcelThermoProperties(parcel, lagrangianDt);

    const liquidMixtureProperties& liqMix = this->composition().liquids();

    const scalarField& Y(parcel.Y());
    scalarField X(liqMix.X(Y));
    const scalar pc = this->p()[parcel.cell()];

    // override rho and Cp from constantProperties
    parcel.Cp() = liqMix.Cp(pc, parcel.T(), X);
    parcel.rho() = liqMix.rho(pc, parcel.T(), X);
    parcel.sigma() = liqMix.sigma(pc, parcel.T(), X);
    parcel.mu() = liqMix.mu(pc, parcel.T(), X);
}


template<class CloudType>
void CML::SprayCloud<CloudType>::checkParcelProperties
(
    parcelType& parcel,
    const scalar lagrangianDt,
    const bool fullyDescribed
)
{
    CloudType::checkParcelProperties(parcel, lagrangianDt, fullyDescribed);

    // store the injection position and initial drop size
    parcel.position0() = parcel.position();
    parcel.d0() = parcel.d();

    parcel.y() = breakup().y0();
    parcel.yDot() = breakup().yDot0();

    parcel.liquidCore() = atomization().initLiquidCore();
}


template<class CloudType>
void CML::SprayCloud<CloudType>::storeState()
{
    cloudCopyPtr_.reset
    (
        static_cast<SprayCloud<CloudType>*>
        (
            clone(this->name() + "Copy").ptr()
        )
    );
}


template<class CloudType>
void CML::SprayCloud<CloudType>::restoreState()
{
    cloudReset(cloudCopyPtr_());
    cloudCopyPtr_.clear();
}


template<class CloudType>
void CML::SprayCloud<CloudType>::evolve()
{
    if (this->solution().canEvolve())
    {
        typename parcelType::trackingData td(*this);

        this->solve(*this, td);
    }
}


template<class CloudType>
void CML::SprayCloud<CloudType>::info()
{
    CloudType::info();
    scalar d32 = 1.0e+6*this->Dij(3, 2);
    scalar d10 = 1.0e+6*this->Dij(1, 0);
    scalar dMax = 1.0e+6*this->Dmax();
    scalar pen = this->penetration(0.95);

    Info << "    D10, D32, Dmax (mu)             = " << d10 << ", " << d32
         << ", " << dMax << nl
         << "    Liquid penetration 95% mass (m) = " << pen << endl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
