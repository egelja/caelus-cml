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
#include "StochasticCollisionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
template<class CloudType>
class AtomizationModel;

template<class CloudType>
class BreakupModel;

template<class CloudType>
class StochasticCollisionModel;

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

            //- Collision model
            autoPtr<StochasticCollisionModel<SprayCloud<CloudType> > >
                stochasticCollisionModel_;


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

                //- Return const-access to the breakup model
                inline const StochasticCollisionModel<SprayCloud<CloudType> >&
                    stochasticCollision() const;

                //- Return reference to the breakup model
                inline StochasticCollisionModel<SprayCloud<CloudType> >&
                    stochasticCollision();


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
            //- Particle motion

            template<class TrackData>
            void motion(TrackData& td);


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
inline const CML::StochasticCollisionModel<CML::SprayCloud<CloudType> >&
CML::SprayCloud<CloudType>::stochasticCollision() const
{
    return stochasticCollisionModel_;
}


template<class CloudType>
inline CML::StochasticCollisionModel<CML::SprayCloud<CloudType> >&
CML::SprayCloud<CloudType>::stochasticCollision()
{
    return stochasticCollisionModel_();
}


template<class CloudType>
inline CML::scalar CML::SprayCloud<CloudType>::averageParcelMass() const
{
    return averageParcelMass_;
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

    stochasticCollisionModel_.reset
    (
        StochasticCollisionModel<SprayCloud<CloudType> >::New
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
    stochasticCollisionModel_.reset(c.stochasticCollisionModel_.ptr());
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
    cloudCopyPtr_(NULL),
    averageParcelMass_(0.0),
    atomizationModel_(NULL),
    breakupModel_(NULL),
    stochasticCollisionModel_(NULL)
{
    if (this->solution().active())
    {
        setModels();

        averageParcelMass_ = this->injection().averageParcelMass();

        if (readFields)
        {
            parcelType::readFields(*this, this->composition());
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
    cloudCopyPtr_(NULL),
    averageParcelMass_(c.averageParcelMass_),
    atomizationModel_(c.atomizationModel_->clone()),
    breakupModel_(c.breakupModel_->clone()),
    stochasticCollisionModel_(c.stochasticCollisionModel_->clone())
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
    cloudCopyPtr_(NULL),
    averageParcelMass_(0.0),
    atomizationModel_(NULL),
    breakupModel_(NULL),
    stochasticCollisionModel_(NULL)
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

    // override rho and Cp from constantProperties
    parcel.Cp() = liqMix.Cp(parcel.pc(), parcel.T(), X);
    parcel.rho() = liqMix.rho(parcel.pc(), parcel.T(), X);
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
        typename parcelType::template
            TrackingData<SprayCloud<CloudType> > td(*this);

        this->solve(td);
    }
}


template<class CloudType>
template<class TrackData>
void CML::SprayCloud<CloudType>::motion(TrackData& td)
{
    const scalar dt = this->solution().trackTime();

    td.part() = TrackData::tpLinearTrack;
    CloudType::move(td, dt);

    this->updateCellOccupancy();

    if (stochasticCollision().active())
    {
        const liquidMixtureProperties& liqMix = this->composition().liquids();

        label i = 0;
        forAllIter(typename SprayCloud<CloudType>, *this, iter)
        {
            label j = 0;
            forAllIter(typename SprayCloud<CloudType>, *this, jter)
            {
                if (j > i)
                {
                    parcelType& p = iter();
                    scalar Vi = this->mesh().V()[p.cell()];
                    scalarField X1(liqMix.X(p.Y()));
                    scalar sigma1 = liqMix.sigma(p.pc(), p.T(), X1);
                    scalar mp = p.mass()*p.nParticle();

                    parcelType& q = jter();
                    scalar Vj = this->mesh().V()[q.cell()];
                    scalarField X2(liqMix.X(q.Y()));
                    scalar sigma2 = liqMix.sigma(q.pc(), q.T(), X2);
                    scalar mq = q.mass()*q.nParticle();

                    bool updateProperties = stochasticCollision().update
                    (
                        dt,
                        this->rndGen(),
                        p.position(),
                        mp,
                        p.d(),
                        p.nParticle(),
                        p.U(),
                        p.rho(),
                        p.T(),
                        p.Y(),
                        sigma1,
                        p.cell(),
                        Vi,
                        q.position(),
                        mq,
                        q.d(),
                        q.nParticle(),
                        q.U(),
                        q.rho(),
                        q.T(),
                        q.Y(),
                        sigma2,
                        q.cell(),
                        Vj
                    );

                    // for coalescence we need to update the density and
                    // the diameter cause of the temp/conc/mass-change
                    if (updateProperties)
                    {
                        if (mp > VSMALL)
                        {
                            scalarField Xp(liqMix.X(p.Y()));
                            p.rho() = liqMix.rho(p.pc(), p.T(), Xp);
                            p.Cp() = liqMix.Cp(p.pc(), p.T(), Xp);
                            p.d() =
                                cbrt
                                (
                                    6.0*mp
                                   /(
                                        p.nParticle()
                                       *p.rho()
                                       *constant::mathematical::pi
                                    )
                                );
                        }

                        if (mq > VSMALL)
                        {
                            scalarField Xq(liqMix.X(q.Y()));
                            q.rho() = liqMix.rho(q.pc(), q.T(), Xq);
                            q.Cp() = liqMix.Cp(q.pc(), q.T(), Xq);
                            q.d() =
                                cbrt
                                (
                                    6.0*mq
                                   /(
                                        q.nParticle()
                                       *q.rho()
                                       *constant::mathematical::pi
                                    )
                                );
                        }
                    }
                }
                j++;
            }
            i++;
        }

        // remove coalesced particles (diameter set to 0)
        forAllIter(typename SprayCloud<CloudType>, *this, iter)
        {
            parcelType& p = iter();
            if (p.mass() < VSMALL)
            {
                this->deleteParticle(p);
            }
        }
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
