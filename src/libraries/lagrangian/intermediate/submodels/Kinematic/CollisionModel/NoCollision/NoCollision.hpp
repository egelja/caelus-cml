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
    CML::NoCollision

Description
    Place holder for 'none' option


\*---------------------------------------------------------------------------*/

#ifndef NoCollision_H
#define NoCollision_H

#include "CollisionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class NoCollision Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoCollision
:
    public CollisionModel<CloudType>
{

public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from components
        NoCollision(const dictionary& dict, CloudType& owner);

        //- Construct copy
        NoCollision(const NoCollision<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<CollisionModel<CloudType> > clone() const
        {
            return autoPtr<CollisionModel<CloudType> >
            (
                new NoCollision<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoCollision();


    // Member Functions

        //- Return the number of times to subcycle the current
        //  timestep to meet the criteria of the collision model.  For
        //  this model this will always be 1.
        virtual label nSubCycles() const;

        //- Flag to indicate whether model activates collision model
        virtual bool active() const;

        // Collision function
        virtual void collide();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoCollision<CloudType>::NoCollision
(
    const dictionary& dict,
    CloudType& owner
)
:
    CollisionModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoCollision<CloudType>::NoCollision
(
    const NoCollision<CloudType>& cm
)
:
    CollisionModel<CloudType>(cm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoCollision<CloudType>::~NoCollision()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::label CML::NoCollision<CloudType>::nSubCycles() const
{
    return 1;
}


template<class CloudType>
bool CML::NoCollision<CloudType>::active() const
{
    return false;
}


template<class CloudType>
void CML::NoCollision<CloudType>::collide()
{}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
