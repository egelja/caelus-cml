/*---------------------------------------------------------------------------*\
Copyright (C) 2019 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::DenseDragForce

\*---------------------------------------------------------------------------*/

#ifndef DenseDragForce_HPP
#define DenseDragForce_HPP

#include "ParticleForce.hpp"
#include "interpolation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class DenseDragForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class DenseDragForce
:
    public ParticleForce<CloudType>
{
protected:

    // Protected Data

        //- Name of the carrier volume fraction field
        const word alphacName_;

        //- The carrier volume fraction field
        autoPtr<volScalarField> alphacPtr_;

        //- Interpolation for the carrier volume fraction field
        autoPtr<interpolation<scalar>> alphacInterpPtr_;


public:

    // Constructors

        //- Construct from mesh
        DenseDragForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& typeName
        );

        //- Construct copy
        DenseDragForce(const DenseDragForce<CloudType>& df);


    //- Destructor
    virtual ~DenseDragForce();


    // Member Functions

        // Access

            //- Return the volume fraction interpolation
            const interpolation<scalar>& alphacInterp() const;

        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#include "SchillerNaumannDragForce.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::DenseDragForce<CloudType>::DenseDragForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& typeName
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
    alphacName_(this->coeffs().lookup("alphac")),
    alphacPtr_(nullptr),
    alphacInterpPtr_(nullptr)
{}


template<class CloudType>
CML::DenseDragForce<CloudType>::DenseDragForce
(
    const DenseDragForce<CloudType>& df
)
:
    ParticleForce<CloudType>(df),
    alphacName_(df.alphacName_),
    alphacPtr_(nullptr),
    alphacInterpPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::DenseDragForce<CloudType>::~DenseDragForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CML::interpolation<CML::scalar>&
CML::DenseDragForce<CloudType>::alphacInterp() const
{
    if (!alphacInterpPtr_.valid())
    {
        FatalErrorInFunction
            << "Carrier phase volume-fraction interpolation object not set"
            << abort(FatalError);
    }

    return alphacInterpPtr_();
}


template<class CloudType>
void CML::DenseDragForce<CloudType>::cacheFields(const bool store)
{
    if (store)
    {
        if (!this->mesh().template foundObject<volVectorField>(alphacName_))
        {
            alphacPtr_.reset
            (
                new volScalarField(alphacName_, 1 - this->owner().theta())
            );
        }

        const volScalarField& alphac =
            this->mesh().template lookupObject<volScalarField>(alphacName_);

        alphacInterpPtr_.reset
        (
            interpolation<scalar>::New
            (
                this->owner().solution().interpolationSchemes(),
                alphac
            ).ptr()
        );
    }
    else
    {
        alphacInterpPtr_.clear();
        alphacPtr_.clear();
    }
}


#endif
