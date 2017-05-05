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

#include "setValueAveragePluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

typedef setValueAveragePluginFunction<scalar> setAverageScalar;
defineTemplateTypeNameAndDebug(setAverageScalar,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setAverageScalar , name, setValueAverageScalar);

typedef setValueAveragePluginFunction<vector> setAverageVector;
defineTemplateTypeNameAndDebug(setAverageVector,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setAverageVector , name, setValueAverageVector);

typedef setValueAveragePluginFunction<tensor> setAverageTensor;
defineTemplateTypeNameAndDebug(setAverageTensor,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setAverageTensor , name, setValueAverageTensor);

typedef setValueAveragePluginFunction<symmTensor> setAverageSymmTensor;
defineTemplateTypeNameAndDebug(setAverageSymmTensor,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setAverageSymmTensor , name, setValueAverageSymmTensor);

typedef setValueAveragePluginFunction<sphericalTensor> setAverageSphericalTensor;
defineTemplateTypeNameAndDebug(setAverageSphericalTensor,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setAverageSphericalTensor , name, setValueAverageSphericalTensor);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
setValueAveragePluginFunction<Type>::setValueAveragePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    GeneralSetEvaluationPluginFunction<Type>(
        parentDriver,
        name
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void setValueAveragePluginFunction<Type>::doEvaluation()
{
    typedef typename GeneralSetEvaluationPluginFunction<Type>::resultType rType;
    autoPtr<rType> pValueAverage(
        new rType(
            IOobject(
                "setValueAverageInCell",
                this->mesh().time().timeName(),
                this->mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            this->mesh(),
            dimensioned<Type>("no",dimless,pTraits<Type>::zero)
        )
    );

    const labelList &cells=this->theSet().cells();
    scalarField nr(pValueAverage->size(),0);
    const Field<Type> vals=this->values();

    forAll(cells,i) {
        const label cellI=cells[i];

        pValueAverage()[cellI]+=vals[i];
        nr[cellI]++;
    }

    forAll(nr,cellI) {
        if(nr[cellI]>SMALL) {
            pValueAverage->internalField()[cellI]/=nr[cellI];
        }
    }

    pValueAverage->correctBoundaryConditions();

    this->result().setObjectResult(pValueAverage);
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
