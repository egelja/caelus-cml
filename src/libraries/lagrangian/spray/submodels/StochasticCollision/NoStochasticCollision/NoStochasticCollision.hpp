/*---------------------------------------------------------------------------*\
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
    CML::NoStochasticCollision

Description
    Dummy collision model for 'none'

\*---------------------------------------------------------------------------*/

#ifndef NoStochasticCollision_H
#define NoStochasticCollision_H

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
public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoStochasticCollision(const dictionary& dict, CloudType& owner);

        //- Construct copy
        NoStochasticCollision(const NoStochasticCollision<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<StochasticCollisionModel<CloudType> > clone() const
        {
            return autoPtr<StochasticCollisionModel<CloudType> >
            (
                new NoStochasticCollision<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoStochasticCollision();


    // Member Functions

        //- Flag to indicate whether model activates collision model
        virtual bool active() const;

        virtual bool update
        (
            const scalar dt,
            cachedRandom& rndGen,
            vector& pos1,
            scalar& m1,
            scalar& d1,
            scalar& N1,
            vector& U,
            scalar& rho1,
            scalar& T1,
            scalarField& Y1,
            const scalar sigma1,
            const label celli,
            const scalar voli,
            vector& pos2,
            scalar& m2,
            scalar& d2,
            scalar& N2,
            vector& U2,
            scalar& rho2,
            scalar& T2,
            scalarField& Y2,
            const scalar sigma2,
            const label cellj,
            const scalar volj
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

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


template<class CloudType>
bool CML::NoStochasticCollision<CloudType>::update
(
    const scalar dt,
    cachedRandom& rndGen,
    vector& pos1,
    scalar& m1,
    scalar& d1,
    scalar& N1,
    vector& U,
    scalar& rho1,
    scalar& T1,
    scalarField& Y1,
    const scalar sigma1,
    const label celli,
    const scalar voli,
    vector& pos2,
    scalar& m2,
    scalar& d2,
    scalar& N2,
    vector& U2,
    scalar& rho2,
    scalar& T2,
    scalarField& Y2,
    const scalar sigma2,
    const label cellj,
    const scalar volj
) const
{
    return false;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
