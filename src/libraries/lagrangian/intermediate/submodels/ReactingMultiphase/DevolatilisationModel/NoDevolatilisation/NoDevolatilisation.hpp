/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::NoDevolatilisation

Description
    Dummy devolatilisation model for 'none'

\*---------------------------------------------------------------------------*/

#ifndef NoDevolatilisation_H
#define NoDevolatilisation_H

#include "DevolatilisationModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                     Class NoDevolatilisation Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoDevolatilisation
:
    public DevolatilisationModel<CloudType>
{

public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoDevolatilisation(const dictionary&, CloudType& owner);

        //- Construct copy
        NoDevolatilisation(const NoDevolatilisation<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DevolatilisationModel<CloudType> > clone() const
        {
            return autoPtr<DevolatilisationModel<CloudType> >
            (
                new NoDevolatilisation<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoDevolatilisation();


    // Member Functions

        //- Flag to indicate whether model activates devolatilisation model
        virtual bool active() const;

        //- Update model
        virtual void calculate
        (
            const scalar dt,
            const scalar age,
            const scalar mass0,
            const scalar mass,
            const scalar T,
            const scalarField& YGasEff,
            const scalarField& YLiquidEff,
            const scalarField& YSolidEff,
            label& canCombust,
            scalarField& dMassDV
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoDevolatilisation<CloudType>::NoDevolatilisation
(
    const dictionary&,
    CloudType& owner
)
:
    DevolatilisationModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoDevolatilisation<CloudType>::NoDevolatilisation
(
    const NoDevolatilisation<CloudType>& dm
)
:
    DevolatilisationModel<CloudType>(dm.owner_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoDevolatilisation<CloudType>::~NoDevolatilisation()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoDevolatilisation<CloudType>::active() const
{
    return false;
}


template<class CloudType>
void CML::NoDevolatilisation<CloudType>::calculate
(
    const scalar,
    const scalar,
    const scalar,
    const scalar,
    const scalar,
    const scalarField&,
    const scalarField&,
    const scalarField&,
    label& canCombust,
    scalarField&
) const
{
    // Model does not stop combustion taking place
    canCombust = true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
