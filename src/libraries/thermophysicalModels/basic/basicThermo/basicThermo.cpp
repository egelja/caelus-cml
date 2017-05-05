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

#include "basicThermo.hpp"
#include "fvMesh.hpp"
#include "HashTable.hpp"
#include "zeroGradientFvPatchFields.hpp"
#include "fixedEnthalpyFvPatchScalarField.hpp"
#include "gradientEnthalpyFvPatchScalarField.hpp"
#include "mixedEnthalpyFvPatchScalarField.hpp"
#include "fixedInternalEnergyFvPatchScalarField.hpp"
#include "gradientInternalEnergyFvPatchScalarField.hpp"
#include "mixedInternalEnergyFvPatchScalarField.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace CML
{
    defineTypeNameAndDebug(basicThermo, 0);
    defineRunTimeSelectionTable(basicThermo, fvMesh);
}

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

CML::wordList CML::basicThermo::hBoundaryTypes()
{
    const volScalarField::GeometricBoundaryField& tbf = T_.boundaryField();

    wordList hbt = tbf.types();

    forAll(tbf, patchi)
    {
        if (isA<fixedValueFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = fixedEnthalpyFvPatchScalarField::typeName;
        }
        else if
        (
            isA<zeroGradientFvPatchScalarField>(tbf[patchi])
         || isA<fixedGradientFvPatchScalarField>(tbf[patchi])
        )
        {
            hbt[patchi] = gradientEnthalpyFvPatchScalarField::typeName;
        }
        else if (isA<mixedFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = mixedEnthalpyFvPatchScalarField::typeName;
        }
    }

    return hbt;
}


void CML::basicThermo::hBoundaryCorrection(volScalarField& h)
{
    volScalarField::GeometricBoundaryField& hbf = h.boundaryField();

    forAll(hbf, patchi)
    {
        if (isA<gradientEnthalpyFvPatchScalarField>(hbf[patchi]))
        {
            refCast<gradientEnthalpyFvPatchScalarField>(hbf[patchi]).gradient()
                = hbf[patchi].fvPatchField::snGrad();
        }
        else if (isA<mixedEnthalpyFvPatchScalarField>(hbf[patchi]))
        {
            refCast<mixedEnthalpyFvPatchScalarField>(hbf[patchi]).refGrad()
                = hbf[patchi].fvPatchField::snGrad();
        }
    }
}


CML::wordList CML::basicThermo::eBoundaryTypes()
{
    const volScalarField::GeometricBoundaryField& tbf = T_.boundaryField();

    wordList ebt = tbf.types();

    forAll(tbf, patchi)
    {
        if (isA<fixedValueFvPatchScalarField>(tbf[patchi]))
        {
            ebt[patchi] = fixedInternalEnergyFvPatchScalarField::typeName;
        }
        else if
        (
            isA<zeroGradientFvPatchScalarField>(tbf[patchi])
         || isA<fixedGradientFvPatchScalarField>(tbf[patchi])
        )
        {
            ebt[patchi] = gradientInternalEnergyFvPatchScalarField::typeName;
        }
        else if (isA<mixedFvPatchScalarField>(tbf[patchi]))
        {
            ebt[patchi] = mixedInternalEnergyFvPatchScalarField::typeName;
        }
    }

    return ebt;
}


void CML::basicThermo::eBoundaryCorrection(volScalarField& e)
{
    volScalarField::GeometricBoundaryField& ebf = e.boundaryField();

    forAll(ebf, patchi)
    {
        if (isA<gradientInternalEnergyFvPatchScalarField>(ebf[patchi]))
        {
            refCast<gradientInternalEnergyFvPatchScalarField>(ebf[patchi])
                .gradient() = ebf[patchi].fvPatchField::snGrad();
        }
        else if (isA<mixedInternalEnergyFvPatchScalarField>(ebf[patchi]))
        {
            refCast<mixedInternalEnergyFvPatchScalarField>(ebf[patchi])
                .refGrad() = ebf[patchi].fvPatchField::snGrad();
        }
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::basicThermo::basicThermo(const fvMesh& mesh)
:
    IOdictionary
    (
        IOobject
        (
            "thermophysicalProperties",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),

    p_
    (
        IOobject
        (
            "p",
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),

    psi_
    (
        IOobject
        (
            "psi",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionSet(0, -2, 2, 0, 0)
    ),

    T_
    (
        IOobject
        (
            "T",
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),

    mu_
    (
        IOobject
        (
            "mu",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionSet(1, -1, -1, 0, 0)
    ),

    alpha_
    (
        IOobject
        (
            "alpha",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionSet(1, -1, -1, 0, 0)
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::basicThermo::~basicThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::volScalarField& CML::basicThermo::p()
{
    return p_;
}


const CML::volScalarField& CML::basicThermo::p() const
{
    return p_;
}


const CML::volScalarField& CML::basicThermo::psi() const
{
    return psi_;
}


CML::volScalarField& CML::basicThermo::h()
{
    notImplemented("basicThermo::h()");
    return const_cast<volScalarField&>(volScalarField::null());
}


const CML::volScalarField& CML::basicThermo::h() const
{
    notImplemented("basicThermo::h() const");
    return volScalarField::null();
}


CML::tmp<CML::scalarField> CML::basicThermo::h
(
    const scalarField& T,
    const labelList& cells
) const
{
    notImplemented
    (
        "basicThermo::h"
        "(const scalarField& T, const labelList& cells) const"
    );
    return tmp<scalarField>(NULL);
}


CML::tmp<CML::scalarField> CML::basicThermo::h
(
    const scalarField& T,
    const label patchi
) const
{
    notImplemented
    (
        "basicThermo::h"
        "(const scalarField& T, const label patchi) const"
    );
    return tmp<scalarField>(NULL);
}


CML::volScalarField& CML::basicThermo::hs()
{
    notImplemented("basicThermo::hs()");
    return const_cast<volScalarField&>(volScalarField::null());
}


const CML::volScalarField& CML::basicThermo::hs() const
{
    notImplemented("basicThermo::hs() const");
    return volScalarField::null();
}


CML::tmp<CML::scalarField> CML::basicThermo::hs
(
    const scalarField& T,
    const labelList& cells
) const
{
    notImplemented
    (
        "basicThermo::hs"
        "(const scalarField& T, const labelList& cells) const"
    );
    return tmp<scalarField>(NULL);
}


CML::tmp<CML::scalarField> CML::basicThermo::hs
(
    const scalarField& T,
    const label patchi
) const
{
    notImplemented
    (
        "basicThermo::hs"
        "(const scalarField& T, const label patchi) const"
    );
    return tmp<scalarField>(NULL);
}


CML::tmp<CML::volScalarField> CML::basicThermo::hc() const
{
    notImplemented("basicThermo::hc()");
    return volScalarField::null();
}


CML::volScalarField& CML::basicThermo::e()
{
    notImplemented("basicThermo::e()");
    return const_cast<volScalarField&>(volScalarField::null());
}


const CML::volScalarField& CML::basicThermo::e() const
{
    notImplemented("basicThermo::e()");
    return volScalarField::null();
}


CML::tmp<CML::scalarField> CML::basicThermo::e
(
    const scalarField& T,
    const labelList& cells
) const
{
    notImplemented
    (
        "basicThermo::e"
        "(const scalarField& T, const labelList& cells) const"
    );
    return tmp<scalarField>(NULL);
}


CML::tmp<CML::scalarField> CML::basicThermo::e
(
    const scalarField& T,
    const label patchi
) const
{
    notImplemented
    (
        "basicThermo::e"
        "(const scalarField& T, const label patchi) const"
    );
    return tmp<scalarField>(NULL);
}


const CML::volScalarField& CML::basicThermo::T() const
{
    return T_;
}


CML::tmp<CML::scalarField> CML::basicThermo::Cp
(
    const scalarField& T,
    const label patchi
) const
{
    notImplemented
    (
        "basicThermo::Cp"
        "(const scalarField& T, const label patchi) const"
    );
    return tmp<scalarField>(NULL);
}


CML::tmp<CML::volScalarField> CML::basicThermo::Cp() const
{
    notImplemented("basicThermo::Cp() const");
    return volScalarField::null();
}


CML::tmp<CML::scalarField> CML::basicThermo::Cv
(
    const scalarField& T,
    const label patchi
) const
{
    notImplemented
    (
        "basicThermo::Cv"
        "(const scalarField& T, const label patchi) const"
    );
    return tmp<scalarField>(NULL);
}


CML::tmp<CML::volScalarField> CML::basicThermo::Cv() const
{
    notImplemented("basicThermo::Cv() const");
    return volScalarField::null();
}


const CML::volScalarField& CML::basicThermo::mu() const
{
    return mu_;
}


const CML::volScalarField& CML::basicThermo::alpha() const
{
    return alpha_;
}


bool CML::basicThermo::read()
{
    return regIOobject::read();
}


// ************************************************************************* //
