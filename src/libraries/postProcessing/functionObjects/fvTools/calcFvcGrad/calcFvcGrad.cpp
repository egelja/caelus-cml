/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2015 OpenFOAM Foundation
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

#include "calcFvcGrad.hpp"
#include "volFields.hpp"
#include "dictionary.hpp"
#include "calcFvcGrad.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(calcFvcGrad, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::calcFvcGrad::calcFvcGrad
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    name_(name),
    obr_(obr),
    active_(true),
    fieldName_("undefined-fieldName"),
    resultName_("undefined-resultName")
{
    // Check if the available mesh is an fvMesh, otherwise deactivate
    if (!isA<fvMesh>(obr_))
    {
        active_ = false;
        WarningInFunction
            << "No fvMesh available, deactivating." << nl
            << endl;
    }

    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::calcFvcGrad::~calcFvcGrad()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::calcFvcGrad::read(const dictionary& dict)
{
    if (active_)
    {
        dict.lookup("fieldName") >> fieldName_;
        dict.lookup("resultName") >> resultName_;

        if (resultName_ == "none")
        {
            resultName_ = "fvc::grad(" + fieldName_ + ")";
        }
    }
}


void CML::calcFvcGrad::execute()
{
    if (active_)
    {
        bool processed = false;

        calcGrad<scalar>(fieldName_, resultName_, processed);
        calcGrad<vector>(fieldName_, resultName_, processed);

        if (!processed)
        {
            WarningInFunction
                << "Unprocessed field " << fieldName_ << endl;
        }
    }
}


void CML::calcFvcGrad::end()
{
    if (active_)
    {
        execute();
    }
}


void CML::calcFvcGrad::timeSet()
{
    // Do nothing
}


void CML::calcFvcGrad::write()
{
    if (active_)
    {
        if (obr_.foundObject<regIOobject>(resultName_))
        {
            const regIOobject& field =
                obr_.lookupObject<regIOobject>(resultName_);

            Info<< type() << " " << name_ << " output:" << nl
                << "    writing field " << field.name() << nl << endl;

            field.write();
        }
    }
}


// ************************************************************************* //
