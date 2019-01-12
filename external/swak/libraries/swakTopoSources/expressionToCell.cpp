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

#include "expressionToCell.hpp"
#include "polyMesh.hpp"
#include "cellSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "FieldValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(expressionToCell, 0);

addToRunTimeSelectionTable(topoSetSource, expressionToCell, word);

addToRunTimeSelectionTable(topoSetSource, expressionToCell, istream);

}


CML::topoSetSource::addToUsageTable CML::expressionToCell::usage_
(
    expressionToCell::typeName,
    "\n    Usage: expressionToCell <expression>\n\n"
    "    Select all cells for which expression evaluates to true\n\n"
);

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::expressionToCell::combine(topoSet& set, const bool add) const
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
    if(!driver.resultIsTyp<volScalarField>(true)) {
        FatalErrorInFunction
            << "Expression " << expression_ << " does not evaluate to a logical expression"
            << endl
            << exit(FatalError);
    }
    const volScalarField &condition=driver.getResult<volScalarField>();

    forAll(condition, cellI)
    {
        if (condition[cellI])
        {
            addOrDelete(set, cellI, add);
        }
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::expressionToCell::expressionToCell
(
    const polyMesh& mesh,
    const exprString& expression
)
:
    topoSetSource(mesh),
    expression_(expression)
{}


// Construct from dictionary
CML::expressionToCell::expressionToCell
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
CML::expressionToCell::expressionToCell
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

CML::expressionToCell::~expressionToCell()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::topoSetSource::sourceType CML::expressionToCell::setType() const
{
    return CELLSETSOURCE;
}

void CML::expressionToCell::applyToSet
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
