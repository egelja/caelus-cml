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
    CML::Rebound

Description
    Simple rebound patch interaction model

\*---------------------------------------------------------------------------*/

#ifndef Rebound_H
#define Rebound_H

#include "PatchInteractionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                           Class Rebound Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class Rebound
:
    public PatchInteractionModel<CloudType>
{
    // Private data

       //- Factor applied to velocity on rebound
       //  Normal rebound = 1
       scalar UFactor_;


public:

    //- Runtime type information
    TypeName("rebound");


    // Constructors

        //- Construct from dictionary
        Rebound(const dictionary& dict, CloudType& cloud);

        //- Construct copy
        Rebound(const Rebound<CloudType>& pim);

        //- Construct and return a clone
        virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
        {
            return autoPtr<PatchInteractionModel<CloudType> >
            (
                new Rebound<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~Rebound();


    // Member Functions
        //- Apply velocity correction
        //  Returns true if particle remains in same cell
        virtual bool correct
        (
            typename CloudType::parcelType& p,
            const polyPatch& pp,
            bool& keepParticle
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::Rebound<CloudType>::Rebound
(
    const dictionary& dict,
    CloudType& cloud
)
:
    PatchInteractionModel<CloudType>(dict, cloud, typeName),
    UFactor_(readScalar(this->coeffDict().lookup("UFactor")))
{}


template<class CloudType>
CML::Rebound<CloudType>::Rebound(const Rebound<CloudType>& pim)
:
    PatchInteractionModel<CloudType>(pim),
    UFactor_(pim.UFactor_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::Rebound<CloudType>::~Rebound()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::Rebound<CloudType>::correct
(
    typename CloudType::parcelType& p,
    const polyPatch& pp,
    bool& keepParticle
)
{
    vector& U = p.U();

    keepParticle = true;
    p.active() = true;

    vector nw;
    vector Up;

    this->owner().patchData(p, pp, nw, Up);

    // Calculate motion relative to patch velocity
    U -= Up;

    scalar Un = U & nw;

    if (Un > 0.0)
    {
        U -= UFactor_*2.0*Un*nw;
    }

    // Return velocity to global space
    U += Up;

    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
