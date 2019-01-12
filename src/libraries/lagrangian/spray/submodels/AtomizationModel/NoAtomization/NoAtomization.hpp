/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::NoAtomization

Description
    Dummy phase change model for 'none'

\*---------------------------------------------------------------------------*/

#ifndef NoAtomization_H
#define NoAtomization_H

#include "AtomizationModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class NoAtomization Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoAtomization
:
    public AtomizationModel<CloudType>
{
public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoAtomization(const dictionary& dict, CloudType& cloud);

        //- Construct copy
        NoAtomization(const NoAtomization<CloudType>& am);

        //- Construct and return a clone
        virtual autoPtr<AtomizationModel<CloudType> > clone() const
        {
            return autoPtr<AtomizationModel<CloudType> >
            (
                new NoAtomization<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoAtomization();


    // Member Functions

        //- Flag to indicate whether model activates atomization model
        virtual bool active() const;

        //- Initial value of liquidCore
        virtual scalar initLiquidCore() const;

        //- Flag to indicate if chi needs to be calculated
        virtual bool calcChi() const;

        virtual void update
        (
            const scalar dt,
            scalar& d,
            scalar& liquidCore,
            scalar& tc,
            const scalar rho,
            const scalar mu,
            const scalar sigma,
            const scalar volFlowRate,
            const scalar rhoAv,
            const scalar Urel,
            const vector& pos,
            const vector& injectionPos,
            const scalar pAmbient,
            const scalar chi,
            Random& rndGen
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoAtomization<CloudType>::NoAtomization
(
    const dictionary& dict,
    CloudType& owner
)
:
    AtomizationModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoAtomization<CloudType>::NoAtomization
(
    const NoAtomization<CloudType>& am
)
:
    AtomizationModel<CloudType>(am)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoAtomization<CloudType>::~NoAtomization()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoAtomization<CloudType>::active() const
{
    return false;
}


template<class CloudType>
CML::scalar CML::NoAtomization<CloudType>::initLiquidCore() const
{
    return 0.0;
}


template<class CloudType>
bool CML::NoAtomization<CloudType>::calcChi() const
{
    return false;
}


template<class CloudType>
void CML::NoAtomization<CloudType>::update
(
    const scalar dt,
    scalar& d,
    scalar& liquidCore,
    scalar& tc,
    const scalar rho,
    const scalar mu,
    const scalar sigma,
    const scalar volFlowRate,
    const scalar rhoAv,
    const scalar Urel,
    const vector& pos,
    const vector& injectionPos,
    const scalar pAmbient,
    const scalar chi,
    Random& rndGen
) const
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
