/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "staticPressure.hpp"
#include "volFields.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::staticPressure, 0);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool CML::staticPressure::isKinematicPressure()
{
    const volScalarField& p = obr_.lookupObject<volScalarField>(pName_);

    return p.dimensions() == sqr(dimLength)/sqr(dimTime);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::staticPressure::staticPressure
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
    pName_(dict.lookupOrDefault<word>("p", "p")),
    rho_(readScalar(dict.lookup("rho")))
{
    // Check if the available mesh is an fvMesh, otherwise deactivate
    if (!isA<fvMesh>(obr_))
    {
        active_ = false;
        WarningIn
        (
            "staticPressure::staticPressure"
            "("
                "const word&, "
                "const objectRegistry&, "
                "const dictionary&, "
                "const bool"
            ")"
        )   << "No fvMesh available, deactivating." << nl
            << endl;
    }
    else
    {
        // Check if the pressure is kinematic pressure, otherwise deactivate
        if (!isKinematicPressure())
        {
            active_ = false;
            WarningIn
            (
                "staticPressure::staticPressure"
                "("
                    "const word&, "
                    "const objectRegistry&, "
                    "const dictionary&, "
                    "const bool"
                ")"
            )   << "Pressure is not kinematic pressure, deactivating." << nl
                << endl;
        }
    }

    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::staticPressure::~staticPressure()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::staticPressure::read(const dictionary& dict)
{
    if (active_)
    {
        dict.readIfPresent("p", pName_);
        dict.lookup("rho") >> rho_;
    }
}


void CML::staticPressure::execute()
{
    // Do nothing - only valid on write
}


void CML::staticPressure::end()
{
    // Do nothing - only valid on write
}


void CML::staticPressure::write()
{
    if (active_)
    {
        const volScalarField& p = obr_.lookupObject<volScalarField>(pName_);

        volScalarField pStatic
        (
            IOobject
            (
                "pStatic",
                obr_.time().timeName(),
                obr_,
                IOobject::NO_READ
            ),
            dimensionedScalar("rho", dimDensity, rho_)*p
        );

        pStatic.write();
    }
}


// ************************************************************************* //
