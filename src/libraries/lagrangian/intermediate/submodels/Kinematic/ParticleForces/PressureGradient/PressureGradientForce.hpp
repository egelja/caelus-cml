/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::PressureGradientForce

Description
    Calculates particle pressure gradient force


\*---------------------------------------------------------------------------*/

#ifndef PressureGradientForce_H
#define PressureGradientForce_H

#include "ParticleForce.hpp"
#include "volFields.hpp"
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
    // Private data

        //- Name of velocity field
        const word UName_;

        //- Velocity gradient field
        const volTensorField* gradUPtr_;


public:

    //- Runtime type information
    TypeName("pressureGradient");


    // Constructors

        //- Construct from mesh
        PressureGradientForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
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

            //- Return const access to the velocity gradient field
            inline const volTensorField& gradU() const;


        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);

            //- Calculate the non-coupled force
            virtual forceSuSp calcCoupled
            (
                const typename CloudType::parcelType& p,
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
const CML::volTensorField& CML::PressureGradientForce<CloudType>::gradU()
const
{
    if (gradUPtr_)
    {
        return *gradUPtr_;
    }
    else
    {
        FatalErrorIn
        (
            "const volTensorField& PressureGradientForce<CloudType>::gradU()"
            "const"
        )   << "gradU field not allocated" << abort(FatalError);

        return *reinterpret_cast<const volTensorField*>(0);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PressureGradientForce<CloudType>::PressureGradientForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
    UName_(this->coeffs().lookup("U")),
    gradUPtr_(NULL)
{}


template<class CloudType>
CML::PressureGradientForce<CloudType>::PressureGradientForce
(
    const PressureGradientForce& pgf
)
:
    ParticleForce<CloudType>(pgf),
    UName_(pgf.UName_),
    gradUPtr_(NULL)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PressureGradientForce<CloudType>::~PressureGradientForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::PressureGradientForce<CloudType>::cacheFields(const bool store)
{
    if (store)
    {
        const volVectorField& U = this->mesh().template
            lookupObject<volVectorField>(UName_);
        gradUPtr_ = fvc::grad(U).ptr();
    }
    else
    {
        if (gradUPtr_)
        {
            delete gradUPtr_;
            gradUPtr_ = NULL;
        }
    }
}


template<class CloudType>
CML::forceSuSp CML::PressureGradientForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value(vector::zero, 0.0);

    const volTensorField& gradU = *gradUPtr_;
    value.Su() = mass*p.rhoc()/p.rho()*(p.U() & gradU[p.cell()]);

    return value;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
