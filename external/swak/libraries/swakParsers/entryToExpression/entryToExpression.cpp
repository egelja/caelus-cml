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
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "entryToExpression.hpp"

#include "primitiveEntry.hpp"
#include "OStringStream.hpp"

namespace CML {

defineTypeNameAndDebug(entryToExpression,0);

defineRunTimeSelectionTable(entryToExpression, nothing);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

entryToExpression::entryToExpression()
{
}


autoPtr<entryToExpression> entryToExpression::New
(
    const word& name
)
{
    nothingConstructorTable::iterator cstrIter =
        nothingConstructorTablePtr_->find(name);

    if (cstrIter == nothingConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown  entryToExpression type " << name
            << endl << endl
            << "Valid entryToExpression-types are :" << endl
            << nothingConstructorTablePtr_->sortedToc() // does not work in 1.6
            << exit(FatalError);
    }

    return autoPtr<entryToExpression>
        (
            cstrIter()()
        );
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

entryToExpression::~entryToExpression()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

string entryToExpression::fromEntry(const entry &e)
{
    const primitiveEntry &pe=dynamicCast<const primitiveEntry&>(e);
    OStringStream o;

    for (label i=0; i<pe.size(); i++) {
        o << pe[i];

        if (i < pe.size()-1){
            o << token::SPACE;
        }
    }
    return o.str();
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
