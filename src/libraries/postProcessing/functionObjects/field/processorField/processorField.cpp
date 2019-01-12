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

#include "processorField.hpp"
#include "dictionary.hpp"
#include "Pstream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(processorField, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::processorField::processorField
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    name_(name),
    obr_(obr),
    active_(true)
{
    // Check if the available mesh is an fvMesh otherise deactivate
    if (isA<fvMesh>(obr_))
    {
        read(dict);

        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        volScalarField* procFieldPtr
        (
            new volScalarField
            (
                IOobject
                (
                    "processorID",
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("0", dimless, 0.0)
            )
        );

        mesh.objectRegistry::store(procFieldPtr);
    }
    else
    {
        active_ = false;
        WarningInFunction
            << "No fvMesh available, deactivating " << name_
            << endl;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::processorField::~processorField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::processorField::read(const dictionary& dict)
{
    // do nothing
}


void CML::processorField::execute()
{
    if (active_)
    {
        const volScalarField& procField =
            obr_.lookupObject<volScalarField>("processorID");

        const_cast<volScalarField&>(procField) ==
            dimensionedScalar("procI", dimless, Pstream::myProcNo());
    }
}


void CML::processorField::end()
{
    if (active_)
    {
        execute();
    }
}


void CML::processorField::timeSet()
{
    // Do nothing
}


void CML::processorField::write()
{
    if (active_)
    {
        const volScalarField& procField =
            obr_.lookupObject<volScalarField>("processorID");

        procField.write();
    }
}


// ************************************************************************* //
