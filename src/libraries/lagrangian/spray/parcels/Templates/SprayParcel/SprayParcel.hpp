/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
Copyright (C) 2016 OpenCFD Ltd.
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
    CML::SprayParcel

Description
    Reacting spray parcel, with added functionality for atomization and breakup

\*---------------------------------------------------------------------------*/

#ifndef SprayParcel_H
#define SprayParcel_H

#include "particle.hpp"
#include "CompositionModel.hpp"
#include "AtomizationModel.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class ParcelType>
class SprayParcel;

template<class ParcelType>
Ostream& operator<<
(
    Ostream&,
    const SprayParcel<ParcelType>&
);

/*---------------------------------------------------------------------------*\
                          Class SprayParcel Declaration
\*---------------------------------------------------------------------------*/

template<class ParcelType>
class SprayParcel
:
    public ParcelType
{
    // Private data

        //- Size in bytes of the fields
        static const std::size_t sizeofFields_;


public:

    //- Class to hold reacting particle constant properties
    class constantProperties
    :
        public ParcelType::constantProperties
    {
        // Private data

            //- Particle initial surface tension [N/m]
            demandDrivenEntry<scalar> sigma0_;

            //- Particle initial dynamic viscosity [Pa.s]
            demandDrivenEntry<scalar> mu0_;


    public:

        // Constructors

            //- Null constructor
            constantProperties();

            //- Copy constructor
            constantProperties(const constantProperties& cp);

            //- Construct from dictionary
            constantProperties(const dictionary& parentDict);

            //- Construct from components
            constantProperties
            (
                const label parcelTypeId,
                const scalar rhoMin,
                const scalar rho0,
                const scalar minParcelMass,
                const scalar youngsModulus,
                const scalar poissonsRatio,
                const scalar T0,
                const scalar TMin,
                const scalar TMax,
                const scalar Cp0,
                const scalar epsilon0,
                const scalar f0,
                const scalar Pr,
                const scalar pMin,
                const Switch& constantVolume,
                const scalar sigma0,
                const scalar mu0
            );


        // Access

            //- Return const access to the initial surface tension
            inline scalar sigma0() const;

            //- Return const access to the initial dynamic viscosity
            inline scalar mu0() const;
    };


    //- Use base tracking data
    typedef typename ParcelType::trackingData trackingData;


protected:

    // Protected data

        // Spray parcel properties

            //- Initial droplet diameter
            scalar d0_;

            //- Injection position
            vector position0_;

            //- Liquid surface tension [N/m]
            scalar sigma_;

            //- Liquid dynamic viscosity [Pa.s]
            scalar mu_;

            //- Part of liquid core ( >0.5=liquid, <0.5=droplet )
            scalar liquidCore_;

            //- Index for KH Breakup
            scalar KHindex_;

            //- Spherical deviation
            scalar y_;

            //- Rate of change of spherical deviation
            scalar yDot_;

            //- Characteristic time (used in atomization and/or breakup model)
            scalar tc_;

            //- Stripped parcel mass due to breakup
            scalar ms_;

            //- Injected from injector (needed e.g. for calculating distance
            //  from injector)
            scalar injector_;

            //- Momentum relaxation time (needed for calculating parcel acc.)
            scalar tMom_;

            //- Passive scalar (extra variable to be defined by user)
            scalar user_;


public:

    // Static data members

        //- Runtime type information
        TypeName("SprayParcel");


    // Constructors

        //- Construct from mesh, coordinates and topology
        //  Other properties initialised as null
        inline SprayParcel
        (
            const polyMesh& mesh,
            const barycentric& coordinates,
            const label celli,
            const label tetFacei,
            const label tetPti
        );

        //- Construct from a position and a cell, searching for the rest of the
        //  required topology. Other properties are initialised as null.
        inline SprayParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label celli
        );

        //- Construct from components
        inline SprayParcel
        (
            const polyMesh& mesh,
            const barycentric& coordinates,
            const label celli,
            const label tetFacei,
            const label tetPti,
            const label typeId,
            const scalar nParticle0,
            const scalar d0,
            const scalar dTarget0,
            const vector& U0,
            const vector& f0,
            const vector& angularMomentum0,
            const vector& torque0,
            const scalarField& Y0,
            const scalar liquidCore,
            const scalar KHindex,
            const scalar y,
            const scalar yDot,
            const scalar tc,
            const scalar ms,
            const scalar injector,
            const scalar tMom,
            const scalar user,
            const typename ParcelType::constantProperties& constProps
        );

        //- Construct from Istream
        SprayParcel
        (
            const polyMesh& mesh,
            Istream& is,
            bool readFields = true
        );

        //- Construct as a copy
        SprayParcel
        (
            const SprayParcel& p,
            const polyMesh& mesh
        );

        //- Construct as a copy
        SprayParcel(const SprayParcel& p);

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone() const
        {
            return autoPtr<particle>(new SprayParcel<ParcelType>(*this));
        }

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone(const polyMesh& mesh) const
        {
            return autoPtr<particle>
            (
                new SprayParcel<ParcelType>(*this, mesh)
            );
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

            autoPtr<SprayParcel<ParcelType> > operator()(Istream& is) const
            {
                return autoPtr<SprayParcel<ParcelType> >
                (
                    new SprayParcel<ParcelType>(mesh_, is, true)
                );
            }
        };


    // Member Functions

        // Access

            //- Return const access to initial droplet diameter
            inline scalar d0() const;

            //- Return const access to initial droplet position
            inline const vector& position0() const;

            //- Return const access to the liquid surface tension
            inline scalar sigma() const;

            //- Return const access to the liquid dynamic viscosity
            inline scalar mu() const;

            //- Return const access to liquid core
            inline scalar liquidCore() const;

            //- Return const access to Kelvin-Helmholtz breakup index
            inline scalar KHindex() const;

            //- Return const access to spherical deviation
            inline scalar y() const;

            //- Return const access to rate of change of spherical deviation
            inline scalar yDot() const;

            //- Return const access to atomization characteristic time
            inline scalar tc() const;

            //- Return const access to stripped parcel mass
            inline scalar ms() const;

            //- Return const access to injector id
            inline scalar injector() const;

            //- Return const access to momentum relaxation time
            inline scalar tMom() const;

            //- Return const access to passive user scalar
            inline scalar user() const;


        // Edit

            //- Return access to initial droplet diameter
            inline scalar& d0();

            //- Return access to initial droplet position
            inline vector& position0();

            //- Return access to the liquid surface tension
            inline scalar& sigma();

            //- Return access to the liquid dynamic viscosity
            inline scalar& mu();

            //- Return access to liquid core
            inline scalar& liquidCore();

            //- Return access to Kelvin-Helmholtz breakup index
            inline scalar& KHindex();

            //- Return access to spherical deviation
            inline scalar& y();

            //- Return access to rate of change of spherical deviation
            inline scalar& yDot();

            //- Return access to atomization characteristic time
            inline scalar& tc();

            //- Return access to stripped parcel mass
            inline scalar& ms();

            //- Return access to injector id
            inline scalar& injector();

            //- Return access to momentum relaxation time
            inline scalar& tMom();

            //- Return access to passive user scalar
            inline scalar& user();


        // Main calculation loop

            //- Set cell values
            template<class TrackCloudType>
            void setCellValues(TrackCloudType& cloud, trackingData& td);

            //- Correct parcel properties according to atomization model
            template<class TrackCloudType>
            void calcAtomization
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar dt
            );

            //- Correct parcel properties according to breakup model
            template<class TrackCloudType>
            void calcBreakup
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar dt
            );

            //- Correct cell values using latest transfer information
            template<class TrackCloudType>
            void cellValueSourceCorrection
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar dt
            );

            //- Correct surface values due to emitted species
            template<class TrackCloudType>
            void correctSurfaceValues
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar T,
                const scalarField& Cs,
                scalar& rhos,
                scalar& mus,
                scalar& Pr,
                scalar& kappa
            );

            //- Update parcel properties over the time interval
            template<class TrackCloudType>
            void calc
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar dt
            );

            //- Calculate the chi-factor for flash-boiling for the
            //  atomization model
            template<class TrackCloudType>
            scalar chi
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalarField& X
            ) const;

            //- Solve the TAB equation
            template<class TrackCloudType>
            void solveTABEq
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar dt
            );


        // I-O

            //- Read
            template<class CloudType, class CompositionType>
            static void readFields
            (
                CloudType& c,
                const CompositionType& compModel
            );

            //- Read - no composition
            template<class CloudType>
            static void readFields(CloudType& c);

            //- Write
            template<class CloudType, class CompositionType>
            static void writeFields
            (
                const CloudType& c,
                const CompositionType& compModel
            );

            //- Write - composition supplied
            template<class CloudType>
            static void writeFields(const CloudType& c);


    // Ostream Operator

        friend Ostream& operator<< <ParcelType>
        (
            Ostream&,
            const SprayParcel<ParcelType>&
        );
};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
template<class ParcelType>
inline CML::SprayParcel<ParcelType>::constantProperties::constantProperties()
:
    ParcelType::constantProperties(),
    sigma0_(this->dict_, 0.0),
    mu0_(this->dict_, 0.0)
{}


template<class ParcelType>
inline CML::SprayParcel<ParcelType>::constantProperties::constantProperties
(
    const constantProperties& cp
)
:
    ParcelType::constantProperties(cp),
    sigma0_(cp.sigma0_),
    mu0_(cp.mu0_)
{}


template<class ParcelType>
inline CML::SprayParcel<ParcelType>::constantProperties::constantProperties
(
    const dictionary& parentDict
)
:
    ParcelType::constantProperties(parentDict),
    sigma0_(this->dict_, "sigma0"),
    mu0_(this->dict_, "mu0")
{}


template<class ParcelType>
inline CML::SprayParcel<ParcelType>::constantProperties::constantProperties
(
    const label parcelTypeId,
    const scalar rhoMin,
    const scalar rho0,
    const scalar minParcelMass,
    const scalar youngsModulus,
    const scalar poissonsRatio,
    const scalar T0,
    const scalar TMin,
    const scalar TMax,
    const scalar Cp0,
    const scalar epsilon0,
    const scalar f0,
    const scalar Pr,
    const scalar pMin,
    const Switch& constantVolume,
    const scalar sigma0,
    const scalar mu0
)
:
    ParcelType::constantProperties
    (
        parcelTypeId,
        rhoMin,
        rho0,
        minParcelMass,
        youngsModulus,
        poissonsRatio,
        T0,
        TMin,
        TMax,
        Cp0,
        epsilon0,
        f0,
        Pr,
        pMin,
        constantVolume
    ),
    sigma0_(this->dict_, sigma0),
    mu0_(this->dict_, mu0)
{}


template<class ParcelType>
inline CML::SprayParcel<ParcelType>::SprayParcel
(
    const polyMesh& mesh,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti
)
:
    ParcelType(mesh, coordinates, celli, tetFacei, tetPti),
    d0_(this->d()),
    position0_(this->position()),
    sigma_(0.0),
    mu_(0.0),
    liquidCore_(0.0),
    KHindex_(0.0),
    y_(0.0),
    yDot_(0.0),
    tc_(0.0),
    ms_(0.0),
    injector_(1.0),
    tMom_(GREAT),
    user_(0.0)
{}


template<class ParcelType>
inline CML::SprayParcel<ParcelType>::SprayParcel
(
    const polyMesh& mesh,
    const vector& position,
    const label celli
)
:
    ParcelType(mesh, position, celli),
    d0_(this->d()),
    position0_(this->position()),
    sigma_(0.0),
    mu_(0.0),
    liquidCore_(0.0),
    KHindex_(0.0),
    y_(0.0),
    yDot_(0.0),
    tc_(0.0),
    ms_(0.0),
    injector_(1.0),
    tMom_(GREAT),
    user_(0.0)
{}


template<class ParcelType>
inline CML::SprayParcel<ParcelType>::SprayParcel
(
    const polyMesh& mesh,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti,
    const label typeId,
    const scalar nParticle0,
    const scalar d0,
    const scalar dTarget0,
    const vector& U0,
    const vector& f0,
    const vector& angularMomentum0,
    const vector& torque0,
    const scalarField& Y0,
    const scalar liquidCore,
    const scalar KHindex,
    const scalar y,
    const scalar yDot,
    const scalar tc,
    const scalar ms,
    const scalar injector,
    const scalar tMom,
    const scalar user,
    const typename ParcelType::constantProperties& constProps
)
:
    ParcelType
    (
        mesh,
        coordinates,
        celli,
        tetFacei,
        tetPti,
        typeId,
        nParticle0,
        d0,
        dTarget0,
        U0,
        f0,
        angularMomentum0,
        torque0,
        Y0,
        constProps
    ),
    d0_(d0),
    position0_(this->position()),
    sigma_(constProps.sigma0()),
    mu_(constProps.mu0()),
    liquidCore_(liquidCore),
    KHindex_(KHindex),
    y_(y),
    yDot_(yDot),
    tc_(tc),
    ms_(ms),
    injector_(injector),
    tMom_(tMom),
    user_(user)
{}


// * * * * * * * * * constantProperties Member Functions * * * * * * * * * * //

template<class ParcelType>
inline CML::scalar
CML::SprayParcel<ParcelType>::constantProperties::sigma0() const
{
    return sigma0_.value();
}


template<class ParcelType>
inline CML::scalar
CML::SprayParcel<ParcelType>::constantProperties::mu0() const
{
    return mu0_.value();
}


// * * * * * * * * * * SprayParcel Member Functions * * * * * * * * * * * * //

template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::d0() const
{
    return d0_;
}


template<class ParcelType>
inline const CML::vector& CML::SprayParcel<ParcelType>::position0() const
{
    return position0_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::sigma() const
{
    return sigma_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::mu() const
{
    return mu_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::liquidCore() const
{
    return liquidCore_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::KHindex() const
{
    return KHindex_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::y() const
{
    return y_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::yDot() const
{
    return yDot_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::tc() const
{
    return tc_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::ms() const
{
    return ms_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::injector() const
{
    return injector_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::tMom() const
{
    return tMom_;
}


template<class ParcelType>
inline CML::scalar CML::SprayParcel<ParcelType>::user() const
{
    return user_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::d0()
{
    return d0_;
}


template<class ParcelType>
inline CML::vector& CML::SprayParcel<ParcelType>::position0()
{
    return position0_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::sigma()
{
    return sigma_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::mu()
{
    return mu_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::liquidCore()
{
    return liquidCore_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::KHindex()
{
    return KHindex_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::y()
{
    return y_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::yDot()
{
    return yDot_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::tc()
{
    return tc_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::ms()
{
    return ms_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::injector()
{
    return injector_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::tMom()
{
    return tMom_;
}


template<class ParcelType>
inline CML::scalar& CML::SprayParcel<ParcelType>::user()
{
    return user_;
}


// * * * * * * * * * * *  Protected Member Functions * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackCloudType>
void CML::SprayParcel<ParcelType>::setCellValues
(
    TrackCloudType& cloud,
    trackingData& td
)
{
    ParcelType::setCellValues(cloud, td);
}


template<class ParcelType>
template<class TrackCloudType>
void CML::SprayParcel<ParcelType>::cellValueSourceCorrection
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    ParcelType::cellValueSourceCorrection(cloud, td, dt);
}


template<class ParcelType>
template<class TrackCloudType>
void CML::SprayParcel<ParcelType>::calc
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    typedef typename TrackCloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        cloud.composition();

    // Check if parcel belongs to liquid core
    if (liquidCore() > 0.5)
    {
        // Liquid core parcels should not experience coupled forces
        cloud.forces().setCalcCoupled(false);
    }

    // Get old mixture composition
    scalarField X0(composition.liquids().X(this->Y()));

    // Check if we have critical or boiling conditions
    scalar TMax = composition.liquids().Tc(X0);
    const scalar T0 = this->T();
    const scalar pc0 = td.pc();
    if (composition.liquids().pv(pc0, T0, X0) >= pc0*0.999)
    {
        // Set TMax to boiling temperature
        TMax = composition.liquids().pvInvert(pc0, X0);
    }

    // Set the maximum temperature limit
    cloud.constProps().setTMax(TMax);

    // Store the parcel properties
    this->Cp() = composition.liquids().Cp(pc0, T0, X0);
    sigma_ = composition.liquids().sigma(pc0, T0, X0);
    const scalar rho0 = composition.liquids().rho(pc0, T0, X0);
    this->rho() = rho0;
    const scalar mass0 = this->mass();
    mu_ = composition.liquids().mu(pc0, T0, X0);

    ParcelType::calc(cloud, td, dt);

    if (td.keepParticle)
    {
        // Reduce the stripped parcel mass due to evaporation
        // assuming the number of particles remains unchanged
        this->ms() -= this->ms()*(mass0 - this->mass())/mass0;

        // Update Cp, sigma, density and diameter due to change in temperature
        // and/or composition
        scalar T1 = this->T();
        scalarField X1(composition.liquids().X(this->Y()));

        this->Cp() = composition.liquids().Cp(td.pc(), T1, X1);

        sigma_ = composition.liquids().sigma(td.pc(), T1, X1);

        scalar rho1 = composition.liquids().rho(td.pc(), T1, X1);
        this->rho() = rho1;

        mu_ = composition.liquids().mu(td.pc(), T1, X1);

        scalar d1 = this->d()*cbrt(rho0/rho1);
        this->d() = d1;

        if (liquidCore() > 0.5)
        {
            calcAtomization(cloud, td, dt);

            // Preserve the total mass/volume by increasing the number of
            // particles in parcels due to breakup
            scalar d2 = this->d();
            this->nParticle() *= pow3(d1/d2);
        }
        else
        {
            calcBreakup(cloud, td, dt);
        }
    }

    // Restore coupled forces
    cloud.forces().setCalcCoupled(true);
}


template<class ParcelType>
template<class TrackCloudType>
void CML::SprayParcel<ParcelType>::calcAtomization
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    typedef typename TrackCloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        cloud.composition();

    typedef typename TrackCloudType::sprayCloudType sprayCloudType;
    const AtomizationModel<sprayCloudType>& atomization =
        cloud.atomization();

    // Average molecular weight of carrier mix - assumes perfect gas
    scalar Wc = td.rhoc()*RR*td.Tc()/td.pc();
    scalar R = RR/Wc;
    scalar Tav = atomization.Taverage(this->T(), td.Tc());

    // Calculate average gas density based on average temperature
    scalar rhoAv = td.pc()/(R*Tav);

    scalar soi = cloud.injectors().timeStart();
    scalar currentTime = cloud.db().time().value();
    const vector& pos = this->position();
    const vector& injectionPos = this->position0();

    // Disregard the continous phase when calculating the relative velocity
    // (in line with the deactivated coupled assumption)
    scalar Urel = mag(this->U());

    scalar t0 = max(0.0, currentTime - this->age() - soi);
    scalar t1 = min(t0 + dt, cloud.injectors().timeEnd() - soi);

    // This should be the vol flow rate from when the parcel was injected
    scalar volFlowRate = cloud.injectors().volumeToInject(t0, t1)/dt;

    scalar chi = 0.0;
    if (atomization.calcChi())
    {
        chi = this->chi(cloud, td, composition.liquids().X(this->Y()));
    }

    atomization.update
    (
        dt,
        this->d(),
        this->liquidCore(),
        this->tc(),
        this->rho(),
        mu_,
        sigma_,
        volFlowRate,
        rhoAv,
        Urel,
        pos,
        injectionPos,
        cloud.pAmbient(),
        chi,
        cloud.rndGen()
    );
}


template<class ParcelType>
template<class TrackCloudType>
void CML::SprayParcel<ParcelType>::calcBreakup
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    const typename TrackCloudType::parcelType& p =
        static_cast<const typename TrackCloudType::parcelType&>(*this);
    typename TrackCloudType::parcelType::trackingData& ttd =
        static_cast<typename TrackCloudType::parcelType::trackingData&>(td);

    const typename TrackCloudType::forceType& forces = cloud.forces();

    if (cloud.breakup().solveOscillationEq())
    {
        solveTABEq(cloud, td, dt);
    }

    // Average molecular weight of carrier mix - assumes perfect gas
    scalar Wc = td.rhoc()*RR*td.Tc()/td.pc();
    scalar R = RR/Wc;
    scalar Tav = cloud.atomization().Taverage(this->T(), td.Tc());

    // Calculate average gas density based on average temperature
    scalar rhoAv = td.pc()/(R*Tav);
    scalar muAv = td.muc();
    vector Urel = this->U() - td.Uc();
    scalar Urmag = mag(Urel);
    scalar Re = this->Re(rhoAv, this->U(), td.Uc(), this->d(), muAv);

    const scalar mass = p.mass();
    const forceSuSp Fcp = forces.calcCoupled(p, ttd, dt, mass, Re, muAv);
    const forceSuSp Fncp = forces.calcNonCoupled(p, ttd, dt, mass, Re, muAv);
    this->tMom() = mass/(Fcp.Sp() + Fncp.Sp() + ROOTVSMALL);

    const vector g = cloud.g().value();

    scalar parcelMassChild = 0.0;
    scalar dChild = 0.0;
    if
    (
        cloud.breakup().update
        (
            dt,
            g,
            this->d(),
            this->tc(),
            this->ms(),
            this->nParticle(),
            this->KHindex(),
            this->y(),
            this->yDot(),
            this->d0(),
            this->rho(),
            mu_,
            sigma_,
            this->U(),
            rhoAv,
            muAv,
            Urel,
            Urmag,
            this->tMom(),
            dChild,
            parcelMassChild
        )
    )
    {
        scalar Re = rhoAv*Urmag*dChild/muAv;

        // Add child parcel as copy of parent
        SprayParcel<ParcelType>* child = new SprayParcel<ParcelType>(*this);
        child->origId() = this->getNewParticleID();
        child->d() = dChild;
        child->d0() = dChild;
        const scalar massChild = child->mass();
        child->mass0() = massChild;
        child->nParticle() = parcelMassChild/massChild;

        const forceSuSp Fcp =
            forces.calcCoupled(*child, ttd, dt, massChild, Re, muAv);
        const forceSuSp Fncp =
            forces.calcNonCoupled(*child, ttd, dt, massChild, Re, muAv);

        child->age() = 0.0;
        child->liquidCore() = 0.0;
        child->KHindex() = 1.0;
        child->y() = cloud.breakup().y0();
        child->yDot() = cloud.breakup().yDot0();
        child->tc() = 0.0;
        child->ms() = -GREAT;
        child->injector() = this->injector();
        child->tMom() = massChild/(Fcp.Sp() + Fncp.Sp());
        child->user() = 0.0;
        child->calcDispersion(cloud, td, dt);

        cloud.addParticle(child);
    }
}


template<class ParcelType>
template<class TrackCloudType>
CML::scalar CML::SprayParcel<ParcelType>::chi
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalarField& X
) const
{
    // Modifications to take account of the flash boiling on primary break-up

    typedef typename TrackCloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        cloud.composition();

    scalar chi = 0.0;
    scalar T0 = this->T();
    scalar p0 = td.pc();
    scalar pAmb = cloud.pAmbient();

    scalar pv = composition.liquids().pv(p0, T0, X);

    forAll(composition.liquids(), i)
    {
        if (pv >= 0.999*pAmb)
        {
            // Liquid is boiling - calc boiling temperature

            const liquidProperties& liq = composition.liquids().properties()[i];
            scalar TBoil = liq.pvInvert(p0);

            scalar hl = liq.hl(pAmb, TBoil);
            scalar iTp = liq.h(pAmb, T0) - pAmb/liq.rho(pAmb, T0);
            scalar iTb = liq.h(pAmb, TBoil) - pAmb/liq.rho(pAmb, TBoil);

            chi += X[i]*(iTp - iTb)/hl;
        }
    }

    chi = min(1.0, max(chi, 0.0));

    return chi;
}


template<class ParcelType>
template<class TrackCloudType>
void CML::SprayParcel<ParcelType>::solveTABEq
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    const scalar& TABCmu = cloud.breakup().TABCmu();
    const scalar& TABtwoWeCrit = cloud.breakup().TABtwoWeCrit();
    const scalar& TABComega = cloud.breakup().TABComega();

    scalar r = 0.5*this->d();
    scalar r2 = r*r;
    scalar r3 = r*r2;

    // Inverse of characteristic viscous damping time
    scalar rtd = 0.5*TABCmu*mu_/(this->rho()*r2);

    // Oscillation frequency (squared)
    scalar omega2 = TABComega*sigma_/(this->rho()*r3) - rtd*rtd;

    if (omega2 > 0)
    {
        scalar omega = sqrt(omega2);
        scalar We =
            this->We(td.rhoc(), this->U(), td.Uc(), r, sigma_)/TABtwoWeCrit;

        // Initial values for y and yDot
        scalar y0 = this->y() - We;
        scalar yDot0 = this->yDot() + y0*rtd;

        // Update distortion parameters
        scalar c = cos(omega*dt);
        scalar s = sin(omega*dt);
        scalar e = exp(-rtd*dt);

        this->y() = We + e*(y0*c + (yDot0/omega)*s);
        this->yDot() = (We - this->y())*rtd + e*(yDot0*c - omega*y0*s);
    }
    else
    {
        // Reset distortion parameters
        this->y() = 0;
        this->yDot() = 0;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::SprayParcel<ParcelType>::SprayParcel(const SprayParcel<ParcelType>& p)
:
    ParcelType(p),
    d0_(p.d0_),
    position0_(p.position0_),
    sigma_(p.sigma_),
    mu_(p.mu_),
    liquidCore_(p.liquidCore_),
    KHindex_(p.KHindex_),
    y_(p.y_),
    yDot_(p.yDot_),
    tc_(p.tc_),
    ms_(p.ms_),
    injector_(p.injector_),
    tMom_(p.tMom_),
    user_(p.user_)
{}


template<class ParcelType>
CML::SprayParcel<ParcelType>::SprayParcel
(
    const SprayParcel<ParcelType>& p,
    const polyMesh& mesh
)
:
    ParcelType(p, mesh),
    d0_(p.d0_),
    position0_(p.position0_),
    sigma_(p.sigma_),
    mu_(p.mu_),
    liquidCore_(p.liquidCore_),
    KHindex_(p.KHindex_),
    y_(p.y_),
    yDot_(p.yDot_),
    tc_(p.tc_),
    ms_(p.ms_),
    injector_(p.injector_),
    tMom_(p.tMom_),
    user_(p.user_)
{}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
template<class ParcelType>
const std::size_t CML::SprayParcel<ParcelType>::sizeofFields_
(
    sizeof(SprayParcel<ParcelType>) - sizeof(ParcelType)
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::SprayParcel<ParcelType>::SprayParcel
(
    const polyMesh& mesh,
    Istream& is,
    bool readFields
)
:
    ParcelType(mesh, is, readFields),
    d0_(0.0),
    position0_(Zero),
    sigma_(0.0),
    mu_(0.0),
    liquidCore_(0.0),
    KHindex_(0.0),
    y_(0.0),
    yDot_(0.0),
    tc_(0.0),
    ms_(0.0),
    injector_(1.0),
    tMom_(GREAT),
    user_(0.0)
{
    if (readFields)
    {
        if (is.format() == IOstream::ASCII)
        {
            d0_ = readScalar(is);
            is >> position0_;
            sigma_ = readScalar(is);
            mu_ = readScalar(is);
            liquidCore_ = readScalar(is);
            KHindex_ = readScalar(is);
            y_ = readScalar(is);
            yDot_ = readScalar(is);
            tc_ = readScalar(is);
            ms_ = readScalar(is);
            injector_ = readScalar(is);
            tMom_ = readScalar(is);
            user_ = readScalar(is);
        }
        else
        {
            is.read(reinterpret_cast<char*>(&d0_), sizeofFields_);
        }
    }

    // Check state of Istream
    is.check
    (
        "SprayParcel<ParcelType>::SprayParcel"
        "("
            "const polyMesh, "
            "Istream&, "
            "bool"
        ")"
    );
}


template<class ParcelType>
template<class CloudType>
void CML::SprayParcel<ParcelType>::readFields(CloudType& c)
{
    if (!c.size())
    {
        return;
    }

    ParcelType::readFields(c);
}


template<class ParcelType>
template<class CloudType, class CompositionType>
void CML::SprayParcel<ParcelType>::readFields
(
    CloudType& c,
    const CompositionType& compModel
)
{
    if (!c.size())
    {
        return;
    }

    ParcelType::readFields(c, compModel);

    IOField<scalar> d0(c.fieldIOobject("d0", IOobject::MUST_READ));
    c.checkFieldIOobject(c, d0);

    IOField<vector> position0
    (
        c.fieldIOobject("position0", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, position0);

    IOField<scalar> sigma(c.fieldIOobject("sigma", IOobject::MUST_READ));
    c.checkFieldIOobject(c, sigma);

    IOField<scalar> mu(c.fieldIOobject("mu", IOobject::MUST_READ));
    c.checkFieldIOobject(c, mu);

    IOField<scalar> liquidCore
    (
        c.fieldIOobject("liquidCore", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, liquidCore);

    IOField<scalar> KHindex
    (
        c.fieldIOobject("KHindex", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, KHindex);

    IOField<scalar> y
    (
        c.fieldIOobject("y", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, y);

    IOField<scalar> yDot
    (
        c.fieldIOobject("yDot", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, yDot);

    IOField<scalar> tc
    (
        c.fieldIOobject("tc", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, tc);

    IOField<scalar> ms
    (
        c.fieldIOobject("ms", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, ms);

    IOField<scalar> injector
    (
        c.fieldIOobject("injector", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, injector);

    IOField<scalar> tMom
    (
        c.fieldIOobject("tMom", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, tMom);

    IOField<scalar> user
    (
        c.fieldIOobject("user", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, user);

    label i = 0;
    forAllIter(typename Cloud<SprayParcel<ParcelType> >, c, iter)
    {
        SprayParcel<ParcelType>& p = iter();
        p.d0_ = d0[i];
        p.position0_ = position0[i];
        p.sigma_ = sigma[i];
        p.mu_ = mu[i];
        p.liquidCore_ = liquidCore[i];
        p.KHindex_ = KHindex[i];
        p.y_ = y[i];
        p.yDot_ = yDot[i];
        p.tc_ = tc[i];
        p.ms_ = ms[i];
        p.injector_ = injector[i];
        p.tMom_ = tMom[i];
        p.user_ = user[i];
        i++;
    }
}


template<class ParcelType>
template<class CloudType>
void CML::SprayParcel<ParcelType>::writeFields(const CloudType& c)
{
    ParcelType::writeFields(c);
}


template<class ParcelType>
template<class CloudType, class CompositionType>
void CML::SprayParcel<ParcelType>::writeFields
(
    const CloudType& c,
    const CompositionType& compModel
)
{
    ParcelType::writeFields(c, compModel);

    label np = c.size();

    IOField<scalar> d0(c.fieldIOobject("d0", IOobject::NO_READ), np);
    IOField<vector> position0
    (
        c.fieldIOobject("position0", IOobject::NO_READ),
        np
    );
    IOField<scalar> sigma(c.fieldIOobject("sigma", IOobject::NO_READ), np);
    IOField<scalar> mu(c.fieldIOobject("mu", IOobject::NO_READ), np);
    IOField<scalar> liquidCore
    (
        c.fieldIOobject("liquidCore", IOobject::NO_READ),
        np
    );
    IOField<scalar> KHindex(c.fieldIOobject("KHindex", IOobject::NO_READ), np);
    IOField<scalar> y(c.fieldIOobject("y", IOobject::NO_READ), np);
    IOField<scalar> yDot(c.fieldIOobject("yDot", IOobject::NO_READ), np);
    IOField<scalar> tc(c.fieldIOobject("tc", IOobject::NO_READ), np);
    IOField<scalar> ms(c.fieldIOobject("ms", IOobject::NO_READ), np);
    IOField<scalar> injector
    (
        c.fieldIOobject("injector", IOobject::NO_READ),
        np
    );
    IOField<scalar> tMom(c.fieldIOobject("tMom", IOobject::NO_READ), np);
    IOField<scalar> user(c.fieldIOobject("user", IOobject::NO_READ), np);

    label i = 0;
    forAllConstIter(typename Cloud<SprayParcel<ParcelType> >, c, iter)
    {
        const SprayParcel<ParcelType>& p = iter();
        d0[i] = p.d0_;
        position0[i] = p.position0_;
        sigma[i] = p.sigma_;
        mu[i] = p.mu_;
        liquidCore[i] = p.liquidCore_;
        KHindex[i] = p.KHindex_;
        y[i] = p.y_;
        yDot[i] = p.yDot_;
        tc[i] = p.tc_;
        ms[i] = p.ms_;
        injector[i] = p.injector_;
        tMom[i] = p.tMom_;
        user[i] = p.user_;
        i++;
    }

    d0.write();
    position0.write();
    sigma.write();
    mu.write();
    liquidCore.write();
    KHindex.write();
    y.write();
    yDot.write();
    tc.write();
    ms.write();
    injector.write();
    tMom.write();
    user.write();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class ParcelType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const SprayParcel<ParcelType>& p
)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const ParcelType&>(p)
        << token::SPACE << p.d0()
        << token::SPACE << p.position0()
        << token::SPACE << p.sigma()
        << token::SPACE << p.mu()
        << token::SPACE << p.liquidCore()
        << token::SPACE << p.KHindex()
        << token::SPACE << p.y()
        << token::SPACE << p.yDot()
        << token::SPACE << p.tc()
        << token::SPACE << p.ms()
        << token::SPACE << p.injector()
        << token::SPACE << p.tMom()
        << token::SPACE << p.user();
    }
    else
    {
        os  << static_cast<const ParcelType&>(p);
        os.write
        (
            reinterpret_cast<const char*>(&p.d0_),
            SprayParcel<ParcelType>::sizeofFields_
        );
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const SprayParcel<ParcelType>&)"
    );

    return os;
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
