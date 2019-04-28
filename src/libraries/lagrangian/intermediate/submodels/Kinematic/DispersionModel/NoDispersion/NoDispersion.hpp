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
    CML::NoDispersion

Description
    Place holder for 'none' option

\*---------------------------------------------------------------------------*/

#ifndef NoDispersion_H
#define NoDispersion_H

#include "DispersionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class NoDispersion Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoDispersion
:
    public DispersionModel<CloudType>
{
public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from components
        NoDispersion(const dictionary& dict, CloudType& owner);

        //- Construct copy
        NoDispersion(const NoDispersion<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType> > clone() const
        {
            return autoPtr<DispersionModel<CloudType> >
            (
                new NoDispersion<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoDispersion();


    // Member Functions

        //- Flag to indicate whether model activates injection model
        virtual bool active() const;

        //- Update (disperse particles)
        virtual vector update
        (
            const scalar dt,
            const label celli,
            const vector& U,
            const vector& Uc,
            vector& UTurb,
            scalar& tTurb
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoDispersion<CloudType>::NoDispersion(const dictionary&, CloudType& owner)
:
    DispersionModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoDispersion<CloudType>::NoDispersion(const NoDispersion<CloudType>& dm)
:
    DispersionModel<CloudType>(dm.owner_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoDispersion<CloudType>::~NoDispersion()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoDispersion<CloudType>::active() const
{
    return false;
}


template<class CloudType>
CML::vector CML::NoDispersion<CloudType>::update
(
    const scalar,
    const label,
    const vector&,
    const vector& Uc,
    vector&,
    scalar&
)
{
    return Uc;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
