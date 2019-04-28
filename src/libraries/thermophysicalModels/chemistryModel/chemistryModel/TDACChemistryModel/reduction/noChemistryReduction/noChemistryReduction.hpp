/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::chemistryReductionMethods::none

Description


\*---------------------------------------------------------------------------*/

#ifndef noChemistryReduction_HPP
#define noChemistryReduction_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace chemistryReductionMethods
{

/*---------------------------------------------------------------------------*\
                       Class none Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class none
:
    public chemistryReductionMethod<CompType, ThermoType>
{

public:

    //- Runtime type information
    TypeName("none");


    //- Construct from components
    none
    (
        const IOdictionary& dict,
        TDACChemistryModel<CompType, ThermoType>& chemistry
    );


    //- Destructor
    virtual ~none()
    {}


    // Member Functions

    //- Reduce the mechanism
    virtual void reduceMechanism
    (
        const scalarField &c,
        const scalar T,
        const scalar p
    );

};


} // End namespace chemistryReductionMethods
} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
CML::chemistryReductionMethods::none<CompType, ThermoType>::none
(
    const IOdictionary& dict,
    TDACChemistryModel<CompType, ThermoType>& chemistry
)
:
    chemistryReductionMethod<CompType, ThermoType>(dict, chemistry)
{
    this->active_ = false;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CompType, class ThermoType>
void CML::chemistryReductionMethods::none<CompType, ThermoType>::
reduceMechanism
(
    const scalarField &c,
    const scalar T,
    const scalar p
)
{
    NotImplemented;
}


#endif
