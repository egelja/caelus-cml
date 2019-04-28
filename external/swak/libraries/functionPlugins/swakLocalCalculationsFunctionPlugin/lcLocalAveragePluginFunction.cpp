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

#include "lcLocalAveragePluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

defineTypeNameAndDebug(lcLocalAveragePluginFunction,1);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, lcLocalAveragePluginFunction , name, lcLocalAverage);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

lcLocalAveragePluginFunction::lcLocalAveragePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    localCellCalculationFunction(
        parentDriver,
        name,
        string("originalField internalField volScalarField,userCellItself primitive bool")
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void lcLocalAveragePluginFunction::setArgument(
    label index,
    const string &content,
    const CommonValueExpressionDriver &driver
) {
    assert(index==0);

    original_.set(
        new volScalarField(
            //            dynamicCast<const FieldValueExpressionDriver &>(
            dynamic_cast<const FieldValueExpressionDriver &>(
                driver
            ).getResult<volScalarField>()
        )
    );
}

void lcLocalAveragePluginFunction::setArgument(
    label index,
    const bool &sw
) {
    assert(index==1);

    useCellItself_=sw;
}

void lcLocalAveragePluginFunction::doCellCalculation(volScalarField &field)
{
    if(Pstream::parRun()) {
        FatalErrorInFunction
            << "Not yet parallelized"
            << endl
            << exit(FatalError);
    }

    const cellList &cl=field.mesh().cells();
    const volScalarField &o=original_();

    const labelList &own=field.mesh().owner();
    const labelList &nei=field.mesh().neighbour();

    forAll(field,cellI) {
        scalar sum=0;
        label nr=0;

        if(useCellItself_) {
            sum+=o[cellI];
            nr++;
        }
        const cell &c=cl[cellI];
        forAll(c,i) {
            const label faceI=c[i];
            if(faceI<field.mesh().nInternalFaces()) {
                label otherCell=-1;
                if(own[faceI]==cellI) {
                    otherCell=nei[faceI];
                } else {
                    otherCell=own[faceI];
                }
                sum+=o[otherCell];
                nr++;
            }
        }

        if(nr>0) {
            field[cellI]=sum/nr;
        } else {
            FatalErrorInFunction
                << "Cell " << cellI << " doesn't have neighbours. Strange"
                << endl
                << exit(FatalError);
        }
    }
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
