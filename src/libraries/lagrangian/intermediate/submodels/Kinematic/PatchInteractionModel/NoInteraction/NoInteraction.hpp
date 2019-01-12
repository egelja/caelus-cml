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
    CML::NoInteraction

Description
    Dummy class for 'none' option - will raise an error if any functions are
    called that require return values.

\*---------------------------------------------------------------------------*/

#ifndef NoInteraction_H
#define NoInteraction_H

#include "PatchInteractionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class NoInteraction Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoInteraction
:
    public PatchInteractionModel<CloudType>
{
public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoInteraction(const dictionary& dict, CloudType& cloud);

        //- Construct copy
        NoInteraction(const NoInteraction<CloudType>& pim);

        //- Construct and return a clone
        virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
        {
            return autoPtr<PatchInteractionModel<CloudType> >
            (
                new NoInteraction<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoInteraction();


    // Member Functions

        //- Flag to indicate whether model activates patch interaction model
        virtual bool active() const;

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
CML::NoInteraction<CloudType>::NoInteraction
(
    const dictionary&,
    CloudType& owner
)
:
    PatchInteractionModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoInteraction<CloudType>::NoInteraction
(
    const NoInteraction<CloudType>& pim
)
:
    PatchInteractionModel<CloudType>(pim)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoInteraction<CloudType>::~NoInteraction()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoInteraction<CloudType>::active() const
{
    return false;
}


template<class CloudType>
bool CML::NoInteraction<CloudType>::correct
(
    typename CloudType::parcelType& p,
    const polyPatch&,
    bool&
)
{
    return false;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
