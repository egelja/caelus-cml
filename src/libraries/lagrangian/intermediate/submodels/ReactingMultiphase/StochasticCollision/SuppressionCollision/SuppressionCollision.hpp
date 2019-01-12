/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2017 OpenFOAM Foundation
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
    CML::SuppressionCollision

Description
    Inter-cloud collision model, whereby the \c canReact flag can be used
    to inhibit devolatilisation and surface reactions

\*---------------------------------------------------------------------------*/

#ifndef SuppressionCollision_HPP
#define SuppressionCollision_HPP

#include "StochasticCollisionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                    Class SuppressionCollision Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class SuppressionCollision
:
    public StochasticCollisionModel<CloudType>
{
protected:

    // Protected data

        //- Name of cloud used for suppression
        const word suppressionCloud_;

        //- Suppressed parcel type - optional
        const label suppressedParcelType_;


   // Protected Member Functions

        //- Update the model
        virtual void collide
        (
            typename CloudType::parcelType::trackingData& td,
            const scalar dt
        );


public:

    //- Runtime type information
    TypeName("suppressionCollision");


    // Constructors

        //- Construct from dictionary
        SuppressionCollision(const dictionary& dict, CloudType& owner);

        //- Construct copy
        SuppressionCollision(const SuppressionCollision<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<StochasticCollisionModel<CloudType>> clone() const
        {
            return autoPtr<StochasticCollisionModel<CloudType>>
            (
                new SuppressionCollision<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~SuppressionCollision();
};


} // End namespace CML


#include "kinematicCloud.hpp"

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::SuppressionCollision<CloudType>::collide
(
    typename CloudType::parcelType::trackingData& td,
    const scalar dt
)
{
    const kinematicCloud& sc =
        this->owner().mesh().template
        lookupObject<kinematicCloud>(suppressionCloud_);

    volScalarField vDotSweep(sc.vDotSweep());

    dimensionedScalar Dt("dt", dimTime, dt);
    volScalarField P(type() + ":p", 1.0 - exp(-vDotSweep*Dt));

    forAllIter(typename CloudType, this->owner(), iter)
    {
        typename CloudType::parcelType& p = iter();
        label celli = p.cell();

        scalar xx = this->owner().rndGen().template sample01<scalar>();

        if (xx < P[celli])
        {
            p.canCombust() = -1;
            p.typeId() = max(p.typeId(), suppressedParcelType_);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::SuppressionCollision<CloudType>::SuppressionCollision
(
    const dictionary& dict,
    CloudType& owner
)
:
    StochasticCollisionModel<CloudType>(dict, owner, typeName),
    suppressionCloud_(this->coeffDict().lookup("suppressionCloud")),
    suppressedParcelType_
    (
        this->coeffDict().lookupOrDefault("suppressedParcelType", -1)
    )
{}


template<class CloudType>
CML::SuppressionCollision<CloudType>::SuppressionCollision
(
    const SuppressionCollision<CloudType>& cm
)
:
    StochasticCollisionModel<CloudType>(cm),
    suppressionCloud_(cm.suppressionCloud_),
    suppressedParcelType_(cm.suppressedParcelType_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::SuppressionCollision<CloudType>::~SuppressionCollision()
{}


#endif
