/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "fieldValue.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(fieldValue, 0);
    defineRunTimeSelectionTable(fieldValue, dictionary);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void CML::fieldValue::read(const dictionary& dict)
{
    if (active_)
    {
        dict_ = dict;

        log_ = dict.lookupOrDefault<Switch>("log", true);
        dict.lookup("fields") >> fields_;
        dict.lookup("valueOutput") >> valueOutput_;
    }
}


void CML::fieldValue::write()
{
    if (active_)
    {
        functionObjectFile::write();

        Info(log_)<< type() << " " << name_ << " output:" << nl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fieldValue::fieldValue
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const word& valueType,
    const bool loadFromFiles
)
:
    functionObjectFile(obr, name, valueType),
    name_(name),
    obr_(obr),
    dict_(dict),
    active_(true),
    log_(true),
    sourceName_(word::null),
    fields_(dict.lookup("fields")),
    valueOutput_(dict.lookup("valueOutput")),
    resultDict_(fileName("name"), dictionary::null)
{
    // Only active if obr is an fvMesh
    if (isA<fvMesh>(obr_))
    {
        read(dict);
    }
    else
    {
        WarningInFunction
            << "No fvMesh available, deactivating " << name << nl
            << endl;
        active_ = false;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fieldValue::~fieldValue()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fieldValue::execute()
{
    // Do nothing
}


void CML::fieldValue::end()
{
    // Do nothing
}


void CML::fieldValue::timeSet()
{
    // Do nothing
}


void CML::fieldValue::updateMesh(const mapPolyMesh&)
{
    // Do nothing
}


void CML::fieldValue::movePoints(const pointField&)
{
    // Do nothing
}


// ************************************************************************* //
