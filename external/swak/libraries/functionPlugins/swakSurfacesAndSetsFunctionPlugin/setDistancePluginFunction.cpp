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

#include "setDistancePluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

defineTypeNameAndDebug(setDistancePluginFunction,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setDistancePluginFunction , name, setDistance);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

setDistancePluginFunction::setDistancePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    GeneralSetsPluginFunction(
        parentDriver,
        name,
        "volScalarField",
        string("setName primitive word")
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void setDistancePluginFunction::doEvaluation()
{
    autoPtr<volScalarField> pDistance(
        new volScalarField(
            IOobject(
                "setDistanceInCell",
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh(),
            dimensionedScalar("no",dimless,HUGE_VAL)
        )
    );

    const vectorField &cc=mesh().C();

    List<vectorField> faceCenters(Pstream::nProcs());
    faceCenters[Pstream::myProcNo()]=theSet();
    Pstream::scatterList(faceCenters);

    forAll(cc,cellI) {
        const vector &here=cc[cellI];
        scalar &minDist=(pDistance)()[cellI];

        forAll(faceCenters,i){
            const vectorField &fc=faceCenters[i];
            forAll(fc,faceI){
                const scalar dist=mag(here-fc[faceI]);
                if(dist<minDist) {
                    minDist=dist;
                }
            }
        }
    }

    pDistance->correctBoundaryConditions();

    result().setObjectResult(pDistance);
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
