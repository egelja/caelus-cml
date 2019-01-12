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
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "expressionToPoint.hpp"
#include "polyMesh.hpp"
#include "pointSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "FieldValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(expressionToPoint, 0);

addToRunTimeSelectionTable(topoSetSource, expressionToPoint, word);

addToRunTimeSelectionTable(topoSetSource, expressionToPoint, istream);

}


CML::topoSetSource::addToUsageTable CML::expressionToPoint::usage_
(
    expressionToPoint::typeName,
    "\n    Usage: expressionToPoint <expression>\n\n"
    "    Select all points for which expression evaluates to true\n\n"
);

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::expressionToPoint::combine(topoSet& set, const bool add) const
{
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
    if(!driver.resultIsTyp<pointScalarField>(true)) {
        FatalErrorInFunction
            << "Expression " << expression_ << " does not evaluate to a logical expression"
            << endl
            << exit(FatalError);
    }
    const pointScalarField &condition=driver.getResult<pointScalarField>();

    forAll(condition, pointI)
    {
        if (condition[pointI])
        {
            addOrDelete(set, pointI, add);
        }
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::expressionToPoint::expressionToPoint
(
    const polyMesh& mesh,
    const exprString& expression
)
:
    topoSetSource(mesh),
    expression_(expression)
{}


// Construct from dictionary
CML::expressionToPoint::expressionToPoint
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
CML::expressionToPoint::expressionToPoint
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

CML::expressionToPoint::~expressionToPoint()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::topoSetSource::sourceType CML::expressionToPoint::setType() const
{
    return POINTSETSOURCE;
}

void CML::expressionToPoint::applyToSet
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
