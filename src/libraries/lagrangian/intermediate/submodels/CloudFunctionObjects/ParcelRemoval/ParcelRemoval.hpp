/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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
    CML::ParcelRemoval

Description
    Traps particles within a given phase fraction for multi-phase cases

    Model is activated using:
    \verbatim
    ParcelRemoval
    {
        type        parcelRemoval;
        alphaName   alpha1;      // name volume fraction field
        threshold   0.95;       // alpha value below which model is active
    }
    \endverbatim



\*---------------------------------------------------------------------------*/

#ifndef ParcelRemoval_H
#define ParcelRemoval_H

#include "CloudFunctionObject.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class ParcelRemoval Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ParcelRemoval
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

        //- Threshold beyond which model is active
        scalar threshold_;


public:

    //- Runtime type information
    TypeName("parcelRemoval");


    // Constructors

        //- Construct from dictionary
        ParcelRemoval
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        ParcelRemoval(const ParcelRemoval<CloudType>& pe);

        //- Construct and return a clone
        virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
        {
            return autoPtr<CloudFunctionObject<CloudType> >
            (
                new ParcelRemoval<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ParcelRemoval();


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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvcGrad.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParcelRemoval<CloudType>::ParcelRemoval
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
    alphaName_
    (
        this->coeffDict().template lookupOrDefault<word>("alphaName", "alpha")
    ),
    alphaPtr_(nullptr),
    threshold_(readScalar(this->coeffDict().lookup("threshold")))
{}


template<class CloudType>
CML::ParcelRemoval<CloudType>::ParcelRemoval
(
    const ParcelRemoval<CloudType>& pt
)
:
    CloudFunctionObject<CloudType>(pt),
    alphaName_(pt.alphaName_),
    alphaPtr_(pt.alphaPtr_),
    threshold_(pt.threshold_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParcelRemoval<CloudType>::~ParcelRemoval()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ParcelRemoval<CloudType>::preEvolve()
{
    if (alphaPtr_ == nullptr)
    {
        const fvMesh& mesh = this->owner().mesh();
        const volScalarField& alpha =
            mesh.lookupObject<volScalarField>(alphaName_);

        alphaPtr_ = &alpha;
    }
}


template<class CloudType>
void CML::ParcelRemoval<CloudType>::postEvolve()
{
}


template<class CloudType>
void CML::ParcelRemoval<CloudType>::postMove
(
    parcelType& p,
    const scalar,
    const point&,
    bool& keepParticle
)
{
    bool& active = p.active();
    if (alphaPtr_->internalField()[p.cell()] < threshold_)
    {
        keepParticle = false;
        active = false;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
