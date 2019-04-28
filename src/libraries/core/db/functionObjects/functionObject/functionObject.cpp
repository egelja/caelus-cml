/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "functionObject.hpp"
#include "dictionary.hpp"
#include "dlLibraryTable.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineRunTimeSelectionTable(CML::functionObject, dictionary);
int CML::functionObject::debug(CML::debug::debugSwitch("functionObject", 0));


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::functionObject::functionObject(const word& name)
:
    name_(name)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::functionObject> CML::functionObject::New
(
    const word& name,
    const Time& t,
    const dictionary& functionDict
)
{
    const word functionType(functionDict.lookup("type"));

    if (debug)
    {
        Info<< "Selecting function " << functionType << endl;
    }

    const_cast<Time&>(t).libs().open
    (
        functionDict,
        "functionObjectLibs",
        dictionaryConstructorTablePtr_
    );

    if (!dictionaryConstructorTablePtr_)
    {
        FatalErrorInFunction
            << "Unknown function type "
            << functionType << nl << nl
            << "Table of functionObjects is empty" << endl
            << exit(FatalError);
    }

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(functionType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown function type "
            << functionType << nl << nl
            << "Valid functions are : " << nl
            << dictionaryConstructorTablePtr_->sortedToc() << endl
            << exit(FatalError);
    }

    return autoPtr<functionObject>(cstrIter()(name, t, functionDict));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::functionObject::~functionObject()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::word& CML::functionObject::name() const
{
    return name_;
}


bool CML::functionObject::end()
{
    return execute(false);
}


bool CML::functionObject::timeSet()
{
    return false;
}


bool CML::functionObject::adjustTimeStep()
{
    return false;
}


CML::autoPtr<CML::functionObject> CML::functionObject::iNew::operator()
(
    const word& name,
    Istream& is
) const
{
    dictionary dict(is);
    return functionObject::New(name, time_, dict);
}


// ************************************************************************* //
