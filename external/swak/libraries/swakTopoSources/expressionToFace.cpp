/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author

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
    2010-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "expressionToFace.hpp"
#include "polyMesh.hpp"
#include "cellSet.hpp"
#include "Time.hpp"
#include "syncTools.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "FieldValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(expressionToFace, 0);

addToRunTimeSelectionTable(topoSetSource, expressionToFace, word);

addToRunTimeSelectionTable(topoSetSource, expressionToFace, istream);

}


CML::topoSetSource::addToUsageTable CML::expressionToFace::usage_
(
    expressionToFace::typeName,
    "\n    Usage: expressionToFace <expression>\n\n"
    "    Select all faces for which expression evaluates to true on one and false on the other side\n\n"
);

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::expressionToFace::combine(topoSet& set, const bool add) const
{
    if(Pstream::parRun()) {
        WarningInFunction
            << " Does not give correct results if faces are on the processor boundary"
            << endl;
    }

    fvMesh mesh(set.db());

    FieldValueExpressionDriver driver
        (
            mesh.time().timeName(),
            mesh.time(),
            mesh,
            true, // cache stuff
            true, // search in memory
            true  // search on disc
        );

    if(dict_.valid()) {
        driver.readVariablesAndTables(dict_());
        driver.clearVariables();
    }
    driver.parse(expression_);
    if(!driver.isLogical()) {
        FatalErrorInFunction
            << "Expression " << expression_ << " does not evaluate to a logical expression"
            << endl
            << exit(FatalError);
    }

    if(driver.resultIsTyp<volScalarField>(true)) {
        const volScalarField &condition=driver.getResult<volScalarField>();

        const labelList &own=condition.mesh().faceOwner();
        const labelList &nei=condition.mesh().faceNeighbour();

        Info << "    Expression " << expression_
            << " evaluates to cellValue: using boundary" << endl;

        for(label faceI=0;faceI<condition.mesh().nInternalFaces();faceI++)
        {
            if (condition[own[faceI]] != condition[nei[faceI]])
            {
                addOrDelete(set, faceI, add);
            }
        }
    } else if(driver.resultIsTyp<surfaceScalarField>(true)) {
        const surfaceScalarField &condition=driver.getResult<surfaceScalarField>();
        forAll(condition,faceI) {
            if(condition[faceI]>0) {
                addOrDelete(set, faceI, add);
            }
        }
        forAll(condition.boundaryField(),patchI) {
            const surfaceScalarField::PatchFieldType &patch=
                condition.boundaryField()[patchI];
            label start=condition.mesh().boundaryMesh()[patchI].start();

            forAll(patch,i) {
                if(patch[i]>0) {
                    addOrDelete(set, i+start, add);
                }
            }
        }
    } else {
        FatalErrorInFunction
            << "Don't know how to handle a logical field of type "
            << driver.typ()
            << endl
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from componenta
CML::expressionToFace::expressionToFace
(
    const polyMesh& mesh,
    const exprString& expression
)
:
    topoSetSource(mesh),
    expression_(expression)
{}


// Construct from dictionary
CML::expressionToFace::expressionToFace
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    expression_(
        dict.lookup("expression"),
        dict
    ),
    dict_(new dictionary(dict))
{}


// Construct from Istream
CML::expressionToFace::expressionToFace
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    expression_(
        checkIs(is),
        dictionary::null
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::expressionToFace::~expressionToFace()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::topoSetSource::sourceType CML::expressionToFace::setType() const
{
    return FACESETSOURCE;
}

void CML::expressionToFace::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::ADD) || (action == topoSetSource::NEW))
    {
        Info<< "    Adding all elements of for which " << expression_ << " evaluates to true ..."
            << endl;

        combine(set,true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing all elements of for which " << expression_ << " evaluates to true ..."
            << endl;

        combine(set,false);
    }
}


// ************************************************************************* //
