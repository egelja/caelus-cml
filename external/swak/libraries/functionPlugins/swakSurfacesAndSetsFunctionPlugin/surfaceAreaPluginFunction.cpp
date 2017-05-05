/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on OpenFOAM.

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

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "surfaceAreaPluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

defineTypeNameAndDebug(surfaceAreaPluginFunction,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, surfaceAreaPluginFunction , name, surfaceArea);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

surfaceAreaPluginFunction::surfaceAreaPluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    GeneralSurfacesPluginFunction(
        parentDriver,
        name,
        "volScalarField",
        string("surfaceName primitive word")
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void surfaceAreaPluginFunction::doEvaluation()
{
    if(debug) {
        Info << "surfaceArea for " << name() << endl;
    }

    autoPtr<volScalarField> pArea(
        new volScalarField(
            IOobject(
                "surfaceAreaInCell",
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh(),
            dimensionedScalar("no",dimless,0)
        )
    );

    const labelList &cells=meshCells();
    const scalarField &area=theSurface().magSf();

    if(debug) {
        Pout << "Size " << area.size() << endl;
        //        Pout << gMin(area) << " " << gMax(area) << endl;
    }

    forAll(cells,i) {
        const label cellI=cells[i];

        if(cellI>=0) {
            pArea()[cellI]+=area[i];
        }
    }

    pArea->correctBoundaryConditions();

    result().setObjectResult(pArea);

    if(debug) {
        Info << "Done: surfaceArea for " << name() << endl;
    }
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
