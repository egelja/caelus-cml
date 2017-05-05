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

#include "simpleFilter.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvc.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(simpleFilter, 0);
    addToRunTimeSelectionTable(LESfilter, simpleFilter, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::simpleFilter::simpleFilter
(
    const fvMesh& mesh
)
:
    LESfilter(mesh)
{}


CML::simpleFilter::simpleFilter(const fvMesh& mesh, const dictionary&)
:
    LESfilter(mesh)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::simpleFilter::read(const dictionary&)
{}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

CML::tmp<CML::volScalarField> CML::simpleFilter::operator()
(
    const tmp<volScalarField>& unFilteredField
) const
{
    tmp<volScalarField> filteredField = fvc::surfaceSum
    (
        mesh().magSf()*fvc::interpolate(unFilteredField)
    )/fvc::surfaceSum(mesh().magSf());

    unFilteredField.clear();

    return filteredField;
}


CML::tmp<CML::volVectorField> CML::simpleFilter::operator()
(
    const tmp<volVectorField>& unFilteredField
) const
{
    tmp<volVectorField> filteredField = fvc::surfaceSum
    (
        mesh().magSf()*fvc::interpolate(unFilteredField)
    )/fvc::surfaceSum(mesh().magSf());

    unFilteredField.clear();

    return filteredField;
}


CML::tmp<CML::volSymmTensorField> CML::simpleFilter::operator()
(
    const tmp<volSymmTensorField>& unFilteredField
) const
{
    tmp<volSymmTensorField> filteredField = fvc::surfaceSum
    (
        mesh().magSf()*fvc::interpolate(unFilteredField)
    )/fvc::surfaceSum(mesh().magSf());

    unFilteredField.clear();

    return filteredField;
}


CML::tmp<CML::volTensorField> CML::simpleFilter::operator()
(
    const tmp<volTensorField>& unFilteredField
) const
{
    tmp<volTensorField> filteredField = fvc::surfaceSum
    (
        mesh().magSf()*fvc::interpolate(unFilteredField)
    )/fvc::surfaceSum(mesh().magSf());

    unFilteredField.clear();

    return filteredField;
}


// ************************************************************************* //
