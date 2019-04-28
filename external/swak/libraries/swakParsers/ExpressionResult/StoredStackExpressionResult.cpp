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

#include "StoredStackExpressionResult.hpp"
#include "vector.hpp"
#include "tensor.hpp"
#include "symmTensor.hpp"
#include "sphericalTensor.hpp"

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

defineTypeNameAndDebug(StoredStackExpressionResult,0);

addToRunTimeSelectionTable(ExpressionResult, StoredStackExpressionResult, dictionary);
addToRunTimeSelectionTable(ExpressionResult, StoredStackExpressionResult, nothing);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

StoredStackExpressionResult::StoredStackExpressionResult()
:
    StackExpressionResult()
{
    // reset the setting of the parent-class
    setNeedsReset(false);
}

StoredStackExpressionResult::StoredStackExpressionResult(
    const StoredStackExpressionResult &rhs
)
:
    StackExpressionResult(rhs)
{
    // reset the setting of the parent-class
    setNeedsReset(false);
}

StoredStackExpressionResult::StoredStackExpressionResult(const dictionary &dict)
:
    StackExpressionResult(dict)
{
    Dbug << "StoredStackExpressionResult(const dictionary &dict)" << endl;
    Dbug << "Value: " << (*this) << endl;

    // reset the setting of the parent-class
    setNeedsReset(false);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

StoredStackExpressionResult::~StoredStackExpressionResult()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void StoredStackExpressionResult::resetInternal()
{
    // do nothing
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void StoredStackExpressionResult::operator=(const StoredStackExpressionResult& rhs)
{
    Dbug << "operator=(const StoredStackExpressionResult& rhs)" << endl;

    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "Attempted assignment to self"
            << exit(FatalError);
    }

    static_cast<StackExpressionResult&>(*this)=rhs;
}

// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


} // namespace

// ************************************************************************* //
