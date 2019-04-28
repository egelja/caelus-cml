/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
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
    CML::NoStochasticCollision

Description
    Dummy collision model for 'none'

\*---------------------------------------------------------------------------*/

#ifndef NoStochasticCollision_HPP
#define NoStochasticCollision_HPP

#include "StochasticCollisionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                    Class NoStochasticCollision Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoStochasticCollision
:
    public StochasticCollisionModel<CloudType>
{
protected:

    // Protected Member Functions

        //- Update the model
        virtual void collide
        (
            typename CloudType::parcelType::trackingData& td,
            const scalar dt
        );


public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoStochasticCollision(const dictionary& dict, CloudType& owner);

        //- Construct copy
        NoStochasticCollision(const NoStochasticCollision<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<StochasticCollisionModel<CloudType>> clone() const
        {
            return autoPtr<StochasticCollisionModel<CloudType>>
            (
                new NoStochasticCollision<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoStochasticCollision();


    // Member Functions

        //- Flag to indicate whether model activates collision model
        virtual bool active() const;
};


} // End namespace CML


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::NoStochasticCollision<CloudType>::collide
(
    typename CloudType::parcelType::trackingData&,
    const scalar
)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoStochasticCollision<CloudType>::NoStochasticCollision
(
    const dictionary& dict,
    CloudType& owner
)
:
    StochasticCollisionModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoStochasticCollision<CloudType>::NoStochasticCollision
(
    const NoStochasticCollision<CloudType>& cm
)
:
    StochasticCollisionModel<CloudType>(cm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoStochasticCollision<CloudType>::~NoStochasticCollision()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoStochasticCollision<CloudType>::active() const
{
    return false;
}


#endif
