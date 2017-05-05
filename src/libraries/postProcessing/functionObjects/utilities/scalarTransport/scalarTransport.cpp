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
#include "dictionary.hpp"
#include "fixedValueFvPatchFields.hpp"
#include "zeroGradientFvPatchFields.hpp"
#include "fvScalarMatrix.hpp"
#include "fvmDdt.hpp"
#include "fvmDiv.hpp"
#include "fvcDiv.hpp"
#include "fvmLaplacian.hpp"
#include "fvmSup.hpp"
#include "incompressible/turbulenceModel/turbulenceModel.hpp"
#include "compressible/turbulenceModel/turbulenceModel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
defineTypeNameAndDebug(scalarTransport, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::wordList CML::scalarTransport::boundaryTypes() const
{
    const volVectorField& U = mesh_.lookupObject<volVectorField>(UName_);

    wordList bTypes(U.boundaryField().size());

    forAll(bTypes, patchI)
    {
        const fvPatchField<vector>& pf = U.boundaryField()[patchI];
        if (isA<fixedValueFvPatchVectorField>(pf))
        {
            bTypes[patchI] = fixedValueFvPatchScalarField::typeName;
        }
        else
        {
            bTypes[patchI] = zeroGradientFvPatchScalarField::typeName;
        }
    }

    return bTypes;
}


CML::tmp<CML::volScalarField> CML::scalarTransport::DT
(
    const surfaceScalarField& phi
) const
{
    typedef incompressible::turbulenceModel icoModel;
    typedef compressible::turbulenceModel cmpModel;

    if (userDT_)
    {
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (
                    "DT",
                    mesh_.time().timeName(),
                    mesh_.time(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh_,
                dimensionedScalar("DT", phi.dimensions()/dimLength, DT_)
            )
        );
    }
    else if (mesh_.foundObject<icoModel>("turbulenceModel"))
    {
        const icoModel& model = mesh_.lookupObject<icoModel>("turbulenceModel");

        return model.nuEff();
    }
    else if (mesh_.foundObject<cmpModel>("turbulenceModel"))
    {
        const cmpModel& model = mesh_.lookupObject<cmpModel>("turbulenceModel");

        return model.muEff();
    }
    else
    {
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (
                    "DT",
                    mesh_.time().timeName(),
                    mesh_.time(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh_,
                dimensionedScalar("DT", phi.dimensions()/dimLength, 0.0)
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
    name_(name),
    mesh_(refCast<const fvMesh>(obr)),
    active_(true),
    phiName_("phi"),
    UName_("U"),
    rhoName_("rho"),
    DT_(0.0),
    userDT_(false),
    resetOnStartUp_(false),
    nCorr_(0),
    autoSchemes_(false),
    fvOptions_(mesh_),
    T_
    (
        IOobject
        (
            name,
            mesh_.time().timeName(),
            mesh_,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("zero", dimless, 0.0),
        boundaryTypes()
    )
{
    read(dict);

    if (resetOnStartUp_)
    {
        T_ == dimensionedScalar("zero", dimless, 0.0);
    }
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
        UName_ = dict.lookupOrDefault<word>("UName", "U");
        rhoName_ = dict.lookupOrDefault<word>("rhoName", "rho");

        userDT_ = false;
        if (dict.readIfPresent("DT", DT_))
        {
            userDT_ = true;
        }

        dict.lookup("resetOnStartUp") >> resetOnStartUp_;

        dict.readIfPresent("nCorr", nCorr_);

        dict.lookup("autoSchemes") >> autoSchemes_;

        fvOptions_.reset(dict.subDict("fvOptions"));
    }
}


void CML::scalarTransport::execute()
{
    if (active_)
    {
        Info<< type() << " output:" << endl;

        const surfaceScalarField& phi =
            mesh_.lookupObject<surfaceScalarField>(phiName_);

        // calculate the diffusivity
        volScalarField DT(this->DT(phi));

        // set schemes
        word schemeVar = T_.name();
        if (autoSchemes_)
        {
            schemeVar = UName_;
        }

        word divScheme("div(phi," + schemeVar + ")");
        word laplacianScheme("laplacian(" + DT.name() + "," + schemeVar + ")");

        // set under-relaxation coeff
        scalar relaxCoeff = 0.0;
        if (mesh_.relaxEquation(schemeVar))
        {
            relaxCoeff = mesh_.equationRelaxationFactor(schemeVar);
        }

        if (phi.dimensions() == dimMass/dimTime)
        {
            const volScalarField& rho =
                mesh_.lookupObject<volScalarField>(rhoName_);

            // solve
            for (label i = 0; i <= nCorr_; i++)
            {
                fvScalarMatrix TEqn
                (
                    fvm::ddt(rho, T_)
                  + fvm::div(phi, T_, divScheme)
                  - fvm::laplacian(DT, T_, laplacianScheme)
                 ==
                    fvOptions_(rho, T_)
                );

                TEqn.relax(relaxCoeff);

                fvOptions_.constrain(TEqn);

                TEqn.solve(mesh_.solverDict(schemeVar));
            }
        }
        else if (phi.dimensions() == dimVolume/dimTime)
        {
            // solve
            for (label i = 0; i <= nCorr_; i++)
            {
                fvScalarMatrix TEqn
                (
                    fvm::ddt(T_)
                  + fvm::div(phi, T_, divScheme)
                  - fvm::laplacian(DT, T_, laplacianScheme)
                 ==
                    fvOptions_(T_)
                );

                TEqn.relax(relaxCoeff);

                fvOptions_.constrain(TEqn);

                TEqn.solve(mesh_.solverDict(schemeVar));
            }
        }
        else
        {
            FatalErrorIn("void CML::scalarTransport::execute()")
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
