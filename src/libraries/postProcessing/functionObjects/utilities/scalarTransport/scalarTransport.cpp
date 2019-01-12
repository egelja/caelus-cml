/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2014 OpenFOAM Foundation
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

#include "scalarTransport.hpp"
#include "surfaceFields.hpp"
#include "fvmDdt.hpp"
#include "fvmDiv.hpp"
#include "fvmLaplacian.hpp"
#include "fvmSup.hpp"
#include "incompressible/turbulenceModel/turbulenceModel.hpp"
#include "compressible/turbulenceModel/compressibleTurbulenceModel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(scalarTransport, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::tmp<CML::volScalarField> CML::scalarTransport::D
(
    const surfaceScalarField& phi
) const
{
    typedef incompressible::turbulenceModel icoModel;
    typedef compressible::turbulenceModel cmpModel;

    word Dname("D" + s_.name());

    if (constantD_)
    {
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (
                    Dname,
                    mesh_.time().timeName(),
                    mesh_.time(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh_,
                dimensionedScalar(Dname, phi.dimensions()/dimLength, D_)
            )
        );
    }
    else if (mesh_.foundObject<icoModel>("turbulenceModel"))
    {
        const icoModel& model = mesh_.lookupObject<icoModel>("turbulenceModel");

        return alphaD_*model.nu() + alphaDt_*model.nut();
    }
    else if (mesh_.foundObject<cmpModel>("turbulenceModel"))
    {
        const cmpModel& model = mesh_.lookupObject<cmpModel>("turbulenceModel");

        return alphaD_*model.mu() + alphaDt_*model.mut();
    }
    else
    {
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (
                    Dname,
                    mesh_.time().timeName(),
                    mesh_.time(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh_,
                dimensionedScalar(Dname, phi.dimensions()/dimLength, 0.0)
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::scalarTransport::scalarTransport
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    fieldName_(dict.lookupOrDefault<word>("field", "s")),
    mesh_(refCast<const fvMesh>(obr)),
    active_(true),
    D_(0),
    nCorr_(0),
    fvOptions_(mesh_),
    s_
    (
        IOobject
        (
            fieldName_,
            mesh_.time().timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    )
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::scalarTransport::~scalarTransport()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::scalarTransport::read(const dictionary& dict)
{
    if (active_)
    {
        Info<< type() << ":" << nl;

        phiName_ = dict.lookupOrDefault<word>("phiName", "phi");
        rhoName_ = dict.lookupOrDefault<word>("rhoName", "rho");
        schemesField_ = dict.lookupOrDefault<word>("schemesField", fieldName_);

        constantD_ = dict.readIfPresent("D", D_);
        alphaD_ = dict.lookupOrDefault("alphaD", 1.0);
        alphaDt_ = dict.lookupOrDefault("alphaDt", 1.0);

        dict.readIfPresent("nCorr", nCorr_);

        if (dict.found("fvOptions"))
        {
            fvOptions_.reset(dict.subDict("fvOptions"));
        }
    }
}


void CML::scalarTransport::execute()
{
    if (active_)
    {
        Info<< type() << " write:" << endl;

        const surfaceScalarField& phi =
            mesh_.lookupObject<surfaceScalarField>(phiName_);

        // Calculate the diffusivity
        volScalarField D(this->D(phi));

        word divScheme("div(phi," + schemesField_ + ")");
        word laplacianScheme("laplacian(" + D.name() + "," + schemesField_ + ")");

        // Set under-relaxation coeff
        scalar relaxCoeff = 0.0;
        if (mesh_.relaxEquation(schemesField_))
        {
            relaxCoeff = mesh_.equationRelaxationFactor(schemesField_);
        }

        if (phi.dimensions() == dimMass/dimTime)
        {
            const volScalarField& rho =
                mesh_.lookupObject<volScalarField>(rhoName_);

            for (label i = 0; i <= nCorr_; i++)
            {
                fvScalarMatrix sEqn
                (
                    fvm::ddt(rho, s_)
                  + fvm::div(phi, s_, divScheme)
                  - fvm::laplacian(D, s_, laplacianScheme)
                 ==
                    fvOptions_(rho, s_)
                );

                sEqn.relax(relaxCoeff);

                fvOptions_.constrain(sEqn);

                sEqn.solve(mesh_.solverDict(schemesField_));
            }
        }
        else if (phi.dimensions() == dimVolume/dimTime)
        {
            for (label i = 0; i <= nCorr_; i++)
            {
                fvScalarMatrix sEqn
                (
                    fvm::ddt(s_)
                  + fvm::div(phi, s_, divScheme)
                  - fvm::laplacian(D, s_, laplacianScheme)
                 ==
                    fvOptions_(s_)
                );

                sEqn.relax(relaxCoeff);

                fvOptions_.constrain(sEqn);

                sEqn.solve(mesh_.solverDict(schemesField_));
            }
        }
        else
        {
            FatalErrorInFunction
                << "Incompatible dimensions for phi: " << phi.dimensions() << nl
                << "Dimensions should be " << dimMass/dimTime << " or "
                << dimVolume/dimTime << endl;
    }

    Info<< endl;
    }
}


void CML::scalarTransport::end()
{
    if (active_)
    {
        execute();
    }
}


void CML::scalarTransport::timeSet()
{
    // Do nothing
}


void CML::scalarTransport::write()
{
    // Do nothing
}


// ************************************************************************* //
