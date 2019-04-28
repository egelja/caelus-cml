/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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
    CML::ThermoCombustion

Description
    Thermo model wrapper for combustion models


\*---------------------------------------------------------------------------*/

#ifndef ThermoCombustion_HPP
#define ThermoCombustion_HPP

#include "autoPtr.hpp"
#include "CombustionModel_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    class ThermoCombustion Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class ThermoCombustion
:
    public CombustionModel<ReactionThermo>
{
protected:

    //- Thermo
    ReactionThermo& thermo_;


public:

    //- Construct from components
    ThermoCombustion
    (
        const word& modelType,
        ReactionThermo& thermo,
        const compressible::turbulenceModel& turb
    );


    //- Destructor
    virtual ~ThermoCombustion();


    // Member Functions

    //- Return access to the thermo package
    virtual ReactionThermo& thermo();

    //- Return const access to the thermo package
    virtual const ReactionThermo& thermo() const;

};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::ThermoCombustion<ReactionThermo>::ThermoCombustion
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb
)
:
    CombustionModel<ReactionThermo>
    (
        modelType,
        thermo,
        turb,
        combustionModel::combustionPropertiesName
    ),
    thermo_(thermo)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::ThermoCombustion<ReactionThermo>::~ThermoCombustion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo>
ReactionThermo&
CML::ThermoCombustion<ReactionThermo>::thermo()
{
    return thermo_;
}


template<class ReactionThermo>
const ReactionThermo&
CML::ThermoCombustion<ReactionThermo>::thermo() const
{
    return thermo_;
}


#endif
