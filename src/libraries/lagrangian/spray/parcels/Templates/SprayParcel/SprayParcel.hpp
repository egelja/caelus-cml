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
    CML::SprayParcel

Description
    Reacing spray parcel, with added functionality for atomization and breakup

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

protected:

    // Protected data

        // Spray parcel properties

            //- Initial droplet diameter
            scalar d0_;

            //- Injection position
            vector position0_;

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

        //- String representation of properties
        static string propHeader;

        //- Runtime type information
        TypeName("SprayParcel");


    // Constructors

        //- Construct from owner, position, and cloud owner
        //  Other properties initialised as null
        inline SprayParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            const label tetFaceI,
            const label tetPtI
        );

        //- Construct from components
        inline SprayParcel
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
            template<class TrackData>
            void setCellValues
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );

            //- Correct parcel properties according to atomization model
            template<class TrackData>
            void calcAtomization
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );

            //- Correct parcel properties according to breakup model
            template<class TrackData>
            void calcBreakup
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );

            //- Correct cell values using latest transfer information
            template<class TrackData>
            void cellValueSourceCorrection
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );

            //- Correct surface values due to emitted species
            template<class TrackData>
            void correctSurfaceValues
            (
                TrackData& td,
                const label cellI,
                const scalar T,
                const scalarField& Cs,
                scalar& rhos,
                scalar& mus,
                scalar& Pr,
                scalar& kappa
            );

            //- Update parcel properties over the time interval
            template<class TrackData>
            void calc
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );

            //- Calculate the chi-factor for flash-boiling for the
            //  atomization model
            template<class TrackData>
            scalar chi
            (
                TrackData& td,
                const scalarField& X
            ) const;

            //- Solve the TAB equation
            template<class TrackData>
            void solveTABEq
            (
                TrackData& td,
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


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
inline CML::SprayParcel<ParcelType>::SprayParcel
(
    const polyMesh& mesh,
    const vector& position,
    const label cellI,
    const label tetFaceI,
    const label tetPtI
)
:
    ParcelType(mesh, position, cellI, tetFaceI, tetPtI),
    d0_(this->d()),
    position0_(position),
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
        Y0,
        constProps
    ),
    d0_(d0),
    position0_(position),
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
template<class TrackData>
void CML::SprayParcel<ParcelType>::setCellValues
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    ParcelType::setCellValues(td, dt, cellI);
}


template<class ParcelType>
template<class TrackData>
void CML::SprayParcel<ParcelType>::cellValueSourceCorrection
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    ParcelType::cellValueSourceCorrection(td, dt, cellI);
}


template<class ParcelType>
template<class TrackData>
void CML::SprayParcel<ParcelType>::calc
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    typedef typename TrackData::cloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        td.cloud().composition();

    // check if parcel belongs to liquid core
    if (liquidCore() > 0.5)
    {
        // liquid core parcels should not experience coupled forces
        td.cloud().forces().setCalcCoupled(false);
    }

    // get old mixture composition
    const scalarField& Y0(this->Y());
    scalarField X0(composition.liquids().X(Y0));

    // check if we have critical or boiling conditions
    scalar TMax = composition.liquids().Tc(X0);
    const scalar T0 = this->T();
    const scalar pc0 = this->pc_;
    if (composition.liquids().pv(pc0, T0, X0) >= pc0*0.999)
    {
        // set TMax to boiling temperature
        TMax = composition.liquids().pvInvert(pc0, X0);
    }

    // set the maximum temperature limit
    td.cloud().constProps().TMax() = TMax;

    this->Cp() = composition.liquids().Cp(pc0, T0, X0);
    scalar rho0 = composition.liquids().rho(pc0, T0, X0);
    this->rho() = rho0;

    ParcelType::calc(td, dt, cellI);

    if (td.keepParticle)
    {
        // update Cp, diameter and density due to change in temperature
        // and/or composition
        scalar T1 = this->T();
        const scalarField& Y1(this->Y());
        scalarField X1(composition.liquids().X(Y1));

        this->Cp() = composition.liquids().Cp(this->pc_, T1, X1);

        scalar rho1 = composition.liquids().rho(this->pc_, T1, X1);
        this->rho() = rho1;
        scalar d1 = this->d()*cbrt(rho0/rho1);
        this->d() = d1;

        if (liquidCore() > 0.5)
        {
            calcAtomization(td, dt, cellI);

            // preserve the total mass/volume by increasing the number of
            // particles in parcels due to breakup
            scalar d2 = this->d();
            this->nParticle() *= pow3(d1/d2);
        }
        else
        {
            calcBreakup(td, dt, cellI);
        }
    }

    // restore coupled forces
    td.cloud().forces().setCalcCoupled(true);
}


template<class ParcelType>
template<class TrackData>
void CML::SprayParcel<ParcelType>::calcAtomization
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    typedef typename TrackData::cloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        td.cloud().composition();

    typedef typename TrackData::cloudType::sprayCloudType sprayCloudType;
    const AtomizationModel<sprayCloudType>& atomization =
        td.cloud().atomization();


    // cell state info is updated in ReactingParcel calc
    const scalarField& Y(this->Y());
    scalarField X(composition.liquids().X(Y));

    scalar rho = composition.liquids().rho(this->pc(), this->T(), X);
    scalar mu = composition.liquids().mu(this->pc(), this->T(), X);
    scalar sigma = composition.liquids().sigma(this->pc(), this->T(), X);

    // Average molecular weight of carrier mix - assumes perfect gas
    scalar Wc = this->rhoc_*specie::RR*this->Tc()/this->pc();
    scalar R = specie::RR/Wc;
    scalar Tav = atomization.Taverage(this->T(), this->Tc());

    // calculate average gas density based on average temperature
    scalar rhoAv = this->pc()/(R*Tav);

    scalar soi = td.cloud().injection().timeStart();
    scalar currentTime = td.cloud().db().time().value();
    const vector& pos = this->position();
    const vector& injectionPos = this->position0();

    // disregard the continous phase when calculating the relative velocity
    // (in line with the deactivated coupled assumption)
    scalar Urel = mag(this->U());

    scalar t0 = max(0.0, currentTime - this->age() - soi);
    scalar t1 = min(t0 + dt, td.cloud().injection().timeEnd() - soi);
    // this should be the vol flow rate from when the parcel was injected
    scalar volFlowRate = td.cloud().injection().volumeToInject(t0, t1)/dt;

    scalar chi = 0.0;
    if (atomization.calcChi())
    {
        chi = this->chi(td, X);
    }

    atomization.update
    (
        dt,
        this->d(),
        this->liquidCore(),
        this->tc(),
        rho,
        mu,
        sigma,
        volFlowRate,
        rhoAv,
        Urel,
        pos,
        injectionPos,
        td.cloud().pAmbient(),
        chi,
        td.cloud().rndGen()
    );
}


template<class ParcelType>
template<class TrackData>
void CML::SprayParcel<ParcelType>::calcBreakup
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    typedef typename TrackData::cloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        td.cloud().composition();

    typedef typename TrackData::cloudType cloudType;
    typedef typename cloudType::parcelType parcelType;
    typedef typename cloudType::forceType forceType;

    const parcelType& p = static_cast<const parcelType&>(*this);
    const forceType& forces = td.cloud().forces();

    if (td.cloud().breakup().solveOscillationEq())
    {
        solveTABEq(td, dt);
    }

    // cell state info is updated in ReactingParcel calc
    const scalarField& Y(this->Y());
    scalarField X(composition.liquids().X(Y));

    scalar rho = composition.liquids().rho(this->pc(), this->T(), X);
    scalar mu = composition.liquids().mu(this->pc(), this->T(), X);
    scalar sigma = composition.liquids().sigma(this->pc(), this->T(), X);

    // Average molecular weight of carrier mix - assumes perfect gas
    scalar Wc = this->rhoc()*specie::RR*this->Tc()/this->pc();
    scalar R = specie::RR/Wc;
    scalar Tav = td.cloud().atomization().Taverage(this->T(), this->Tc());

    // calculate average gas density based on average temperature
    scalar rhoAv = this->pc()/(R*Tav);
    scalar muAv = this->muc();
    vector Urel = this->U() - this->Uc();
    scalar Urmag = mag(Urel);
    scalar Re = this->Re(this->U(), this->d(), rhoAv, muAv);

    const scalar mass = p.mass();
    const forceSuSp Fcp = forces.calcCoupled(p, dt, mass, Re, muAv);
    const forceSuSp Fncp = forces.calcNonCoupled(p, dt, mass, Re, muAv);
    this->tMom() = mass/(Fcp.Sp() + Fncp.Sp());

    const vector g = td.cloud().g().value();

    scalar massChild = 0.0;
    scalar dChild = 0.0;
    if
    (
        td.cloud().breakup().update
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
            rho,
            mu,
            sigma,
            this->U(),
            rhoAv,
            muAv,
            Urel,
            Urmag,
            this->tMom(),
            dChild,
            massChild
        )
    )
    {
        scalar Re = rhoAv*Urmag*dChild/muAv;
        this->mass0() -= massChild;

        // Add child parcel as copy of parent
        SprayParcel<ParcelType>* child = new SprayParcel<ParcelType>(*this);
        child->mass0() = massChild;
        child->d() = dChild;
        child->nParticle() = massChild/(this->rho()*this->volume(dChild));

        const forceSuSp Fcp =
            forces.calcCoupled(*child, dt, massChild, Re, muAv);
        const forceSuSp Fncp =
            forces.calcNonCoupled(*child, dt, massChild, Re, muAv);

        child->liquidCore() = 0.0;
        child->KHindex() = 1.0;
        child->y() = td.cloud().breakup().y0();
        child->yDot() = td.cloud().breakup().yDot0();
        child->tc() = -GREAT;
        child->ms() = 0.0;
        child->injector() = this->injector();
        child->tMom() = massChild/(Fcp.Sp() + Fncp.Sp());
        child->user() = 0.0;
        child->setCellValues(td, dt, cellI);

        td.cloud().addParticle(child);
    }
}


template<class ParcelType>
template<class TrackData>
CML::scalar CML::SprayParcel<ParcelType>::chi
(
    TrackData& td,
    const scalarField& X
) const
{
    // modifications to take account of the flash boiling on primary break-up

    typedef typename TrackData::cloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        td.cloud().composition();

    scalar chi = 0.0;
    scalar T0 = this->T();
    scalar p0 = this->pc();
    scalar pAmb = td.cloud().pAmbient();

    scalar pv = composition.liquids().pv(p0, T0, X);

    forAll(composition.liquids(), i)
    {
        if (pv >= 0.999*pAmb)
        {
            // liquid is boiling - calc boiling temperature

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
template<class TrackData>
void CML::SprayParcel<ParcelType>::solveTABEq
(
    TrackData& td,
    const scalar dt
)
{
    typedef typename TrackData::cloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        td.cloud().composition();

    const scalar& TABCmu = td.cloud().breakup().TABCmu();
    const scalar& TABWeCrit = td.cloud().breakup().TABWeCrit();
    const scalar& TABComega = td.cloud().breakup().TABComega();

    scalar r = 0.5*this->d();
    scalar r2 = r*r;
    scalar r3 = r*r2;

    const scalarField& Y(this->Y());
    scalarField X(composition.liquids().X(Y));

    scalar rho = composition.liquids().rho(this->pc(), this->T(), X);
    scalar mu = composition.liquids().mu(this->pc(), this->T(), X);
    scalar sigma = composition.liquids().sigma(this->pc(), this->T(), X);

    // inverse of characteristic viscous damping time
    scalar rtd = 0.5*TABCmu*mu/(rho*r2);

    // oscillation frequency (squared)
    scalar omega2 = TABComega*sigma/(rho*r3) - rtd*rtd;

    if(omega2 > 0)
    {
        scalar omega = sqrt(omega2);
        scalar rhoc = this->rhoc();
        scalar Wetmp = this->We(this->U(), r, rhoc, sigma)/TABWeCrit;

        scalar y1 = this->y() - Wetmp;
        scalar y2 = this->yDot()/omega;

        // update distortion parameters
        scalar c = cos(omega*dt);
        scalar s = sin(omega*dt);
        scalar e = exp(-rtd*dt);
        y2 = (this->yDot() + y1*rtd)/omega;

        this->y() = Wetmp + e*(y1*c + y2*s);
        if (this->y() < 0)
        {
            this->y() = 0.0;
            this->yDot() = 0.0;
        }
        else
        {
            this->yDot() = (Wetmp - this->y())*rtd + e*omega*(y2*c - y1*s);
        }
    }
    else
    {
        // reset distortion parameters
        this->y() = 0;
        this->yDot() = 0;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <class ParcelType>
CML::SprayParcel<ParcelType>::SprayParcel(const SprayParcel<ParcelType>& p)
:
    ParcelType(p),
    d0_(p.d0_),
    position0_(p.position0_),
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


template <class ParcelType>
CML::SprayParcel<ParcelType>::SprayParcel
(
    const SprayParcel<ParcelType>& p,
    const polyMesh& mesh
)
:
    ParcelType(p, mesh),
    d0_(p.d0_),
    position0_(p.position0_),
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

template <class ParcelType>
CML::string CML::SprayParcel<ParcelType>::propHeader =
    ParcelType::propHeader
  + " d0"
  + " position0"
  + " liquidCore"
  + " KHindex"
  + " y"
  + " yDot"
  + " tc"
  + " ms"
  + " injector"
  + " tMom"
  + " user";


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
    position0_(vector::zero),
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
            is.read
            (
                reinterpret_cast<char*>(&d0_),
                sizeof(d0_)
              + sizeof(position0_)
              + sizeof(liquidCore_)
              + sizeof(KHindex_)
              + sizeof(y_)
              + sizeof(yDot_)
              + sizeof(tc_)
              + sizeof(ms_)
              + sizeof(injector_)
              + sizeof(tMom_)
              + sizeof(user_)
            );
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

    IOField<scalar> liquidCore(c.fieldIOobject
    (
        "liquidCore", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, liquidCore);

    IOField<scalar> KHindex(c.fieldIOobject("KHindex", IOobject::MUST_READ));
    c.checkFieldIOobject(c, KHindex);

    IOField<scalar> y(c.fieldIOobject("y", IOobject::MUST_READ));
    c.checkFieldIOobject(c, y);

    IOField<scalar> yDot(c.fieldIOobject("yDot", IOobject::MUST_READ));
    c.checkFieldIOobject(c, yDot);

    IOField<scalar> tc(c.fieldIOobject("tc", IOobject::MUST_READ));
    c.checkFieldIOobject(c, tc);

    IOField<scalar> ms(c.fieldIOobject("ms", IOobject::MUST_READ));
    c.checkFieldIOobject(c, ms);

    IOField<scalar> injector(c.fieldIOobject("injector", IOobject::MUST_READ));
    c.checkFieldIOobject(c, injector);

    IOField<scalar> tMom(c.fieldIOobject("tMom", IOobject::MUST_READ));
    c.checkFieldIOobject(c, tMom);

    IOField<scalar> user(c.fieldIOobject("user", IOobject::MUST_READ));
    c.checkFieldIOobject(c, user);

    label i = 0;
    forAllIter(typename Cloud<SprayParcel<ParcelType> >, c, iter)
    {
        SprayParcel<ParcelType>& p = iter();
        p.d0_ = d0[i];
        p.position0_ = position0[i];
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
            sizeof(p.d0())
          + sizeof(p.position0())
          + sizeof(p.liquidCore())
          + sizeof(p.KHindex())
          + sizeof(p.y())
          + sizeof(p.yDot())
          + sizeof(p.tc())
          + sizeof(p.ms())
          + sizeof(p.injector())
          + sizeof(p.tMom())
          + sizeof(p.user())
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
