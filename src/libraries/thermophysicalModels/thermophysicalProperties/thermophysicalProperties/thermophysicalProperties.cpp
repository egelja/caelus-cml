/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "thermophysicalProperties.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(thermophysicalProperties, 0);
    defineRunTimeSelectionTable(thermophysicalProperties,);
    defineRunTimeSelectionTable(thermophysicalProperties, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::thermophysicalProperties::thermophysicalProperties(scalar W)
:
    W_(W)
{}


CML::thermophysicalProperties::thermophysicalProperties(const dictionary& dict)
:
    W_(readScalar(dict.lookup("W")))
{}


// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

CML::autoPtr<CML::thermophysicalProperties>
CML::thermophysicalProperties::New
(
    const word& name
)
{
    if (debug)
    {
        InfoInFunction << "Constructing thermophysicalProperties" << endl;
    }

    ConstructorTable::iterator cstrIter = ConstructorTablePtr_->find(name);

    if (cstrIter == ConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown thermophysicalProperties type "
            << name << nl << nl
            << "Valid thermophysicalProperties types are:" << nl
            << ConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<thermophysicalProperties>(cstrIter()());
}


CML::autoPtr<CML::thermophysicalProperties>
CML::thermophysicalProperties::New
(
    const dictionary& dict
)
{
    if (debug)
    {
        InfoInFunction << "Constructing thermophysicalProperties" << endl;
    }

    const word& thermophysicalPropertiesTypeName = dict.dictName();

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(thermophysicalPropertiesTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown thermophysicalProperties type "
            << thermophysicalPropertiesTypeName << nl << nl
            << "Valid thermophysicalProperties types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<thermophysicalProperties>(cstrIter()(dict));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::thermophysicalProperties::readIfPresent(const dictionary &dict)
{
    dict.readIfPresent("W", W_);
}


void CML::thermophysicalProperties::writeData(Ostream& os) const
{
    os  << W_;
}


void CML::thermophysicalProperties::write(Ostream& os) const
{
    dictionary dict("thermophysicalProperties");
    dict.add("W", W_);
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const thermophysicalProperties& l)
{
    l.writeData(os);
    return os;
}
