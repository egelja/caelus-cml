/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Deltares.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

#include "relaxationShapeFrozen.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "relaxationShapeRectangular.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationShapes
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(relaxationShapeFrozen, 0);
addToRunTimeSelectionTable
(
    relaxationShape,
    relaxationShapeFrozen,
    dictionary
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


relaxationShapeFrozen::relaxationShapeFrozen
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    relaxationShape(subDictName, mesh_)
{
    // Everything is placed in the constructor in order to avoid any updating
    // the relaxation zone coefficients during mesh motion.
    if (mesh_.time().timeName() == "0")
    {
        word actualType(coeffDict_.lookup("actualRelaxationShape"));

        if ("relaxationShape"+actualType == typeName)
        {
            FatalErrorInFunction
                << "The chosen type of the relaxation shape type "
                << "'actualRelaxationShape' is the calling type.\n"
                << "This is not allowed.\n\n"
                << endl << exit(FatalError);
        }

        autoPtr<CML::relaxationShapes::relaxationShape> rsr
            = CML::relaxationShapes::relaxationShape
              ::New(subDictName, actualType, mesh_);

        const labelList& recCells = rsr->cells();
        const scalarField& recSigma = rsr->sigma();

        cells_.setSize(recCells.size(), -1);
        cells_ = recCells;

        sigma_.setSize(recSigma.size(), -1);
        sigma_ = recSigma;

        // Writing the cell indices to prepare for restart of computation
        IOField<label> cellsOutput
        (
            IOobject
            (
                "cells_" + subDictName,
                mesh_.time().constant(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            )
        );

        cellsOutput.setSize(cells_.size());
        forAll (cells_, celli)
        {
            cellsOutput[celli] = cells_[celli];
        }

        cellsOutput.write();

        IOField<scalar> sigmaOutput
        (
            IOobject
            (
                "sigma_" + subDictName,
                mesh_.time().constant(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            )
        );

        sigmaOutput.setSize(sigma_.size());
        sigmaOutput = sigma_;

        sigmaOutput.write();
    }
    else
    {
        IOField<label> cellsInput
        (
            IOobject
            (
                "cells_" + subDictName,
                mesh_.time().constant(),
                mesh_,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        );

        cells_.setSize(cellsInput.size());
        cells_ = cellsInput;

        IOField<scalar> sigmaInput
        (
            IOobject
            (
                "sigma_" + subDictName,
                mesh_.time().constant(),
                mesh_,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        );

        sigma_.setSize(sigmaInput.size());
        sigma_ = sigmaInput;
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void relaxationShapeFrozen::findComputationalCells()
{
    // Intentionally left blank
}


void relaxationShapeFrozen::computeSigmaCoordinate()
{
    // Intentionally left blank
}


const pointField& relaxationShapeFrozen::pointSet()
{
    NotImplemented;
}


scalar relaxationShapeFrozen::interpolation
(
    const scalarField& source,
    const point& p0
) const
{
    NotImplemented;
    return 0.0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationShapes
} // End namespace CML

// ************************************************************************* //
