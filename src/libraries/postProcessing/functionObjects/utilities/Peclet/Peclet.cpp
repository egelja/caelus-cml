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

#include "Peclet.hpp"
#include "volFields.hpp"
#include "dictionary.hpp"
#include "surfaceFields.hpp"
#include "incompressible/turbulenceModel/turbulenceModel.hpp"
#include "compressible/turbulenceModel/compressibleTurbulenceModel.hpp"
#include "surfaceInterpolate.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(Peclet, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::Peclet::Peclet
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
    phiName_("phi"),
    rhoName_("rho")
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

        surfaceScalarField* PecletPtr
        (
            new surfaceScalarField
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
                dimensionedScalar("0", dimless, 0.0)
            )
        );

        mesh.objectRegistry::store(PecletPtr);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::Peclet::~Peclet()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::Peclet::read(const dictionary& dict)
{
    if (active_)
    {
        phiName_ = dict.lookupOrDefault<word>("phiName", "phi");
        rhoName_ = dict.lookupOrDefault<word>("rhoName", "rho");
    }
}


void CML::Peclet::execute()
{
    typedef compressible::turbulenceModel cmpTurbModel;
    typedef incompressible::turbulenceModel icoTurbModel;

    if (active_)
    {
        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        tmp<volScalarField> nuEff;
        if (mesh.foundObject<cmpTurbModel>("turbulenceModel"))
        {
            const cmpTurbModel& model =
                mesh.lookupObject<cmpTurbModel>("turbulenceModel");

            const volScalarField& rho =
                mesh.lookupObject<volScalarField>(rhoName_);

            nuEff = model.muEff()/rho;
        }
        else if (mesh.foundObject<icoTurbModel>("turbulenceModel"))
        {
            const icoTurbModel& model =
                mesh.lookupObject<icoTurbModel>("turbulenceModel");

            nuEff = model.nuEff();
        }
        else if (mesh.foundObject<dictionary>("transportProperties"))
        {
            const dictionary& model =
                mesh.lookupObject<dictionary>("transportProperties");

            nuEff =
                tmp<volScalarField>
                (
                    new volScalarField
                    (
                        IOobject
                        (
                            "nuEff",
                            mesh.time().timeName(),
                            mesh,
                            IOobject::NO_READ,
                            IOobject::NO_WRITE
                        ),
                        mesh,
                        dimensionedScalar(model.lookup("nu"))
                    )
                );
        }
        else
        {
            FatalErrorInFunction
                << "Unable to determine the viscosity"
                << exit(FatalError);
        }

        const surfaceScalarField& phi =
            mesh.lookupObject<surfaceScalarField>(phiName_);

        surfaceScalarField& Peclet =
            const_cast<surfaceScalarField&>
            (
                mesh.lookupObject<surfaceScalarField>(type())
            );

        Peclet =
            mag(phi)
           /(
                mesh.magSf()
               *mesh.surfaceInterpolation::deltaCoeffs()
               *fvc::interpolate(nuEff)
            );
    }
}


void CML::Peclet::end()
{
    if (active_)
    {
        execute();
    }
}

void CML::Peclet::timeSet()
{
    // Do nothing
}


void CML::Peclet::write()
{
    if (active_)
    {
        const surfaceScalarField& Peclet =
            obr_.lookupObject<surfaceScalarField>(type());

        Info<< type() << " " << name_ << " output:" << nl
            << "    writing field " << Peclet.name() << nl
            << endl;

        Peclet.write();
    }
}


// ************************************************************************* //
