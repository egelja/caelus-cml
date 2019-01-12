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

\*---------------------------------------------------------------------------*/

#include "phasePropertiesList.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::phasePropertiesList::phasePropertiesList()
:
    props_(),
    phaseTypeNames_(),
    stateLabels_()
{}


CML::phasePropertiesList::phasePropertiesList
(
    Istream& is,
    const wordList& gasNames,
    const wordList& liquidNames,
    const wordList& solidNames
)
:
    props_(is),
    phaseTypeNames_(),
    stateLabels_()
{
    forAll(props_, i)
    {
        props_[i].reorder(gasNames, liquidNames, solidNames);
    }

    phaseTypeNames_.setSize(props_.size());
    stateLabels_.setSize(props_.size());
    forAll(props_, i)
    {
        phaseTypeNames_[i] = props_[i].phaseTypeName();
        stateLabels_[i] = props_[i].stateLabel();
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::phasePropertiesList::~phasePropertiesList()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::List<CML::phaseProperties>&
CML::phasePropertiesList::props() const
{
    return props_;
}


const CML::wordList& CML::phasePropertiesList::phaseTypes() const
{
    return phaseTypeNames_;
}


const CML::wordList& CML::phasePropertiesList::stateLabels() const
{
    return stateLabels_;
}


CML::label CML::phasePropertiesList::size() const
{
    return props_.size();
}


const CML::phaseProperties&
CML::phasePropertiesList::operator[](const label phaseI) const
{
    return props_[phaseI];
}


// ************************************************************************* //
