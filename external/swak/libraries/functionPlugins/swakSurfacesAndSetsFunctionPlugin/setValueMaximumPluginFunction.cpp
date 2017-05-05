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

#include "setValueMaximumPluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

typedef setValueMaximumPluginFunction<scalar> setMaximumScalar;
defineTemplateTypeNameAndDebug(setMaximumScalar,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setMaximumScalar , name, setValueMaximumScalar);

typedef setValueMaximumPluginFunction<vector> setMaximumVector;
defineTemplateTypeNameAndDebug(setMaximumVector,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setMaximumVector , name, setValueMaximumVector);

typedef setValueMaximumPluginFunction<tensor> setMaximumTensor;
defineTemplateTypeNameAndDebug(setMaximumTensor,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setMaximumTensor , name, setValueMaximumTensor);

typedef setValueMaximumPluginFunction<symmTensor> setMaximumSymmTensor;
defineTemplateTypeNameAndDebug(setMaximumSymmTensor,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setMaximumSymmTensor , name, setValueMaximumSymmTensor);

typedef setValueMaximumPluginFunction<sphericalTensor> setMaximumSphericalTensor;
defineTemplateTypeNameAndDebug(setMaximumSphericalTensor,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setMaximumSphericalTensor , name, setValueMaximumSphericalTensor);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
setValueMaximumPluginFunction<Type>::setValueMaximumPluginFunction(
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
void setValueMaximumPluginFunction<Type>::doEvaluation()
{
    typedef typename GeneralSetEvaluationPluginFunction<Type>::resultType rType;
    autoPtr<rType> pValueMaximum(
        new rType(
            IOobject(
                "setValueMaximumInCell",
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
    List<bool> here(pValueMaximum->size(),false);
    const Field<Type> vals=this->values();

    forAll(cells,i) {
        const label cellI=cells[i];

        if(here[cellI]) {
            pValueMaximum()[cellI]=max(
                vals[i],
                pValueMaximum()[cellI]
            );
        } else {
            here[cellI]=true;
            pValueMaximum()[cellI]=vals[i];
        }
    }

    pValueMaximum->correctBoundaryConditions();

    this->result().setObjectResult(pValueMaximum);
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
