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

#include "mqFaceNonOrthoPluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

namespace CML {

defineTypeNameAndDebug(mqFaceNonOrthoPluginFunction,1);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, mqFaceNonOrthoPluginFunction , name, mqFaceNonOrtho);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

mqFaceNonOrthoPluginFunction::mqFaceNonOrthoPluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    FieldValuePluginFunction(
        parentDriver,
        name,
        word("surfaceScalarField"),
        string("")
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void mqFaceNonOrthoPluginFunction::doEvaluation()
{
    autoPtr<surfaceScalarField> pNonOrto(
        new surfaceScalarField(
            IOobject(
                "faceNonOrthogonality",
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh(),
            dimensionedScalar("nonOr",dimless,0),
            "fixedValue"
        )
    );

    surfaceScalarField &nonOrto=pNonOrto();

    const vectorField& centres = mesh().cellCentres();
    const vectorField& areas = mesh().faceAreas();

    const labelList& own = mesh().faceOwner();
    const labelList& nei = mesh().faceNeighbour();

    forAll(nonOrto,faceI)
    {
        vector d = centres[nei[faceI]] - centres[own[faceI]];
        const vector& s = areas[faceI];

        scalar dDotS = max(
            -1,
            min(
                1,
                (d & s)/(mag(d)*mag(s) + VSMALL)
            )
        );

        // Info << d << s << dDotS << endl;
        // Info << ::acos(dDotS) << endl;

        nonOrto[faceI]=::acos(dDotS)/
        constant::mathematical::pi
        *180.0;
    }

    result().setObjectResult(pNonOrto);
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
