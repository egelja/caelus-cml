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
    CML::NoPhaseChange

Description
    Dummy phase change model for 'none'

\*---------------------------------------------------------------------------*/

#ifndef NoPhaseChange_H
#define NoPhaseChange_H

#include "PhaseChangeModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class NoPhaseChange Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoPhaseChange
:
    public PhaseChangeModel<CloudType>
{
public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoPhaseChange(const dictionary&, CloudType&);

        //- Construct copy
        NoPhaseChange(const NoPhaseChange<CloudType>& pcm);

        //- Construct and return a clone
        virtual autoPtr<PhaseChangeModel<CloudType> > clone() const
        {
            return autoPtr<PhaseChangeModel<CloudType> >
            (
                new NoPhaseChange<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoPhaseChange();


    // Member Functions

        //- Flag to indicate whether model activates phase change model
        virtual bool active() const;

        //- Update model
        virtual void calculate
        (
            const scalar dt,
            const label celli,
            const scalar Re,
            const scalar Pr,
            const scalar d,
            const scalar nu,
            const scalar T,
            const scalar Ts,
            const scalar pc,
            const scalar Tc,
            const scalarField& X,
            scalarField& dMassPC
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoPhaseChange<CloudType>::NoPhaseChange
(
    const dictionary&,
    CloudType& owner
)
:
    PhaseChangeModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoPhaseChange<CloudType>::NoPhaseChange
(
    const NoPhaseChange<CloudType>& pcm
)
:
    PhaseChangeModel<CloudType>(pcm.owner_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoPhaseChange<CloudType>::~NoPhaseChange()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoPhaseChange<CloudType>::active() const
{
    return false;
}


template<class CloudType>
void CML::NoPhaseChange<CloudType>::calculate
(
    const scalar dt,
    const label celli,
    const scalar Re,
    const scalar Pr,
    const scalar d,
    const scalar nu,
    const scalar T,
    const scalar Ts,
    const scalar pc,
    const scalar Tc,
    const scalarField& X,
    scalarField& dMassPC
) const
{
    // Nothing to do...
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
