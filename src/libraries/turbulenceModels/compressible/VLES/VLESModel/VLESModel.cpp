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
void compressibleVLESModelsLoad()
{}
#endif

namespace CML
{
namespace compressible
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
    const word& type,
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const basicThermo& thermoPhysicalModel,
    const word& turbulenceModelName
) : 
    turbulenceModel(rho, U, phi, thermoPhysicalModel, turbulenceModelName),
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
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const basicThermo& thermoPhysicalModel,
    const word& turbulenceModelName
)
{
    // get model name, but do not register the dictionary
    // otherwise it is registered in the database twice
    const word modelType
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
        FatalErrorIn
        (
            "VLESModel::New"
            "("
                "volScalarField const&,"
                "volVectorField const&, "
                "surfaceScalarField const&, "
                "const basicThermo&, "
                "word const&"
            ")"
        )   << "Unknown VLESModel type "
            << modelType << nl << nl
            << "Valid VLESModel types:" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<VLESModel>
    (
        cstrIter()(rho, U, phi, thermoPhysicalModel, turbulenceModelName)
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

