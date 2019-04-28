/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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

#include "GeneralSurfacesPluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "SurfacesRepository.hpp"

#include "meshSearch.hpp"

namespace CML {

defineTypeNameAndDebug(GeneralSurfacesPluginFunction,0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

GeneralSurfacesPluginFunction::GeneralSurfacesPluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name,
    const word &resultType,
    const string &arguments
):
    FieldValuePluginFunction(
        parentDriver,
        name,
        resultType,
        arguments
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void GeneralSurfacesPluginFunction::setArgument(
    label index,
    const word &value
) {
    assert(index==0);

    name_=value;
}

const sampledSurface &GeneralSurfacesPluginFunction::theSurface() const
{
    return SurfacesRepository::getRepository(mesh()).getSurface(
        name_,
        mesh()
    );
}

    // Would be nicer if this functionality was part of sampledSurface
const labelList &GeneralSurfacesPluginFunction::meshCells()
{
    const sampledSurface &surf=theSurface();
    const vectorField& fc = surf.Cf();

    meshCells_.set(
        new labelList(fc.size())
    );

    meshSearch meshSearcher(mesh());

    label seedCell=-1;

    label wrongCellNr=0;

    forAll(fc, triI)
    {
        seedCell=meshSearcher.findCell(
            fc[triI],
            seedCell,
            true
        );
        meshCells_()[triI]=seedCell;
        if(meshCells_()[triI]<0) {
            wrongCellNr++;
        }
    }

    reduce(wrongCellNr,plusOp<label>());
    if(wrongCellNr>0) {
        WarningInFunction
            << "No cell found for " << wrongCellNr << " faces"
            << endl;
    }
    return meshCells_();
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
