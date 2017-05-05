/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

#include "constSolidThermo.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(constSolidThermo, 0);
    addToRunTimeSelectionTable(basicSolidThermo, constSolidThermo, mesh);
    addToRunTimeSelectionTable(basicSolidThermo, constSolidThermo, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::constSolidThermo::constSolidThermo
(
    const fvMesh& mesh,
    const dictionary& dict
)
:
    basicSolidThermo(mesh, dict),
    dict_(dict.subDict(typeName + "Coeffs")),
    constK_(dimensionedScalar(dict_.lookup("K"))),
    K_
    (
        IOobject
        (
            "K",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        constK_
    ),
    constRho_(dimensionedScalar(dict_.lookup("rho"))),
    constCp_(dimensionedScalar(dict_.lookup("Cp"))),
    constHf_(dimensionedScalar(dict_.lookup("Hf"))),
    constEmissivity_(dimensionedScalar(dict_.lookup("emissivity"))),
    constKappa_(dimensionedScalar(dict_.lookup("kappa"))),
    constSigmaS_(dimensionedScalar(dict_.lookup("sigmaS")))
{
    read();

    K_ = constK_;

    rho_ = constRho_;

    emissivity_ = constEmissivity_;

    kappa_ = constKappa_;

    sigmaS_ = constSigmaS_;
}


CML::constSolidThermo::constSolidThermo(const fvMesh& mesh)
:
    basicSolidThermo(mesh),
    dict_(subDict(typeName + "Coeffs")),
    constK_(dimensionedScalar(dict_.lookup("K"))),
    K_
    (
        IOobject
        (
            "K",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        constK_
    ),
    constRho_(dimensionedScalar(dict_.lookup("rho"))),
    constCp_(dimensionedScalar(dict_.lookup("Cp"))),
    constHf_(dimensionedScalar(dict_.lookup("Hf"))),
    constEmissivity_(dimensionedScalar(dict_.lookup("emissivity"))),
    constKappa_(dimensionedScalar(dict_.lookup("kappa"))),
    constSigmaS_(dimensionedScalar(dict_.lookup("sigmaS")))
{
    read();

    K_ = constK_;

    rho_ = constRho_;

    emissivity_ = constEmissivity_;

    kappa_ = constKappa_;

    sigmaS_ = constSigmaS_;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::constSolidThermo::~constSolidThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::constSolidThermo::correct()
{}


CML::tmp<CML::volScalarField> CML::constSolidThermo::K() const
{
    return K_;
}


CML::tmp<CML::volSymmTensorField> CML::constSolidThermo::directionalK() const
{
    dimensionedSymmTensor t
    (
        constK_.name(),
        constK_.dimensions(),
        symmTensor
        (
            constK_.value(),
            0.0,
            0.0,
            constK_.value(),
            0.0,
            constK_.value()
        )
    );
    return tmp<volSymmTensorField>
    (
        new volSymmTensorField
        (
            IOobject
            (
                "K",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            t
        )
    );
}


CML::tmp<CML::volScalarField> CML::constSolidThermo::Cp() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "Cp",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            constCp_
        )
    );
}


CML::tmp<CML::volScalarField> CML::constSolidThermo::Hf() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "Hf",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            constHf_
        )
    );
}


CML::tmp<CML::scalarField> CML::constSolidThermo::K
(
    const label patchI
) const
{
    return (K_.boundaryField()[patchI]);
}


CML::tmp<CML::symmTensorField> CML::constSolidThermo::directionalK
(
    const label patchI
) const
{
    symmTensor t
    (
        constK_.value(),
        0.0,
        0.0,
        constK_.value(),
        0.0,
        constK_.value()
    );
    return tmp<symmTensorField>
    (
        new symmTensorField
        (
            T_.boundaryField()[patchI].size(),
            t
        )
    );
}


CML::tmp<CML::scalarField> CML::constSolidThermo::Cp
(
    const label patchI
) const
{
    return tmp<scalarField>
    (
        new scalarField
        (
            T_.boundaryField()[patchI].size(),
            constCp_.value()
        )
    );
}


CML::tmp<CML::scalarField> CML::constSolidThermo::Hf
(
    const label patchI
) const
{
    return tmp<scalarField>
    (
        new scalarField
        (
            T_.boundaryField()[patchI].size(),
            constHf_.value()
        )
    );
}


bool CML::constSolidThermo::read()
{
    return read(subDict(typeName + "Coeffs"));
}


bool CML::constSolidThermo::read(const dictionary& dict)
{
    constRho_ = dimensionedScalar(dict.lookup("rho"));
    constCp_ = dimensionedScalar(dict.lookup("Cp"));
    constK_ = dimensionedScalar(dict.lookup("K"));
    constHf_ = dimensionedScalar(dict.lookup("Hf"));
    constEmissivity_ = dimensionedScalar(dict.lookup("emissivity"));
    constKappa_ = dimensionedScalar(dict_.lookup("kappa"));
    constSigmaS_ = dimensionedScalar(dict_.lookup("sigmaS"));

    Info<< "Constructed constSolidThermo with" << nl
        << "    rho        : " << constRho_ << nl
        << "    Cp         : " << constCp_ << nl
        << "    K          : " << constK_ << nl
        << "    Hf         : " << constHf_ << nl
        << "    emissivity : " << constEmissivity_ << nl
        << "    kappa      : " << constKappa_ << nl
        << "    sigmaS     : " << constSigmaS_ << nl
        << endl;

    return true;
}


bool CML::constSolidThermo::writeData(Ostream& os) const
{
    bool ok = basicSolidThermo::writeData(os);
    os.writeKeyword("rho") << constRho_ << token::END_STATEMENT << nl;
    os.writeKeyword("Cp") << constCp_ << token::END_STATEMENT << nl;
    os.writeKeyword("K") << constK_ << token::END_STATEMENT << nl;
    os.writeKeyword("Hf") << constHf_ << token::END_STATEMENT << nl;
    os.writeKeyword("kappa") << constKappa_ << token::END_STATEMENT << nl;
    os.writeKeyword("sigmaS") << constSigmaS_ << token::END_STATEMENT << nl;
    os.writeKeyword("emissivity") << constEmissivity_ << token::END_STATEMENT
        << nl;
    return ok && os.good();
}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const constSolidThermo& s)
{
    s.writeData(os);
    return os;
}


// ************************************************************************* //
