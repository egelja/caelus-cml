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

#include "yPlusRAS.hpp"
#include "volFields.hpp"

#include "incompressible/RAS/RASModel/RASModel.hpp"
#include "nutWF/nutWFFvPatchScalarField.hpp"
#include "compressible/RAS/RASModel/compressibleRASModel.hpp"
#include "mutkWF/mutkWFFvPatchScalarField.hpp"
#include "wallDist.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(yPlusRAS, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::yPlusRAS::writeFileHeader(const label i)
{
    writeHeader(file(), "y+ (RAS)");

    writeCommented(file(), "Time");
    writeTabbed(file(), "patch");
    writeTabbed(file(), "min");
    writeTabbed(file(), "max");
    writeTabbed(file(), "average");
    file() << endl;
}


void CML::yPlusRAS::calcIncompressibleYPlus
(
    const fvMesh& mesh,
    volScalarField& yPlus
)
{
    typedef incompressible::nutWallFunctionFvPatchScalarField
        wallFunctionPatchField;

    const incompressible::RASModel& model =
        mesh.lookupObject<incompressible::RASModel>("RASProperties");

    const volScalarField nut(model.nut());
    const volScalarField::GeometricBoundaryField& nutPatches =
        nut.boundaryField();

    bool foundPatch = false;
    forAll(nutPatches, patchi)
    {
        if (isA<wallFunctionPatchField>(nutPatches[patchi]))
        {
            foundPatch = true;

            const wallFunctionPatchField& nutPw =
                dynamic_cast<const wallFunctionPatchField&>(nutPatches[patchi]);

            yPlus.boundaryField()[patchi] = nutPw.yPlus();
            const scalarField& Yp = yPlus.boundaryField()[patchi];

            scalar minYp = gMin(Yp);
            scalar maxYp = gMax(Yp);
            scalar avgYp = gAverage(Yp);

            if (Pstream::master())
            {
                Info(log_)<< "    patch " << nutPw.patch().name()
                    << " y+ : min = " << minYp << ", max = " << maxYp
                    << ", average = " << avgYp << nl;

                file() << obr_.time().value()
                    << token::TAB << nutPw.patch().name()
                    << token::TAB << minYp
                    << token::TAB << maxYp
                    << token::TAB << avgYp
                    << endl;
            }
        }
    }

    if (log_ && !foundPatch)
    {
        Info<< "    no " << wallFunctionPatchField::typeName << " patches"
            << endl;
    }
}


void CML::yPlusRAS::calcCompressibleYPlus
(
    const fvMesh& mesh,
    volScalarField& yPlus
)
{
    typedef compressible::mutkWallFunctionFvPatchScalarField
        wallFunctionPatchField;

    const compressible::RASModel& model =
        mesh.lookupObject<compressible::RASModel>("RASProperties");

    const volScalarField mut(model.mut());
    const volScalarField::GeometricBoundaryField& mutPatches =
        mut.boundaryField();

    bool foundPatch = false;
    forAll(mutPatches, patchi)
    {
        if (isA<wallFunctionPatchField>(mutPatches[patchi]))
        {
            foundPatch = true;

            const wallFunctionPatchField& mutPw =
                dynamic_cast<const wallFunctionPatchField&>(mutPatches[patchi]);

            yPlus.boundaryField()[patchi] = mutPw.yPlus();
            const scalarField& Yp = yPlus.boundaryField()[patchi];

            scalar minYp = gMin(Yp);
            scalar maxYp = gMax(Yp);
            scalar avgYp = gAverage(Yp);

            if (Pstream::master())
            {
                Info(log_)<< "    patch " << mutPw.patch().name()
                    << " y+ : min = " << minYp << ", max = " << maxYp
                    << ", average = " << avgYp << nl;

                file() << obr_.time().value()
                    << token::TAB << mutPw.patch().name()
                    << token::TAB << minYp
                    << token::TAB << maxYp
                    << token::TAB << avgYp
                    << endl;
            }
        }
    }

    if (log_ && !foundPatch)
    {
        Info<< "    no " << wallFunctionPatchField::typeName << " patches"
            << endl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::yPlusRAS::yPlusRAS
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
    log_(true),
    phiName_("phi")
{
    // Check if the available mesh is an fvMesh, otherwise deactivate
    if (!isA<fvMesh>(obr_))
    {
        active_ = false;
        WarningInFunction
            << "No fvMesh available, deactivating " << name_ << nl
            << endl;
    }

    if (active_)
    {
        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        volScalarField* yPlusRASPtr
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
                dimensionedScalar("0", dimless, 0.0)
            )
        );

        mesh.objectRegistry::store(yPlusRASPtr);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::yPlusRAS::~yPlusRAS()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::yPlusRAS::read(const dictionary& dict)
{
    if (active_)
    {
        log_ = dict.lookupOrDefault<Switch>("log", true);
        phiName_ = dict.lookupOrDefault<word>("phiName", "phi");
    }
}


void CML::yPlusRAS::execute()
{
    if (active_)
    {
        functionObjectFile::write();

        const surfaceScalarField& phi =
            obr_.lookupObject<surfaceScalarField>(phiName_);

        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        volScalarField& yPlusRAS =
            const_cast<volScalarField&>
            (
                mesh.lookupObject<volScalarField>(type())
            );

        Info(log_)<< type() << " " << name_ << " output:" << nl;

        if (phi.dimensions() == dimMass/dimTime)
        {
            calcCompressibleYPlus(mesh, yPlusRAS);
        }
        else
        {
            calcIncompressibleYPlus(mesh, yPlusRAS);
        }
    }
}


void CML::yPlusRAS::end()
{
    if (active_)
    {
        execute();
    }
}


void CML::yPlusRAS::timeSet()
{
    // Do nothing
}


void CML::yPlusRAS::write()
{
    if (active_)
    {
        functionObjectFile::write();

        const volScalarField& yPlusRAS =
            obr_.lookupObject<volScalarField>(type());

        Info(log_)<< "    writing field " << yPlusRAS.name() << nl << endl;

        yPlusRAS.write();
    }
}


// ************************************************************************* //
