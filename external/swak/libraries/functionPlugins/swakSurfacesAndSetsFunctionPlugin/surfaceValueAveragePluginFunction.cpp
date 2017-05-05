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

#include "surfaceValueAveragePluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

typedef surfaceValueAveragePluginFunction<scalar> surfaceAverageScalar;
defineTemplateTypeNameAndDebug(surfaceAverageScalar,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, surfaceAverageScalar , name, surfaceValueAverageScalar);

typedef surfaceValueAveragePluginFunction<vector> surfaceAverageVector;
defineTemplateTypeNameAndDebug(surfaceAverageVector,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, surfaceAverageVector , name, surfaceValueAverageVector);

typedef surfaceValueAveragePluginFunction<tensor> surfaceAverageTensor;
defineTemplateTypeNameAndDebug(surfaceAverageTensor,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, surfaceAverageTensor , name, surfaceValueAverageTensor);

typedef surfaceValueAveragePluginFunction<symmTensor> surfaceAverageSymmTensor;
defineTemplateTypeNameAndDebug(surfaceAverageSymmTensor,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, surfaceAverageSymmTensor , name, surfaceValueAverageSymmTensor);

typedef surfaceValueAveragePluginFunction<sphericalTensor> surfaceAverageSphericalTensor;
defineTemplateTypeNameAndDebug(surfaceAverageSphericalTensor,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, surfaceAverageSphericalTensor , name, surfaceValueAverageSphericalTensor);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
surfaceValueAveragePluginFunction<Type>::surfaceValueAveragePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    GeneralSurfaceEvaluationPluginFunction<Type>(
        parentDriver,
        name
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void surfaceValueAveragePluginFunction<Type>::doEvaluation()
{
    typedef typename GeneralSurfaceEvaluationPluginFunction<Type>::resultType rType;
    autoPtr<rType> pValueAverage(
        new rType(
            IOobject(
                "surfaceValueAverageInCell",
                this->mesh().time().timeName(),
                this->mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            this->mesh(),
            dimensioned<Type>("no",dimless,pTraits<Type>::zero)
        )
    );

    const labelList &cells=this->meshCells();
    const scalarField &area=this->theSurface().magSf();
    scalarField areaSum(pValueAverage->size(),0);
    const Field<Type> vals=this->values();

    forAll(cells,i) {
        const label cellI=cells[i];

        if(cellI>=0) {
            pValueAverage()[cellI]+=area[i]*vals[i];
            areaSum[cellI]+=area[i];
        }
    }

    forAll(areaSum,cellI) {
        if(areaSum[cellI]>SMALL) {
            pValueAverage->internalField()[cellI]/=areaSum[cellI];
        }
    }

    pValueAverage->correctBoundaryConditions();

    this->result().setObjectResult(pValueAverage);
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
