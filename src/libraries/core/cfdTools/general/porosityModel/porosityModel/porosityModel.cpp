/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

#include "porosityModel.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(porosityModel, 0);
    defineRunTimeSelectionTable(porosityModel, mesh);
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void CML::porosityModel::adjustNegativeResistance(dimensionedVector& resist)
{
    scalar maxCmpt = max(0, cmptMax(resist.value()));

    if (maxCmpt < 0)
    {
        FatalErrorIn
        (
            "void CML::porosityModel::adjustNegativeResistance"
            "("
                "dimensionedVector&"
            ")"
        )   << "Negative resistances are invalid, resistance = " << resist
            << exit(FatalError);
    }
    else
    {
        vector& val = resist.value();
        for (label cmpt = 0; cmpt < vector::nComponents; cmpt++)
        {
            if (val[cmpt] < 0)
            {
                val[cmpt] *= -maxCmpt;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::porosityModel::porosityModel
(
    const word& name,
    const word& modelType,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& cellZoneName
)
:
    regIOobject
    (
        IOobject
        (
            name,
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        )
    ),
    name_(name),
    mesh_(mesh),
    dict_(dict),
    coeffs_(dict.subDict(modelType + "Coeffs")),
    active_(true),
    zoneName_(cellZoneName),
    cellZoneIds_()
{
    if (zoneName_ == word::null)
    {
        dict.lookup("active") >> active_;
        dict_.lookup("cellZone") >> zoneName_;
    }

    cellZoneIds_ = mesh_.cellZones().findIndices(zoneName_);

    Info<< "    creating porous zone: " << zoneName_ << endl;

    bool foundZone = !cellZoneIds_.empty();
    reduce(foundZone, orOp<bool>());

    if (!foundZone && Pstream::master())
    {
        FatalErrorIn
        (
            "CML::porosityModel::porosityModel"
            "("
                "const word&, "
                "const word&, "
                "const fvMesh&, "
                "const dictionary&"
                "const word&, "
            ")"
        )   << "cannot find porous cellZone " << zoneName_
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::porosityModel::~porosityModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::vectorField> CML::porosityModel::force
(
    const volVectorField& U,
    const volScalarField& rho,
    const volScalarField& mu
) const
{
    tmp<vectorField> tforce(new vectorField(U.size(), vector::zero));

    if (!cellZoneIds_.empty())
    {
        this->calcForce(U, rho, mu, tforce());
    }

    return tforce;
}


void CML::porosityModel::addResistance
(
    fvVectorMatrix& UEqn
) const
{
    if (cellZoneIds_.empty())
    {
        return;
    }

    this->correct(UEqn);
}


void CML::porosityModel::addResistance
(
    fvVectorMatrix& UEqn,
    const volScalarField& rho,
    const volScalarField& mu
) const
{
    if (cellZoneIds_.empty())
    {
        return;
    }

    this->correct(UEqn, rho, mu);
}


void CML::porosityModel::addResistance
(
    const fvVectorMatrix& UEqn,
    volTensorField& AU,
    bool correctAUprocBC         
) const
{
    if (cellZoneIds_.empty())
    {
        return;
    }

    this->correct(UEqn, AU);

    if (correctAUprocBC)
    {
        // Correct the boundary conditions of the tensorial diagonal to ensure
        // processor boundaries are correctly handled when AU^-1 is interpolated
        // for the pressure equation.
        AU.correctBoundaryConditions();
    }
}


bool CML::porosityModel::movePoints()
{
    // no updates necessary; all member data independent of mesh
    return true;
}


void CML::porosityModel::updateMesh(const mapPolyMesh& mpm)
{
    // no updates necessary; all member data independent of mesh
}


bool CML::porosityModel::writeData(Ostream& os) const
{
    return true;
}


bool CML::porosityModel::read(const dictionary& dict)
{
    active_ = readBool(dict.lookup("active"));
    coeffs_ = dict.subDict(type() + "Coeffs");
    dict.lookup("cellZone") >> zoneName_;
    cellZoneIds_ = mesh_.cellZones().findIndices(zoneName_);

    return true;
}


// ************************************************************************* //
