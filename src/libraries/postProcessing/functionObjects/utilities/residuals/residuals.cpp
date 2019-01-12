/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
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

#include "residuals.hpp"
#include "volFields.hpp"
#include "dictionary.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(residuals, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::residuals::residuals
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    functionObjectFile(obr, name, typeName),
    name_(name),
    obr_(obr),
    active_(true),
    fieldSet_()
{
    // Check if the available mesh is an fvMesh otherwise deactivate
    if (!isA<fvMesh>(obr_))
    {
        active_ = false;
        WarningInFunction
            << "No fvMesh available, deactivating " << name_
            << endl;
    }

    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::residuals::~residuals()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::residuals::read(const dictionary& dict)
{
    if (active_)
    {
        dict.lookup("fields") >> fieldSet_;
    }
}


void CML::residuals::writeFileHeader(const label i)
{
    if (Pstream::master())
    {
        writeHeader(file(), "Residuals");
        writeCommented(file(), "Time");

        forAll(fieldSet_, fieldI)
        {
            writeTabbed(file(), fieldSet_[fieldI]);
        }

        file() << endl;
    }
}


void CML::residuals::execute()
{
    // Do nothing - only valid on write
}


void CML::residuals::end()
{
    // Do nothing - only valid on write
}


void CML::residuals::timeSet()
{
    // Do nothing - only valid on write
}


void CML::residuals::write()
{
    if (active_)
    {
        functionObjectFile::write();

        if (Pstream::master())
        {
            file()<< obr_.time().value();

            forAll(fieldSet_, fieldI)
            {
                const word& fieldName = fieldSet_[fieldI];

                writeResidual<scalar>(fieldName);
                writeResidual<vector>(fieldName);
                writeResidual<sphericalTensor>(fieldName);
                writeResidual<symmTensor>(fieldName);
                writeResidual<tensor>(fieldName);
            }

            file() << endl;
        }
    }
}

// ************************************************************************* //
