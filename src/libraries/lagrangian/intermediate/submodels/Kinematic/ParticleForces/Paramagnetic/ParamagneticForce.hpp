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
    CML::ParamagneticForce

Description
    Calculates particle paramagnetic (magnetic field) force


\*---------------------------------------------------------------------------*/

#ifndef ParamagneticForce_H
#define ParamagneticForce_H

#include "ParticleForce.hpp"
#include "interpolation.hpp"
#include "demandDrivenData.hpp"
#include "electromagneticConstants.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;

/*---------------------------------------------------------------------------*\
                       Class ParamagneticForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ParamagneticForce
:
    public ParticleForce<CloudType>
{
    // Private data

        //- Name of paramagnetic field strength field - default = "HdotGradH"
        const word HdotGradHName_;

        //- HdotGradH interpolator
        const interpolation<vector>* HdotGradHInterpPtr_;

        //- Magnetic susceptibility of particle
        const scalar magneticSusceptibility_;


public:

    //- Runtime type information
    TypeName("paramagnetic");


    // Constructors

        //- Construct from mesh
        ParamagneticForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        ParamagneticForce(const ParamagneticForce& gf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new ParamagneticForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ParamagneticForce();


    // Member Functions

        // Access

            //- Return the name of paramagnetic field strength field
            const word& HdotGradHName() const;

            //- Return the magnetic susceptibility of particle
            scalar magneticSusceptibility() const;


        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);

            //- Calculate the non-coupled force
            virtual forceSuSp calcNonCoupled
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
inline const CML::word&
CML::ParamagneticForce<CloudType>::HdotGradHName() const
{
    return HdotGradHName_;
}


template<class CloudType>
inline CML::scalar
CML::ParamagneticForce<CloudType>::magneticSusceptibility() const
{
    return magneticSusceptibility_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParamagneticForce<CloudType>::ParamagneticForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
    HdotGradHName_
    (
        this->coeffs().template lookupOrDefault<word>("HdotGradH", "HdotGradH")
    ),
    HdotGradHInterpPtr_(nullptr),
    magneticSusceptibility_
    (
        readScalar(this->coeffs().lookup("magneticSusceptibility"))
    )
{}


template<class CloudType>
CML::ParamagneticForce<CloudType>::ParamagneticForce
(
    const ParamagneticForce& pf
)
:
    ParticleForce<CloudType>(pf),
    HdotGradHName_(pf.HdotGradHName_),
    HdotGradHInterpPtr_(pf.HdotGradHInterpPtr_),
    magneticSusceptibility_(pf.magneticSusceptibility_)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParamagneticForce<CloudType>::~ParamagneticForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ParamagneticForce<CloudType>::cacheFields(const bool store)
{
    if (store)
    {
        const volVectorField& HdotGradH =
            this->mesh().template lookupObject<volVectorField>(HdotGradHName_);

        HdotGradHInterpPtr_ = interpolation<vector>::New
        (
            this->owner().solution().interpolationSchemes(),
            HdotGradH
        ).ptr();
    }
    else
    {
        deleteDemandDrivenData(HdotGradHInterpPtr_);
    }
}


template<class CloudType>
CML::forceSuSp CML::ParamagneticForce<CloudType>::calcNonCoupled
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

    const interpolation<vector>& HdotGradHInterp = *HdotGradHInterpPtr_;

    value.Su()=
        mass*3.0*constant::electromagnetic::mu0.value()/p.rho()
       *magneticSusceptibility_/(magneticSusceptibility_ + 3)
       *HdotGradHInterp.interpolate(p.coordinates(), p.currentTetIndices());

    return value;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
