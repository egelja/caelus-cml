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
    CML::NoSurfaceReaction

Description
    Dummy surface reaction model for 'none'

\*---------------------------------------------------------------------------*/

#ifndef NoSurfaceReaction_H
#define NoSurfaceReaction_H

#include "SurfaceReactionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                     Class NoSurfaceReaction Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoSurfaceReaction
:
    public SurfaceReactionModel<CloudType>
{
public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoSurfaceReaction(const dictionary& dict, CloudType& owner);

        //- Construct copy
        NoSurfaceReaction(const NoSurfaceReaction<CloudType>& srm);

        //- Construct and return a clone
        virtual autoPtr<SurfaceReactionModel<CloudType> > clone() const
        {
            return autoPtr<SurfaceReactionModel<CloudType> >
            (
                new NoSurfaceReaction<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoSurfaceReaction();


    // Member Functions

        //- Flag to indicate whether model activates devolatisation model
        virtual bool active() const;

        //- Update surface reactions
        virtual scalar calculate
        (
            const scalar dt,
            const label celli,
            const scalar d,
            const scalar T,
            const scalar Tc,
            const scalar pc,
            const scalar rhoc,
            const scalar mass,
            const scalarField& YGas,
            const scalarField& YLiquid,
            const scalarField& YSolid,
            const scalarField& YMixture,
            const scalar N,
            scalarField& dMassGas,
            scalarField& dMassLiquid,
            scalarField& dMassSolid,
            scalarField& dMassSRCarrier
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoSurfaceReaction<CloudType>::NoSurfaceReaction
(
    const dictionary&,
    CloudType& owner
)
:
    SurfaceReactionModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoSurfaceReaction<CloudType>::NoSurfaceReaction
(
    const NoSurfaceReaction<CloudType>& srm
)
:
    SurfaceReactionModel<CloudType>(srm.owner_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoSurfaceReaction<CloudType>::~NoSurfaceReaction()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoSurfaceReaction<CloudType>::active() const
{
    return false;
}


template<class CloudType>
CML::scalar CML::NoSurfaceReaction<CloudType>::calculate
(
    const scalar,
    const label,
    const scalar,
    const scalar,
    const scalar,
    const scalar,
    const scalar,
    const scalar,
    const scalarField&,
    const scalarField&,
    const scalarField&,
    const scalarField&,
    const scalar,
    scalarField&,
    scalarField&,
    scalarField&,
    scalarField&
) const
{
    return 0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
