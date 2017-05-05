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

#include "radiationModel.hpp"
#include "absorptionEmissionModel.hpp"
#include "scatterModel.hpp"
#include "fvmSup.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace radiation
    {
        defineTypeNameAndDebug(radiationModel, 0);
        defineRunTimeSelectionTable(radiationModel, dictionary);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::radiation::radiationModel::radiationModel(const volScalarField& T)
:
    IOdictionary
    (
        IOobject
        (
            "radiationProperties",
            T.time().constant(),
            T.mesh(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    mesh_(T.mesh()),
    time_(T.time()),
    T_(T),
    radiation_(false),
    coeffs_(dictionary::null),
    solverFreq_(0),
    firstIter_(true),
    absorptionEmission_(NULL),
    scatter_(NULL)
{}


CML::radiation::radiationModel::radiationModel
(
    const word& type,
    const volScalarField& T
)
:
    IOdictionary
    (
        IOobject
        (
            "radiationProperties",
            T.time().constant(),
            T.mesh(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    mesh_(T.mesh()),
    time_(T.time()),
    T_(T),
    radiation_(lookup("radiation")),
    coeffs_(subDict(type + "Coeffs")),
    solverFreq_(readLabel(lookup("solverFreq"))),
    firstIter_(true),
    absorptionEmission_(absorptionEmissionModel::New(*this, mesh_)),
    scatter_(scatterModel::New(*this, mesh_))
{
    solverFreq_ = max(1, solverFreq_);
}


// * * * * * * * * * * * * * * * * Destructor    * * * * * * * * * * * * * * //

CML::radiation::radiationModel::~radiationModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::radiation::radiationModel::read()
{
    if (regIOobject::read())
    {
        lookup("radiation") >> radiation_;
        coeffs_ = subDict(type() + "Coeffs");

        lookup("solverFreq") >> solverFreq_,
        solverFreq_ = max(1, solverFreq_);

        return true;
    }
    else
    {
        return false;
    }
}


void CML::radiation::radiationModel::correct()
{
    if (!radiation_)
    {
        return;
    }

    if (firstIter_ || (time_.timeIndex() % solverFreq_ == 0))
    {
        calculate();
        firstIter_ = false;
    }
}


CML::tmp<CML::fvScalarMatrix> CML::radiation::radiationModel::Sh
(
    basicThermo& thermo
) const
{
    volScalarField& h = thermo.h();
    const volScalarField Cp(thermo.Cp());
    const volScalarField T3(pow3(T_));

    return
    (
        Ru()
      - fvm::Sp(4.0*Rp()*T3/Cp, h)
      - Rp()*T3*(T_ - 4.0*h/Cp)
    );
}


CML::tmp<CML::fvScalarMatrix> CML::radiation::radiationModel::Shs
(
    basicThermo& thermo
) const
{
    volScalarField& hs = thermo.hs();
    const volScalarField Cp(thermo.Cp());
    const volScalarField T3(pow3(T_));

    return
    (
        Ru()
      - fvm::Sp(4.0*Rp()*T3/Cp, hs)
      - Rp()*T3*(T_ - 4.0*hs/Cp)
    );
}


// ************************************************************************* //
