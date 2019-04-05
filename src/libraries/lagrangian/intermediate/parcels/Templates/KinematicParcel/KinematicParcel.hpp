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
    CML::KinematicParcel

Description
    Kinematic parcel class with rotational motion (as spherical
    particles only) and one/two-way coupling with the continuous
    phase.

    Sub-models include:
    - drag
    - turbulent dispersion
    - wall interactions


\*---------------------------------------------------------------------------*/

#ifndef KinematicParcel_H
#define KinematicParcel_H

#include "particle.hpp"
#include "IOstream.hpp"
#include "autoPtr.hpp"
#include "interpolation.hpp"
#include "demandDrivenEntry.hpp"
#include "mathematicalConstants.hpp"
#include "forceSuSp.hpp"
#include "integrationScheme.hpp"
#include "meshTools.hpp"
#include "cloudSolution.hpp"
#include "IOstreams.hpp"
#include "IOField.hpp"
#include "Cloud.hpp"

// #include "ParticleForceList.hpp" // TODO

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class ParcelType>
class KinematicParcel;

// Forward declaration of friend functions

template<class ParcelType>
Ostream& operator<<
(
    Ostream&,
    const KinematicParcel<ParcelType>&
);

/*---------------------------------------------------------------------------*\
                         Class KinematicParcel Declaration
\*---------------------------------------------------------------------------*/

template<class ParcelType>
class KinematicParcel
:
    public ParcelType
{
    // Private data

        //- Size in bytes of the fields
        static const std::size_t sizeofFields_;

        //- Number of particle tracking attempts before we assume that it stalls
        static label maxTrackAttempts;


public:

    //- Class to hold kinematic particle constant properties
    class constantProperties
    {
    protected:

        // Protected data

            //- Constant properties dictionary
            const dictionary dict_;


    private:

        // Private data

            //- Parcel type id - used for post-processing to flag the type
            //  of parcels issued by this cloud
            demandDrivenEntry<label> parcelTypeId_;

            //- Minimum density [kg/m3]
            demandDrivenEntry<scalar> rhoMin_;

            //- Particle density [kg/m3] (constant)
            demandDrivenEntry<scalar> rho0_;

            //- Minimum parcel mass [kg]
            demandDrivenEntry<scalar> minParcelMass_;


    public:

        // Constructors

            //- Null constructor
            constantProperties();

            //- Copy constructor
            constantProperties(const constantProperties& cp);

            //- Construct from dictionary
            constantProperties(const dictionary& parentDict);


        // Member functions

            //- Return const access to the constant properties dictionary
            inline const dictionary& dict() const;

            //- Return const access to the parcel type id
            inline label parcelTypeId() const;

            //- Return const access to the minimum density
            inline scalar rhoMin() const;

            //- Return const access to the particle density
            inline scalar rho0() const;

            //- Return const access to the minimum parcel mass
            inline scalar minParcelMass() const;
    };


    class trackingData
    :
        public ParcelType::trackingData
    {
    public:

        enum trackPart
        {
            tpVelocityHalfStep,
            tpLinearTrack,
            tpRotationalTrack
        };


    private:

        // Private data

            // Interpolators for continuous phase fields

                //- Density interpolator
                autoPtr<interpolation<scalar> > rhoInterp_;

                //- Velocity interpolator
                autoPtr<interpolation<vector> > UInterp_;

                //- Dynamic viscosity interpolator
                autoPtr<interpolation<scalar> > muInterp_;


            // Cached continuous phase properties

                //- Density [kg/m3]
                scalar rhoc_;

                //- Velocity [m/s]
                vector Uc_;

                //- Viscosity [Pa.s]
                scalar muc_;


            //- Local gravitational or other body-force acceleration
            const vector& g_;

            // label specifying which part of the integration
            // algorithm is taking place
            trackPart part_;


    public:

        // Constructors

            //- Construct from components
            template <class TrackCloudType>
            inline trackingData
            (
                const TrackCloudType& cloud,
                trackPart part = tpLinearTrack
            );


        // Member functions

            //- Return conat access to the interpolator for continuous
            //  phase density field
            inline const interpolation<scalar>& rhoInterp() const;

            //- Return conat access to the interpolator for continuous
            //  phase velocity field
            inline const interpolation<vector>& UInterp() const;

            //- Return conat access to the interpolator for continuous
            //  phase dynamic viscosity field
            inline const interpolation<scalar>& muInterp() const;

            //- Return the continuous phase density
            inline scalar rhoc() const;

            //- Access the continuous phase density
            inline scalar& rhoc();

            //- Return the continuous phase velocity
            inline const vector& Uc() const;

            //- Access the continuous phase velocity
            inline vector& Uc();

            //- Return the continuous phase viscosity
            inline scalar muc() const;

            //- Access the continuous phase viscosity
            inline scalar& muc();

            // Return const access to the gravitational acceleration vector
            inline const vector& g() const;

            //- Return the part of the tracking operation taking place
            inline trackPart part() const;

            //- Return access to the part of the tracking operation taking place
            inline trackPart& part();
    };


protected:

    // Protected data

        // Parcel properties

            //- Active flag - tracking inactive when active = false
            bool active_;

            //- Parcel type id
            label typeId_;

            //- Number of particles in Parcel
            scalar nParticle_;

            //- Diameter [m]
            scalar d_;

            //- Target diameter [m]
            scalar dTarget_;

            //- Velocity of Parcel [m/s]
            vector U_;

            //- Density [kg/m3]
            scalar rho_;

            //- Age [s]
            scalar age_;

            //- Time spent in turbulent eddy [s]
            scalar tTurb_;

            //- Turbulent velocity fluctuation [m/s]
            vector UTurb_;


    // Protected Member Functions

        //- Calculate new particle velocity
        template<class TrackCloudType>
        const vector calcVelocity
        (
            TrackCloudType& cloud,
            trackingData& td,
            const scalar dt,           // timestep
            const scalar Re,           // Reynolds number
            const scalar mu,           // local carrier viscosity
            const scalar mass,         // mass
            const vector& Su,          // explicit particle momentum source
            vector& dUTrans,           // momentum transfer to carrier
            scalar& Spu                // linearised drag coefficient
        ) const;


public:

    // Static data members

        //- Runtime type information
        TypeName("KinematicParcel");

        //- String representation of properties
        AddToPropertyList
        (
            ParcelType,
            " active"
          + " typeId"
          + " nParticle"
          + " d"
          + " dTarget "
          + " (Ux Uy Uz)"
          + " rho"
          + " age"
          + " tTurb"
          + " (UTurbx UTurby UTurbz)"
        );


    // Constructors

        //- Construct from mesh, coordinates and topology
        //  Other properties initialised as null
        inline KinematicParcel
        (
            const polyMesh& mesh,
            const barycentric& coordinates,
            const label celli,
            const label tetFacei,
            const label tetPti
        );

        //- Construct from a position and a cell, searching for the rest of the
        //  required topology. Other properties are initialised as null.
        inline KinematicParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label celli
        );

        //- Construct from components
        inline KinematicParcel
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
            const constantProperties& constProps
        );

        //- Construct from Istream
        KinematicParcel
        (
            const polyMesh& mesh,
            Istream& is,
            bool readFields = true
        );

        //- Construct as a copy
        KinematicParcel(const KinematicParcel& p);

        //- Construct as a copy
        KinematicParcel(const KinematicParcel& p, const polyMesh& mesh);

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone() const
        {
            return autoPtr<particle>(new KinematicParcel(*this));
        }

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone(const polyMesh& mesh) const
        {
            return autoPtr<particle>(new KinematicParcel(*this, mesh));
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

            autoPtr<KinematicParcel<ParcelType> > operator()(Istream& is) const
            {
                return autoPtr<KinematicParcel<ParcelType> >
                (
                    new KinematicParcel<ParcelType>(mesh_, is, true)
                );
            }
        };


    // Member Functions

        // Access

            //- Return const access to active flag
            inline bool active() const;

            //- Return const access to type id
            inline label typeId() const;

            //- Return const access to number of particles
            inline scalar nParticle() const;

            //- Return const access to diameter
            inline scalar d() const;

            //- Return const access to target diameter
            inline scalar dTarget() const;

            //- Return const access to velocity
            inline const vector& U() const;

            //- Return const access to density
            inline scalar rho() const;

            //- Return const access to the age
            inline scalar age() const;

            //- Return const access to time spent in turbulent eddy
            inline scalar tTurb() const;

            //- Return const access to turbulent velocity fluctuation
            inline const vector& UTurb() const;


        // Edit

            //- Return const access to active flag
            inline bool& active();

            //- Return access to type id
            inline label& typeId();

            //- Return access to number of particles
            inline scalar& nParticle();

            //- Return access to diameter
            inline scalar& d();

            //- Return access to target diameter
            inline scalar& dTarget();

            //- Return access to velocity
            inline vector& U();

            //- Return access to density
            inline scalar& rho();

            //- Return access to the age
            inline scalar& age();

            //- Return access to time spent in turbulent eddy
            inline scalar& tTurb();

            //- Return access to turbulent velocity fluctuation
            inline vector& UTurb();


        // Helper functions

            //- Cell owner mass
            inline scalar massCell(const trackingData& td) const;

            //- Particle mass
            inline scalar mass() const;

            //- Particle moment of inertia around diameter axis
            inline scalar momentOfInertia() const;

            //- Particle volume
            inline scalar volume() const;

            //- Particle volume for a given diameter
            inline static scalar volume(const scalar d);

            //- Particle projected area
            inline scalar areaP() const;

            //- Projected area for given diameter
            inline static scalar areaP(const scalar d);

            //- Particle surface area
            inline scalar areaS() const;

            //- Surface area for given diameter
            inline static scalar areaS(const scalar d);

            //- Reynolds number
            inline scalar Re(const trackingData& td) const;

            //- Reynolds number for given conditions
            inline static scalar Re
            (
                const scalar rhoc,
                const vector& U,
                const vector& Uc,
                const scalar d,
                const scalar muc
            );

            //- Weber number
            inline scalar We
            (
                const trackingData& td,
                const scalar sigma
            ) const;

            //- Weber number for given conditions
            inline static scalar We
            (
                const scalar rhoc,
                const vector& U,
                const vector& Uc,
                const scalar d,
                const scalar sigma
            );

            //- Eotvos number
            inline scalar Eo
            (
                const trackingData& td,
                const scalar sigma
            ) const;

            //- Eotvos number for given conditions
            inline static scalar Eo
            (
                const vector& g,
                const scalar rho,
                const scalar rhoc,
                const vector& U,
                const scalar d,
                const scalar sigma
            );


        // Main calculation loop

            //- Set cell values
            template<class TrackCloudType>
            void setCellValues(TrackCloudType& cloud, trackingData& td);

            //- Apply dispersion to the carrier phase velocity and update
            //  parcel turbulence parameters
            template<class TrackCloudType>
            void calcDispersion
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

            //- Update parcel properties over the time interval
            template<class TrackCloudType>
            void calc
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar dt
            );


        // Tracking

            //- Move the parcel
            template<class TrackCloudType>
            bool move
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar trackTime
            );


        // Patch interactions

            //- Overridable function to handle the particle hitting a patch
            //  Executed before other patch-hitting functions
            template<class TrackCloudType>
            bool hitPatch(TrackCloudType& cloud, trackingData& td);

            //- Overridable function to handle the particle hitting a
            //  processorPatch
            template<class TrackCloudType>
            void hitProcessorPatch(TrackCloudType& cloud, trackingData& td);

            //- Overridable function to handle the particle hitting a wallPatch
            template<class TrackCloudType>
            void hitWallPatch(TrackCloudType& cloud, trackingData& td);

            //- Transform the physical properties of the particle
            //  according to the given transformation tensor
            virtual void transformProperties(const tensor& T);

            //- Transform the physical properties of the particle
            //  according to the given separation vector
            virtual void transformProperties(const vector& separation);


        // I-O

            //- Read
            template<class TrackCloudType>
            static void readFields(TrackCloudType& c);

            //- Write
            template<class TrackCloudType>
            static void writeFields(const TrackCloudType& c);


    // Ostream Operator

        friend Ostream& operator<< <ParcelType>
        (
            Ostream&,
            const KinematicParcel<ParcelType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

using namespace CML::constant::mathematical;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
inline
CML::KinematicParcel<ParcelType>::constantProperties::constantProperties()
:
    dict_(dictionary::null),
    parcelTypeId_(dict_, -1),
    rhoMin_(dict_, 0.0),
    rho0_(dict_, 0.0),
    minParcelMass_(dict_, 0.0)
{}


template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
    const constantProperties& cp
)
:
    dict_(cp.dict_),
    parcelTypeId_(cp.parcelTypeId_),
    rhoMin_(cp.rhoMin_),
    rho0_(cp.rho0_),
    minParcelMass_(cp.minParcelMass_)
{}


template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
    const dictionary& parentDict
)
:
    dict_(parentDict.subOrEmptyDict("constantProperties")),
    parcelTypeId_(dict_, "parcelTypeId", -1),
    rhoMin_(dict_, "rhoMin", 1e-15),
    rho0_(dict_, "rho0"),
    minParcelMass_(dict_, "minParcelMass", 1e-15)
{}


template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& owner,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti
)
:
    ParcelType(owner, coordinates, celli, tetFacei, tetPti),
    active_(true),
    typeId_(-1),
    nParticle_(0),
    d_(0.0),
    dTarget_(0.0),
    U_(Zero),
    rho_(0.0),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(Zero)
{}


template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& owner,
    const vector& position,
    const label celli
)
:
    ParcelType(owner, position, celli),
    active_(true),
    typeId_(-1),
    nParticle_(0),
    d_(0.0),
    dTarget_(0.0),
    U_(Zero),
    rho_(0.0),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(Zero)
{}


template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& owner,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti,
    const label typeId,
    const scalar nParticle0,
    const scalar d0,
    const scalar dTarget0,
    const vector& U0,
    const constantProperties& constProps
)
:
    ParcelType(owner, coordinates, celli, tetFacei, tetPti),
    active_(true),
    typeId_(typeId),
    nParticle_(nParticle0),
    d_(d0),
    dTarget_(dTarget0),
    U_(U0),
    rho_(constProps.rho0()),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(Zero)
{}


// * * * * * * * * * constantProperties Member Functions * * * * * * * * * * //

template<class ParcelType>
inline const CML::dictionary&
CML::KinematicParcel<ParcelType>::constantProperties::dict() const
{
    return dict_;
}


template<class ParcelType>
inline CML::label
CML::KinematicParcel<ParcelType>::constantProperties::parcelTypeId() const
{
    return parcelTypeId_.value();
}


template<class ParcelType>
inline CML::scalar
CML::KinematicParcel<ParcelType>::constantProperties::rhoMin() const
{
    return rhoMin_.value();
}


template<class ParcelType>
inline CML::scalar
CML::KinematicParcel<ParcelType>::constantProperties::rho0() const
{
    return rho0_.value();
}


template<class ParcelType>
inline CML::scalar
CML::KinematicParcel<ParcelType>::constantProperties::minParcelMass() const
{
    return minParcelMass_.value();
}


// * * * * * * * KinematicParcel Member Functions  * * * * * * * //

template<class ParcelType>
inline bool CML::KinematicParcel<ParcelType>::active() const
{
    return active_;
}


template<class ParcelType>
inline CML::label CML::KinematicParcel<ParcelType>::typeId() const
{
    return typeId_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::nParticle() const
{
    return nParticle_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::d() const
{
    return d_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::dTarget() const
{
    return dTarget_;
}


template<class ParcelType>
inline const CML::vector& CML::KinematicParcel<ParcelType>::U() const
{
    return U_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::rho() const
{
    return rho_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::age() const
{
    return age_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::tTurb() const
{
    return tTurb_;
}


template<class ParcelType>
inline const CML::vector& CML::KinematicParcel<ParcelType>::UTurb() const
{
    return UTurb_;
}


template<class ParcelType>
inline bool& CML::KinematicParcel<ParcelType>::active()
{
    return active_;
}


template<class ParcelType>
inline CML::label& CML::KinematicParcel<ParcelType>::typeId()
{
    return typeId_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::nParticle()
{
    return nParticle_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::d()
{
    return d_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::dTarget()
{
    return dTarget_;
}


template<class ParcelType>
inline CML::vector& CML::KinematicParcel<ParcelType>::U()
{
    return U_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::rho()
{
    return rho_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::age()
{
    return age_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::tTurb()
{
    return tTurb_;
}


template<class ParcelType>
inline CML::vector& CML::KinematicParcel<ParcelType>::UTurb()
{
    return UTurb_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::massCell
(
    const trackingData& td
) const
{
    return td.rhoc()*this->mesh().cellVolumes()[this->cell()];
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::mass() const
{
    return rho_*volume();
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::momentOfInertia() const
{
    return 0.1*mass()*sqr(d_);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::volume() const
{
    return volume(d_);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::volume(const scalar d)
{
    return pi/6.0*pow3(d);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::areaP() const
{
    return areaP(d_);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::areaP(const scalar d)
{
    return 0.25*areaS(d);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::areaS() const
{
    return areaS(d_);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::areaS(const scalar d)
{
    return pi*d*d;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::Re
(
    const trackingData& td
) const
{
    return Re(td.rhoc(), U_, td.Uc(), d_, td.muc());
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::Re
(
    const scalar rhoc,
    const vector& U,
    const vector& Uc,
    const scalar d,
    const scalar muc
)
{
    return rhoc*mag(U - Uc)*d/max(muc, ROOTVSMALL);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::We
(
    const trackingData& td,
    const scalar sigma
) const
{
    return We(td.rhoc(), U_, td.Uc(), d_, sigma);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::We
(
    const scalar rhoc,
    const vector& U,
    const vector& Uc,
    const scalar d,
    const scalar sigma
)
{
    return rhoc*magSqr(U - Uc)*d/max(sigma, ROOTVSMALL);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::Eo
(
    const trackingData& td,
    const scalar sigma
) const
{
    return Eo(td.g(), rho_, td.rhoc(), U_, d_, sigma);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::Eo
(
    const vector& g,
    const scalar rho,
    const scalar rhoc,
    const vector& U,
    const scalar d,
    const scalar sigma
)
{
    const vector dir = U/max(mag(U), ROOTVSMALL);
    return mag(g & dir)*(rho - rhoc)*sqr(d)/max(sigma, ROOTVSMALL);
}


// ************************************************************************* //
template<class ParcelType>
template<class TrackCloudType>
inline CML::KinematicParcel<ParcelType>::trackingData::trackingData
(
    const TrackCloudType& cloud,
    trackPart part
)
:
    ParcelType::trackingData(cloud),
    rhoInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.rho()
        )
    ),
    UInterp_
    (
        interpolation<vector>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.U()
        )
    ),
    muInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.mu()
        )
    ),
    rhoc_(Zero),
    Uc_(Zero),
    muc_(Zero),
    g_(cloud.g().value()),
    part_(part)
{}


template<class ParcelType>
inline const CML::interpolation<CML::scalar>&
CML::KinematicParcel<ParcelType>::trackingData::rhoInterp() const
{
    return rhoInterp_();
}


template<class ParcelType>
inline const CML::interpolation<CML::vector>&
CML::KinematicParcel<ParcelType>::trackingData::UInterp() const
{
    return UInterp_();
}


template<class ParcelType>
inline const CML::interpolation<CML::scalar>&
CML::KinematicParcel<ParcelType>::trackingData::muInterp() const
{
    return muInterp_();
}


template<class ParcelType>
inline const CML::vector&
CML::KinematicParcel<ParcelType>::trackingData::g() const
{
    return g_;
}


template<class ParcelType>
inline CML::scalar
CML::KinematicParcel<ParcelType>::trackingData::rhoc() const
{
    return rhoc_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::trackingData::rhoc()
{
    return rhoc_;
}


template<class ParcelType>
inline const CML::vector&
CML::KinematicParcel<ParcelType>::trackingData::Uc() const
{
    return Uc_;
}


template<class ParcelType>
inline CML::vector& CML::KinematicParcel<ParcelType>::trackingData::Uc()
{
    return Uc_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::trackingData::muc() const
{
    return muc_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::trackingData::muc()
{
    return muc_;
}


template<class ParcelType>
inline typename CML::KinematicParcel<ParcelType>::trackingData::trackPart
CML::KinematicParcel<ParcelType>::trackingData::part() const
{
    return part_;
}


template<class ParcelType>
inline typename CML::KinematicParcel<ParcelType>::trackingData::trackPart&
CML::KinematicParcel<ParcelType>::trackingData::part()
{
    return part_;
}


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ParcelType>
CML::label CML::KinematicParcel<ParcelType>::maxTrackAttempts = 1;


// * * * * * * * * * * *  Protected Member Functions * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackCloudType>
void CML::KinematicParcel<ParcelType>::setCellValues
(
    TrackCloudType& cloud,
    trackingData& td
)
{
    tetIndices tetIs = this->currentTetIndices();

    td.rhoc() = td.rhoInterp().interpolate(this->coordinates(), tetIs);

    if (td.rhoc() < cloud.constProps().rhoMin())
    {
        if (debug)
        {
            WarningInFunction
                << "Limiting observed density in cell " << this->cell()
                << " to " << cloud.constProps().rhoMin() <<  nl << endl;
        }

        td.rhoc() = cloud.constProps().rhoMin();
    }

    td.Uc() = td.UInterp().interpolate(this->coordinates(), tetIs);

    td.muc() = td.muInterp().interpolate(this->coordinates(), tetIs);
}


template<class ParcelType>
template<class TrackCloudType>
void CML::KinematicParcel<ParcelType>::calcDispersion
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    td.Uc() = cloud.dispersion().update
    (
        dt,
        this->cell(),
        U_,
        td.Uc(),
        UTurb_,
        tTurb_
    );
}


template<class ParcelType>
template<class TrackCloudType>
void CML::KinematicParcel<ParcelType>::cellValueSourceCorrection
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    td.Uc() += cloud.UTrans()[this->cell()]/massCell(td);
}


template<class ParcelType>
template<class TrackCloudType>
void CML::KinematicParcel<ParcelType>::calc
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    // Define local properties at beginning of time step
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    const scalar np0 = nParticle_;
    const scalar mass0 = mass();

    // Reynolds number
    const scalar Re = this->Re(td);


    // Sources
    //~~~~~~~~

    // Explicit momentum source for particle
    vector Su = Zero;

    // Linearised momentum source coefficient
    scalar Spu = 0.0;

    // Momentum transfer from the particle to the carrier phase
    vector dUTrans = Zero;


    // Motion
    // ~~~~~~

    // Calculate new particle velocity
    this->U_ =
        calcVelocity(cloud, td, dt, Re, td.muc(), mass0, Su, dUTrans, Spu);


    // Accumulate carrier phase source terms
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (cloud.solution().coupled())
    {
        // Update momentum transfer
        cloud.UTrans()[this->cell()] += np0*dUTrans;

        // Update momentum transfer coefficient
        cloud.UCoeff()[this->cell()] += np0*Spu;
    }
}


template<class ParcelType>
template<class TrackCloudType>
const CML::vector CML::KinematicParcel<ParcelType>::calcVelocity
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt,
    const scalar Re,
    const scalar mu,
    const scalar mass,
    const vector& Su,
    vector& dUTrans,
    scalar& Spu
) const
{
    const typename TrackCloudType::parcelType& p =
        static_cast<const typename TrackCloudType::parcelType&>(*this);
    typename TrackCloudType::parcelType::trackingData& ttd =
        static_cast<typename TrackCloudType::parcelType::trackingData&>(td);

    const typename TrackCloudType::forceType& forces = cloud.forces();

    // Momentum source due to particle forces
    const forceSuSp Fcp = forces.calcCoupled(p, ttd, dt, mass, Re, mu);
    const forceSuSp Fncp = forces.calcNonCoupled(p, ttd, dt, mass, Re, mu);
    const scalar massEff = forces.massEff(p, ttd, mass);

    /*
    // Proper splitting ...
    // Calculate the integration coefficients
    const vector acp = (Fcp.Sp()*td.Uc() + Fcp.Su())/massEff;
    const vector ancp = (Fncp.Sp()*td.Uc() + Fncp.Su() + Su)/massEff;
    const scalar bcp = Fcp.Sp()/massEff;
    const scalar bncp = Fncp.Sp()/massEff;

    // Integrate to find the new parcel velocity
    const vector deltaUcp =
        cloud.UIntegrator().partialDelta
        (
            U_, dt, acp + ancp, bcp + bncp, acp, bcp
        );
    const vector deltaUncp =
        cloud.UIntegrator().partialDelta
        (
            U_, dt, acp + ancp, bcp + bncp, ancp, bncp
        );
    const vector deltaT = deltaUcp + deltaUncp;
    */

    // Shortcut splitting assuming no implicit non-coupled force ...
    // Calculate the integration coefficients
    const vector acp = (Fcp.Sp()*td.Uc() + Fcp.Su())/massEff;
    const vector ancp = (Fncp.Su() + Su)/massEff;
    const scalar bcp = Fcp.Sp()/massEff;

    // Integrate to find the new parcel velocity
    const vector deltaU = cloud.UIntegrator().delta(U_, dt, acp + ancp, bcp);
    const vector deltaUncp = ancp*dt;
    const vector deltaUcp = deltaU - deltaUncp;

    // Calculate the new velocity and the momentum transfer terms
    vector Unew = U_ + deltaU;

    dUTrans -= massEff*deltaUcp;

    Spu = dt*Fcp.Sp();

    // Apply correction to velocity and dUTrans for reduced-D cases
    const polyMesh& mesh = cloud.pMesh();
    meshTools::constrainDirection(mesh, mesh.solutionD(), Unew);
    meshTools::constrainDirection(mesh, mesh.solutionD(), dUTrans);

    return Unew;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const KinematicParcel<ParcelType>& p
)
:
    ParcelType(p),
    active_(p.active_),
    typeId_(p.typeId_),
    nParticle_(p.nParticle_),
    d_(p.d_),
    dTarget_(p.dTarget_),
    U_(p.U_),
    rho_(p.rho_),
    age_(p.age_),
    tTurb_(p.tTurb_),
    UTurb_(p.UTurb_)
{}


template<class ParcelType>
CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const KinematicParcel<ParcelType>& p,
    const polyMesh& mesh
)
:
    ParcelType(p, mesh),
    active_(p.active_),
    typeId_(p.typeId_),
    nParticle_(p.nParticle_),
    d_(p.d_),
    dTarget_(p.dTarget_),
    U_(p.U_),
    rho_(p.rho_),
    age_(p.age_),
    tTurb_(p.tTurb_),
    UTurb_(p.UTurb_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackCloudType>
bool CML::KinematicParcel<ParcelType>::move
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar trackTime
)
{
    typename TrackCloudType::parcelType& p =
        static_cast<typename TrackCloudType::parcelType&>(*this);
    typename TrackCloudType::parcelType::trackingData& ttd =
        static_cast<typename TrackCloudType::parcelType::trackingData&>(td);

    ttd.switchProcessor = false;
    ttd.keepParticle = true;

    const scalarField& cellLengthScale = cloud.cellLengthScale();
    const scalar maxCo = cloud.solution().maxCo();

    while (ttd.keepParticle && !ttd.switchProcessor && p.stepFraction() < 1)
    {
        // Cache the current position, cell and step-fraction
        const point start = p.position();
        const scalar sfrac = p.stepFraction();

        // Total displacement over the time-step
        const vector s = trackTime*U_;

        // Cell length scale
        const scalar l = cellLengthScale[p.cell()];

        // Deviation from the mesh centre for reduced-D cases
        const vector d = p.deviationFromMeshCentre();

        // Fraction of the displacement to track in this loop. This is limited
        // to ensure that the both the time and distance tracked is less than
        // maxCo times the total value.
        scalar f = 1 - p.stepFraction();
        f = min(f, maxCo);
        f = min(f, maxCo*l/max(SMALL*l, mag(s)));
        if (p.active())
        {
            // Track to the next face
            p.trackToFace(f*s - d, f);
        }
        else
        {
            // At present the only thing that sets active_ to false is a stick
            // wall interaction. We want the position of the particle to remain
            // the same relative to the face that it is on. The local
            // coordinates therefore do not change. We still advance in time and
            // perform the relevant interactions with the fixed particle.
            p.stepFraction() += f;
        }

        const scalar dt = (p.stepFraction() - sfrac)*trackTime;

        // Avoid problems with extremely small timesteps
        if (dt > ROOTVSMALL)
        {
            // Update cell based properties
            p.setCellValues(cloud, ttd);

            p.calcDispersion(cloud, ttd, dt);

            if (cloud.solution().cellValueSourceCorrection())
            {
                p.cellValueSourceCorrection(cloud, ttd, dt);
            }

            p.calc(cloud, ttd, dt);
        }

        p.age() += dt;

        if (p.active() && p.onFace())
        {
            cloud.functions().postFace(p, ttd.keepParticle);
        }

        cloud.functions().postMove(p, dt, start, ttd.keepParticle);

        if (p.active() && p.onFace() && ttd.keepParticle)
        {
            p.hitFace(s, cloud, ttd);
        }
    }

    return ttd.keepParticle;
}


template<class ParcelType>
template<class TrackCloudType>
bool CML::KinematicParcel<ParcelType>::hitPatch
(
    TrackCloudType& cloud,
    trackingData& td
)
{
    typename TrackCloudType::parcelType& p =
        static_cast<typename TrackCloudType::parcelType&>(*this);

    const polyPatch& pp = p.mesh().boundaryMesh()[p.patch()];

    // Invoke post-processing model
    cloud.functions().postPatch(p, pp, td.keepParticle);

    // Invoke surface film model
    if (cloud.surfaceFilm().transferParcel(p, pp, td.keepParticle))
    {
        // All interactions done
        return true;
    }
    else if (pp.coupled())
    {
        // Don't apply the patchInteraction models to coupled boundaries
        return false;
    }
    else
    {
        // Invoke patch interaction model
        return cloud.patchInteraction().correct(p, pp, td.keepParticle);
    }
}


template<class ParcelType>
template<class TrackCloudType>
void CML::KinematicParcel<ParcelType>::hitProcessorPatch
(
    TrackCloudType&,
    trackingData& td
)
{
    td.switchProcessor = true;
}


template<class ParcelType>
template<class TrackCloudType>
void CML::KinematicParcel<ParcelType>::hitWallPatch
(
    TrackCloudType&,
    trackingData&
)
{
    // wall interactions are handled by the generic hitPatch method
}


template<class ParcelType>
void CML::KinematicParcel<ParcelType>::transformProperties(const tensor& T)
{
    ParcelType::transformProperties(T);

    U_ = transform(T, U_);
}


template<class ParcelType>
void CML::KinematicParcel<ParcelType>::transformProperties
(
    const vector& separation
)
{
    ParcelType::transformProperties(separation);
}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ParcelType>
CML::string CML::KinematicParcel<ParcelType>::propertyList_ =
    CML::KinematicParcel<ParcelType>::propertyList();

template<class ParcelType>
const std::size_t CML::KinematicParcel<ParcelType>::sizeofFields_
(
    sizeof(KinematicParcel<ParcelType>)
  - offsetof(KinematicParcel<ParcelType>, active_)
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& mesh,
    Istream& is,
    bool readFields
)
:
    ParcelType(mesh, is, readFields),
    active_(false),
    typeId_(0),
    nParticle_(0.0),
    d_(0.0),
    dTarget_(0.0),
    U_(Zero),
    rho_(0.0),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(Zero)
{
    if (readFields)
    {
        if (is.format() == IOstream::ASCII)
        {
            active_ = readBool(is);
            typeId_ = readLabel(is);
            nParticle_ = readScalar(is);
            d_ = readScalar(is);
            dTarget_ = readScalar(is);
            is >> U_;
            rho_ = readScalar(is);
            age_ = readScalar(is);
            tTurb_ = readScalar(is);
            is >> UTurb_;
        }
        else
        {
            is.read(reinterpret_cast<char*>(&active_), sizeofFields_);
        }
    }

    // Check state of Istream
    is.check(FUNCTION_NAME);
}


template<class ParcelType>
template<class CloudType>
void CML::KinematicParcel<ParcelType>::readFields(CloudType& c)
{
    if (!c.size())
    {
        return;
    }

    ParcelType::readFields(c);

    IOField<label> active
    (
        c.fieldIOobject("active", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, active);

    IOField<label> typeId
    (
        c.fieldIOobject("typeId", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, typeId);

    IOField<scalar> nParticle
    (
        c.fieldIOobject("nParticle", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, nParticle);

    IOField<scalar> d
    (
        c.fieldIOobject("d", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, d);

    IOField<scalar> dTarget
    (
        c.fieldIOobject("dTarget", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, dTarget);

    IOField<vector> U
    (
        c.fieldIOobject("U", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, U);

    IOField<scalar> rho
    (
        c.fieldIOobject("rho", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, rho);

    IOField<scalar> age
    (
        c.fieldIOobject("age", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, age);

    IOField<scalar> tTurb
    (
        c.fieldIOobject("tTurb", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, tTurb);

    IOField<vector> UTurb
    (
        c.fieldIOobject("UTurb", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, UTurb);

    label i = 0;

    forAllIter(typename CloudType, c, iter)
    {
        KinematicParcel<ParcelType>& p = iter();

        p.active_ = active[i];
        p.typeId_ = typeId[i];
        p.nParticle_ = nParticle[i];
        p.d_ = d[i];
        p.dTarget_ = dTarget[i];
        p.U_ = U[i];
        p.rho_ = rho[i];
        p.age_ = age[i];
        p.tTurb_ = tTurb[i];
        p.UTurb_ = UTurb[i];

        i++;
    }
}


template<class ParcelType>
template<class CloudType>
void CML::KinematicParcel<ParcelType>::writeFields(const CloudType& c)
{
    ParcelType::writeFields(c);

    label np = c.size();

    IOField<label> active(c.fieldIOobject("active", IOobject::NO_READ), np);
    IOField<label> typeId(c.fieldIOobject("typeId", IOobject::NO_READ), np);
    IOField<scalar> nParticle
    (
        c.fieldIOobject("nParticle", IOobject::NO_READ),
        np
    );
    IOField<scalar> d(c.fieldIOobject("d", IOobject::NO_READ), np);
    IOField<scalar> dTarget(c.fieldIOobject("dTarget", IOobject::NO_READ), np);
    IOField<vector> U(c.fieldIOobject("U", IOobject::NO_READ), np);
    IOField<scalar> rho(c.fieldIOobject("rho", IOobject::NO_READ), np);
    IOField<scalar> age(c.fieldIOobject("age", IOobject::NO_READ), np);
    IOField<scalar> tTurb(c.fieldIOobject("tTurb", IOobject::NO_READ), np);
    IOField<vector> UTurb(c.fieldIOobject("UTurb", IOobject::NO_READ), np);

    label i = 0;

    forAllConstIter(typename CloudType, c, iter)
    {
        const KinematicParcel<ParcelType>& p = iter();

        active[i] = p.active();
        typeId[i] = p.typeId();
        nParticle[i] = p.nParticle();
        d[i] = p.d();
        dTarget[i] = p.dTarget();
        U[i] = p.U();
        rho[i] = p.rho();
        age[i] = p.age();
        tTurb[i] = p.tTurb();
        UTurb[i] = p.UTurb();

        i++;
    }

    active.write();
    typeId.write();
    nParticle.write();
    d.write();
    dTarget.write();
    U.write();
    rho.write();
    age.write();
    tTurb.write();
    UTurb.write();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class ParcelType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const KinematicParcel<ParcelType>& p
)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const ParcelType&>(p)
            << token::SPACE << p.active()
            << token::SPACE << p.typeId()
            << token::SPACE << p.nParticle()
            << token::SPACE << p.d()
            << token::SPACE << p.dTarget()
            << token::SPACE << p.U()
            << token::SPACE << p.rho()
            << token::SPACE << p.age()
            << token::SPACE << p.tTurb()
            << token::SPACE << p.UTurb();
    }
    else
    {
        os  << static_cast<const ParcelType&>(p);
        os.write
        (
            reinterpret_cast<const char*>(&p.active_),
            KinematicParcel<ParcelType>::sizeofFields_
        );
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const KinematicParcel<ParcelType>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
