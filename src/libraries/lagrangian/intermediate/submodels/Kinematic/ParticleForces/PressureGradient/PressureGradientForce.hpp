/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2017 OpenFOAM Foundation
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
    CML::PressureGradientForce

Description
    Calculates particle pressure gradient force


\*---------------------------------------------------------------------------*/

#ifndef PressureGradientForce_H
#define PressureGradientForce_H

#include "ParticleForce.hpp"
#include "volFields.hpp"
#include "interpolation.hpp"
#include "fvcDdt.hpp"
#include "fvcGrad.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class PressureGradientForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PressureGradientForce
:
    public ParticleForce<CloudType>
{
protected:

    // Protected data

        //- Name of velocity field
        const word UName_;

        //- Rate of change of carrier phase velocity interpolator
        autoPtr<interpolation<vector> > DUcDtInterpPtr_;


public:

    //- Runtime type information
    TypeName("pressureGradient");


    // Constructors

        //- Construct from mesh
        PressureGradientForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& forceType = typeName
        );

        //- Construct copy
        PressureGradientForce(const PressureGradientForce& pgf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new PressureGradientForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~PressureGradientForce();


    // Member Functions

        // Access

            //- Return the rate of change of carrier phase velocity interpolator
            inline const interpolation<vector>& DUcDtInterp() const;


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

            //- Return the added mass
            virtual scalar massAdd
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar mass
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
inline const CML::interpolation<CML::vector>&
CML::PressureGradientForce<CloudType>::DUcDtInterp() const
{
    if (!DUcDtInterpPtr_.valid())
    {
        FatalErrorInFunction
            << "Carrier phase DUcDt interpolation object not set"
            << abort(FatalError);
    }

    return DUcDtInterpPtr_();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PressureGradientForce<CloudType>::PressureGradientForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& forceType
)
:
    ParticleForce<CloudType>(owner, mesh, dict, forceType, true),
    UName_(this->coeffs().template lookupOrDefault<word>("U", "U")),
    DUcDtInterpPtr_(nullptr)
{}


template<class CloudType>
CML::PressureGradientForce<CloudType>::PressureGradientForce
(
    const PressureGradientForce& pgf
)
:
    ParticleForce<CloudType>(pgf),
    UName_(pgf.UName_),
    DUcDtInterpPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PressureGradientForce<CloudType>::~PressureGradientForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::PressureGradientForce<CloudType>::cacheFields(const bool store)
{
    static word fName("DUcDt");

    bool fieldExists = this->mesh().template foundObject<volVectorField>(fName);

    if (store)
    {
        if (!fieldExists)
        {
            const volVectorField& Uc = this->mesh().template
                lookupObject<volVectorField>(UName_);

            volVectorField* DUcDtPtr = new volVectorField
            (
                fName,
                fvc::ddt(Uc) + (Uc & fvc::grad(Uc))
            );

            DUcDtPtr->store();
        }

        const volVectorField& DUcDt = this->mesh().template
            lookupObject<volVectorField>(fName);

        DUcDtInterpPtr_.reset
        (
            interpolation<vector>::New
            (
                this->owner().solution().interpolationSchemes(),
                DUcDt
            ).ptr()
        );
    }
    else
    {
        DUcDtInterpPtr_.clear();

        if (fieldExists)
        {
            const volVectorField& DUcDt = this->mesh().template
                lookupObject<volVectorField>(fName);

            const_cast<volVectorField&>(DUcDt).checkOut();
        }
    }
}


template<class CloudType>
CML::forceSuSp CML::PressureGradientForce<CloudType>::calcCoupled
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

    vector DUcDt =
        DUcDtInterp().interpolate(p.coordinates(), p.currentTetIndices());

    value.Su() = mass*td.rhoc()/p.rho()*DUcDt;

    return value;
}


template<class CloudType>
CML::scalar CML::PressureGradientForce<CloudType>::massAdd
(
    const typename CloudType::parcelType&,
    const typename CloudType::parcelType::trackingData& td,
    const scalar
) const
{
    return 0.0;
}

#endif

// ************************************************************************* //
