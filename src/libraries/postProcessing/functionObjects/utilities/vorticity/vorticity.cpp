/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2015 OpenFOAM Foundation
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

#include "vorticity.hpp"
#include "volFields.hpp"
#include "dictionary.hpp"
#include "fvcCurl.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(vorticity, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::vorticity::vorticity
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
    UName_("U"),
    outputName_(typeName)
{
    // Check if the available mesh is an fvMesh, otherwise deactivate
    if (!isA<fvMesh>(obr_))
    {
        active_ = false;
        WarningInFunction
            << "No fvMesh available, deactivating " << name_ << nl
            << endl;
    }

    read(dict);

    if (active_)
    {
        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        volVectorField* vorticityPtr
        (
            new volVectorField
            (
                IOobject
                (
                    outputName_,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedVector("0", dimless/dimTime, vector::zero)
            )
        );

        mesh.objectRegistry::store(vorticityPtr);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::vorticity::~vorticity()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::vorticity::read(const dictionary& dict)
{
    if (active_)
    {
        UName_ = dict.lookupOrDefault<word>("UName", "U");
        if (UName_ != "U")
        {
            outputName_ = typeName + "(" + UName_ + ")";
        }
    }
}


void CML::vorticity::execute()
{
    if (active_)
    {
        const volVectorField& U = obr_.lookupObject<volVectorField>(UName_);

        volVectorField& vorticity =
            const_cast<volVectorField&>
            (
                obr_.lookupObject<volVectorField>(outputName_)
            );

        vorticity = fvc::curl(U);
    }
}


void CML::vorticity::end()
{
    if (active_)
    {
        execute();
    }
}


void CML::vorticity::timeSet()
{
    // Do nothing
}


void CML::vorticity::write()
{
    if (active_)
    {
        const volVectorField& vorticity =
            obr_.lookupObject<volVectorField>(outputName_);

        Info<< type() << " " << name_ << " output:" << nl
            << "    writing field " << vorticity.name() << nl
            << endl;

        vorticity.write();
    }
}


// ************************************************************************* //
