/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::chemistryTabulationMethods::none

Description

\*---------------------------------------------------------------------------*/

#ifndef noChemistryTabulation_HPP
#define noChemistryTabulation_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "chemistryTabulationMethod.hpp"

namespace CML
{
namespace chemistryTabulationMethods
{

/*---------------------------------------------------------------------------*\
                           Class none Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class none
:
    public chemistryTabulationMethod<CompType, ThermoType>
{

    //- Disallow default bitwise copy construct
    none(const none&);


public:

    //- Runtime type information
    TypeName("none");


    //- Construct from dictionary
    none
    (
        const dictionary& chemistryProperties,
        TDACChemistryModel<CompType, ThermoType>& chemistry
    );


    // Destructor
    virtual ~none();


    // Member Functions

    //- Return the size of the binary tree
    virtual label size()
    {
        NotImplemented;
        return 0;
    }

    virtual void writePerformance()
    {
        NotImplemented;
    }

    //- Find the closest stored leaf of phiQ and store the result in
    //  RphiQ or return false.
    virtual bool retrieve
    (
        const CML::scalarField& phiq,
        scalarField& Rphiq
    )
    {
        NotImplemented;
        return false;
    }

    // Add information to the tabulation.This function can grow an
    // existing point or add a new leaf to the binary tree Input : phiq
    // the new composition to store Rphiq the mapping of the new
    // composition point
    virtual label add
    (
        const scalarField& phiq,
        const scalarField& Rphiq,
        const scalar rho,
        const scalar deltaT
    )
    {
        NotImplemented;
        return false;
    }

    virtual bool update()
    {
        NotImplemented;
        return false;
    }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace chemistryTabulationMethods
} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::chemistryTabulationMethods::none<CompType, ThermoType>::none
(
    const dictionary& chemistryProperties,
    TDACChemistryModel<CompType, ThermoType>& chemistry
)
:
    chemistryTabulationMethod<CompType, ThermoType>
    (
        chemistryProperties,
        chemistry
    )
{
    this->active_ = false;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::chemistryTabulationMethods::none<CompType, ThermoType>::~none()
{}


#endif
