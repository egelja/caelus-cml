/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2017 OpenFOAM Foundation
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
    CML::LiftForce

Description
    Base class for particle lift force models


\*---------------------------------------------------------------------------*/

#ifndef LiftForce_H
#define LiftForce_H

#include "ParticleForce.hpp"
#include "volFields.hpp"
#include "interpolation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class LiftForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class LiftForce
:
    public ParticleForce<CloudType>
{
protected:

    // Protected data

        //- Name of velocity field
        const word UName_;

        //- Curk of carrier phase velocity interpolator
        autoPtr<interpolation<vector> > curlUcInterpPtr_;


    // Potected Member Functions

        //- Calculate the lift coefficient
        virtual scalar Cl
        (
            const typename CloudType::parcelType& p,
            const typename CloudType::parcelType::trackingData& td,
            const vector& curlUc,
            const scalar Re,
            const scalar muc
        ) const;


public:

    // Constructors

        //- Construct from mesh
        LiftForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& forceType
        );

        //- Construct copy
        LiftForce(const LiftForce& lf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new LiftForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~LiftForce();


    // Member Functions

        // Access

            //- Return the curl of the carrier phase velocity interpolator
            inline const interpolation<vector>& curlUcInterp() const;


        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);

            //- Calculate the non-coupled force
            virtual forceSuSp calcCoupled
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar dt,
                const scalar mass,
                const scalar Re,
                const scalar muc
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
inline const CML::interpolation<CML::vector>&
CML::LiftForce<CloudType>::curlUcInterp() const
{
    if (!curlUcInterpPtr_.valid())
    {
        FatalErrorInFunction
            << "Carrier phase curlUc interpolation object not set"
            << abort(FatalError);
    }

    return curlUcInterpPtr_();
}


#include "fvcCurl.hpp"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::LiftForce<CloudType>::LiftForce::Cl
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const vector& curlUc,
    const scalar Re,
    const scalar muc
) const
{
    // dummy
    return 0.0;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::LiftForce<CloudType>::LiftForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& forceType
)
:
    ParticleForce<CloudType>(owner, mesh, dict, forceType, true),
    UName_(this->coeffs().template lookupOrDefault<word>("U", "U")),
    curlUcInterpPtr_(nullptr)
{}


template<class CloudType>
CML::LiftForce<CloudType>::LiftForce(const LiftForce& lf)
:
    ParticleForce<CloudType>(lf),
    UName_(lf.UName_),
    curlUcInterpPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::LiftForce<CloudType>::~LiftForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::LiftForce<CloudType>::cacheFields(const bool store)
{
    static word fName("curlUcDt");

    bool fieldExists = this->mesh().template foundObject<volVectorField>(fName);

    if (store)
    {
        if (!fieldExists)
        {
            const volVectorField& Uc = this->mesh().template
                lookupObject<volVectorField>(UName_);

            volVectorField* curlUcPtr =
                new volVectorField(fName, fvc::curl(Uc));

            curlUcPtr->store();
        }

        const volVectorField& curlUc = this->mesh().template
            lookupObject<volVectorField>(fName);

        curlUcInterpPtr_.reset
        (
            interpolation<vector>::New
            (
                this->owner().solution().interpolationSchemes(),
                curlUc
            ).ptr()
        );
    }
    else
    {
        curlUcInterpPtr_.clear();

        if (fieldExists)
        {
            const volVectorField& curlUc = this->mesh().template
                lookupObject<volVectorField>(fName);

            const_cast<volVectorField&>(curlUc).checkOut();
        }
    }
}


template<class CloudType>
CML::forceSuSp CML::LiftForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value(Zero, 0.0);

    vector curlUc =
        curlUcInterp().interpolate(p.coordinates(), p.currentTetIndices());

    scalar Cl = this->Cl(p, td, curlUc, Re, muc);

    value.Su() = mass/p.rho()*td.rhoc()*Cl*((td.Uc() - p.U())^curlUc);

    return value;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
