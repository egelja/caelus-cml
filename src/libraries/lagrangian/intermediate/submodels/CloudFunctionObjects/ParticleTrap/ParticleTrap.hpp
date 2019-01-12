/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::ParticleTrap

Description
    Traps particles within a given phase fraction for multi-phase cases.

    Model is activated using:
    \verbatim
    particleTrap1
    {
        type        particleTrap;
        alpha       alpha;      // name of the volume fraction field
        threshold   0.95;       // alpha value below which model is active
    }
    \endverbatim



\*---------------------------------------------------------------------------*/

#ifndef ParticleTrap_H
#define ParticleTrap_H

#include "CloudFunctionObject.hpp"
#include "volFields.hpp"
#include "fvcGrad.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class ParticleTrap Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ParticleTrap
:
    public CloudFunctionObject<CloudType>
{
    // Private Data

        // Typedefs

            //- Convenience typedef for parcel type
            typedef typename CloudType::parcelType parcelType;


        //- Name of vol fraction field
        const word alphaName_;

        //- Pointer to the volume fraction field
        const volScalarField* alphaPtr_;

        //- Gradient of the volume fraction field
        autoPtr<volVectorField> gradAlphaPtr_;

        //- Threshold beyond which model is active
        scalar threshold_;


public:

    //- Runtime type information
    TypeName("particleTrap");


    // Constructors

        //- Construct from dictionary
        ParticleTrap
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        ParticleTrap(const ParticleTrap<CloudType>& pe);

        //- Construct and return a clone
        virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
        {
            return autoPtr<CloudFunctionObject<CloudType> >
            (
                new ParticleTrap<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ParticleTrap();


    // Member Functions

        // Evaluation

            //- Pre-evolve hook
            virtual void preEvolve();

            //- Post-evolve hook
            virtual void postEvolve();

            //- Post-move hook
            virtual void postMove
            (
                typename CloudType::parcelType& p,
                const scalar dt,
                const point& position0,
                bool& keepParticle
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleTrap<CloudType>::ParticleTrap
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
    alphaName_
    (
        this->coeffDict().template lookupOrDefault<word>("alpha", "alpha")
    ),
    alphaPtr_(nullptr),
    gradAlphaPtr_(nullptr),
    threshold_(readScalar(this->coeffDict().lookup("threshold")))
{}


template<class CloudType>
CML::ParticleTrap<CloudType>::ParticleTrap
(
    const ParticleTrap<CloudType>& pt
)
:
    CloudFunctionObject<CloudType>(pt),
    alphaName_(pt.alphaName_),
    alphaPtr_(pt.alphaPtr_),
    gradAlphaPtr_(nullptr),
    threshold_(pt.threshold_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleTrap<CloudType>::~ParticleTrap()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ParticleTrap<CloudType>::preEvolve()
{
    if (alphaPtr_ == nullptr)
    {
        const fvMesh& mesh = this->owner().mesh();
        const volScalarField& alpha =
            mesh.lookupObject<volScalarField>(alphaName_);

        alphaPtr_ = &alpha;
    }

    if (gradAlphaPtr_.valid())
    {
        gradAlphaPtr_() == fvc::grad(*alphaPtr_);
    }
    else
    {
        gradAlphaPtr_.reset(new volVectorField(fvc::grad(*alphaPtr_)));
    }
}


template<class CloudType>
void CML::ParticleTrap<CloudType>::postEvolve()
{
    gradAlphaPtr_.clear();
}


template<class CloudType>
void CML::ParticleTrap<CloudType>::postMove
(
    parcelType& p,
    const scalar,
    const point&,
    bool&
)
{
    if (alphaPtr_->internalField()[p.cell()] < threshold_)
    {
        const vector& gradAlpha = gradAlphaPtr_()[p.cell()];
        vector nHat = gradAlpha/mag(gradAlpha);
        scalar nHatU = nHat & p.U();

        if (nHatU < 0)
        {
            p.U() -= 2*nHat*nHatU;
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
