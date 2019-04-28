/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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

#include "VLESModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

#ifdef windows
extern "C"
void incompressibleVLESModelsLoad()
{}
#endif

namespace CML
{
namespace incompressible
{

defineTypeNameAndDebug(VLESModel, 0);
defineRunTimeSelectionTable(VLESModel, dictionary);
addToRunTimeSelectionTable(turbulenceModel, VLESModel, turbulenceModel);

void VLESModel::printCoeffs()
{
    if (printCoeffs_)
    {
        Info<< type() << "Coeffs" << coeffDict_ << endl;
    }
}

VLESModel::VLESModel
(
    word const& type,
    volVectorField const& U,
    surfaceScalarField const& phi,
    transportModel& transport,
    word const& turbulenceModelName
) : 
    turbulenceModel(U, phi, transport, turbulenceModelName),
    IOdictionary
    (
        IOobject
        (
            "VLESProperties",
            U.time().constant(),
            U.db(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    turbulence_(lookup("turbulence")),
    printCoeffs_(lookupOrDefault<Switch>("printCoeffs", false)),
    coeffDict_(subOrEmptyDict(type + "Coeffs")),
    kMin_("kMin", sqr(dimVelocity), SMALL),
    epsilonMin_("epsilonMin", kMin_.dimensions()/dimTime, SMALL),
    omegaMin_("omegaMin", dimless/dimTime, SMALL)
{
    kMin_.readIfPresent(*this);
    epsilonMin_.readIfPresent(*this);
    omegaMin_.readIfPresent(*this);
    mesh_.deltaCoeffs();
}

autoPtr<VLESModel> VLESModel::New
(
    volVectorField const& U,
    surfaceScalarField const& phi,
    transportModel& transport,
    word const& turbulenceModelName
)
{
    // get model name, but do not register the dictionary
    // otherwise it is registered in the database twice
    word const modelType
    (
        IOdictionary
        (
            IOobject
            (
                "VLESProperties",
                U.time().constant(),
                U.db(),
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE,
                false
            )
        ).lookup("VLESModel")
    );

    Info<< "Selecting VLES turbulence model " << modelType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown VLESModel type "
            << modelType << nl << nl
            << "Valid VLESModel types:" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<VLESModel>
    (
        cstrIter()(U, phi, transport, turbulenceModelName)
    );
}


tmp<volSymmTensorField> VLESModel::R() const
{
    return tmp<volSymmTensorField>
    (
        new volSymmTensorField
        (
            IOobject
            (
                "R",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            ((2.0/3.0)*I)*k() - nut()*twoSymm(fvc::grad(U_)),
            k()().boundaryField().types()
        )
    )*Fr();
}


tmp<volSymmTensorField> VLESModel::devReff() const
{
    return tmp<volSymmTensorField>
    (
        new volSymmTensorField
        (
            IOobject
            (
                "devRhoReff",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
           -(nu() + nut()*Fr())*dev(twoSymm(fvc::grad(U_)))
        )
    );
}


tmp<fvVectorMatrix> VLESModel::divDevReff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(nu(), U)
      - fvc::div(nu()*dev(T(fvc::grad(U))))
      - Fr()*(fvm::laplacian(nut(), U) + fvc::div(nut()*dev(T(fvc::grad(U)))))
    );
}


tmp<fvVectorMatrix> VLESModel::divDevRhoReff
(
    volScalarField const& rho,
    volVectorField& U
) const
{
    volScalarField mu("mu", rho*nu());
    volScalarField mut("mut", rho*nut());

    return
    (
      - fvm::laplacian(mu, U)
      - fvc::div(mu*dev(T(fvc::grad(U))))
      - Fr()*fvm::laplacian(mut, U)
      - Fr()*fvc::div(mut*dev(T(fvc::grad(U))))
    );
}


void VLESModel::correct()
{
    turbulenceModel::correct();
}


bool VLESModel::read()
{
    bool ok = IOdictionary::readData
    (
        IOdictionary::readStream
        (
            IOdictionary::type()
        )
    );

    IOdictionary::close();

    if (ok)
    {
        lookup("turbulence") >> turbulence_;

        if (dictionary const* dictPtr = subDictPtr(type() + "Coeffs"))
        {
            coeffDict_ <<= *dictPtr;
        }

        kMin_.readIfPresent(*this);
        epsilonMin_.readIfPresent(*this);
        omegaMin_.readIfPresent(*this);

        return true;
    }
    else
    {
        return false;
    }
}

}
}

