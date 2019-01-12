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

#include "Q.hpp"
#include "volFields.hpp"
#include "dictionary.hpp"
#include "fvcGrad.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(Q, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::Q::Q
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
    UName_("U")
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

        volScalarField* QPtr
        (
            new volScalarField
            (
                IOobject
                (
                    type(),
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("0", dimless/sqr(dimTime), 0.0)
            )
        );

        mesh.objectRegistry::store(QPtr);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::Q::~Q()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::Q::read(const dictionary& dict)
{
    if (active_)
    {
        UName_ = dict.lookupOrDefault<word>("UName", "U");
    }
}


void CML::Q::execute()
{
    if (active_)
    {
        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        const volVectorField& U =
            mesh.lookupObject<volVectorField>(UName_);

        const volTensorField gradU(fvc::grad(U));

        volScalarField& Q =
            const_cast<volScalarField&>
            (
                mesh.lookupObject<volScalarField>(type())
            );

        Q = 0.5*(sqr(tr(gradU)) - tr(((gradU) & (gradU))));
    }
}


void CML::Q::end()
{
    if (active_)
    {
        execute();
    }
}


void CML::Q::timeSet()
{
    // Do nothing
}


void CML::Q::write()
{
    if (active_)
    {
        const volScalarField& Q =
            obr_.lookupObject<volScalarField>(type());

        Info<< type() << " " << name_ << " output:" << nl
            << "    writing field " << Q.name() << nl
            << endl;

        Q.write();
    }
}


// ************************************************************************* //
