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
    CML::NoComposition

Description
    Dummy class for 'none' option - will raise an error if any functions are
    called that require return values.


\*---------------------------------------------------------------------------*/

#ifndef NoComposition_H
#define NoComposition_H

#include "CompositionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class NoComposition Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoComposition
:
    public CompositionModel<CloudType>
{
public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoComposition(const dictionary& dict, CloudType& owner);

        //- Construct copy
        NoComposition(const NoComposition<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<CompositionModel<CloudType> > clone() const
        {
            return autoPtr<CompositionModel<CloudType> >
            (
                new NoComposition<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoComposition();


    // Member Functions

        // Access

            // Mixture properties

                //- Return the list of mixture mass fractions
                //  If only 1 phase, return component fractions of that phase
                virtual const scalarField& YMixture0() const;

                // Indices of gas, liquid and solid phases in phase properties
                // list

                    //- Gas id
                    virtual label idGas() const;

                    //- Liquid id
                    virtual label idLiquid() const;

                    //- Solid id
                    virtual label idSolid() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoComposition<CloudType>::NoComposition
(
    const dictionary&,
    CloudType& owner
)
:
    CompositionModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoComposition<CloudType>::NoComposition
(
    const NoComposition<CloudType>& cm
)
:
    CompositionModel<CloudType>(cm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoComposition<CloudType>::~NoComposition()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CML::scalarField& CML::NoComposition<CloudType>::YMixture0() const
{
    return scalarField::null();
}


template<class CloudType>
CML::label CML::NoComposition<CloudType>::idGas() const
{
    return -1;
}


template<class CloudType>
CML::label CML::NoComposition<CloudType>::idLiquid() const
{
    return -1;
}


template<class CloudType>
CML::label CML::NoComposition<CloudType>::idSolid() const
{
    return -1;
}

#endif
// ************************************************************************* //
