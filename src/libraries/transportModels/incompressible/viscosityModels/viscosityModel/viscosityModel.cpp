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

#include "viscosityModel.hpp"
#include "fvcGrad.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(viscosityModel, 0);
    defineRunTimeSelectionTable(viscosityModel, dictionary);
}

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //


//- Print model coefficients
void CML::viscosityModel::printCoeffs()
{
    if (printCoeffs_)
    {
        const word modelType(viscosityProperties_.lookup("transportModel"));
        Info<< modelType << "Coeffs" << viscosityProperties_.subDict
            (modelType + "Coeffs") << endl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::viscosityModel::viscosityModel
(
    const word& name,
    const dictionary& viscosityProperties,
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    name_(name),
    viscosityProperties_(viscosityProperties),
    printCoeffs_(viscosityProperties_.lookupOrDefault<Switch>("printCoeffs", false)),
    outputShearStrainRate_(viscosityProperties_.lookupOrDefault<Switch>("outputShearStrainRate", false)),
    U_(U),
    phi_(phi),
    ssr_
    (
        IOobject
        (
            "ShearStrainRate",
            U.mesh().time().timeName(),
            U.mesh(),
            IOobject::READ_IF_PRESENT,
            outputShearStrainRate_ ? IOobject::AUTO_WRITE : IOobject::NO_WRITE 
        ),
        U.mesh(), 
        dimensionedScalar("ssrInit",dimensionSet(0, 0, -1, 0, 0, 0, 0), 0.0)
    )
{
    printCoeffs();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool CML::viscosityModel::read(const dictionary& viscosityProperties)
{
    viscosityProperties_ = viscosityProperties;

    return true;
}


void CML::viscosityModel::correct()
{   
    //- Shear Strain Rate
    ssr_.internalField() = sqrt(2.0)*mag(symm(fvc::grad(U_)));
}


// ************************************************************************* //
