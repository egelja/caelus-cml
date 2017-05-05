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

Class
    CML::entryToExpression

Description
    Helper class that gets an entry and converts it into a valid
    swak-expression

SourceFiles
    entryToExpression.cpp

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef entryToExpression_H
#define entryToExpression_H

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "entry.hpp"

#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class entryToExpression Declaration
\*---------------------------------------------------------------------------*/

class entryToExpression
{
    // Private data

    //- Construct as copy
    entryToExpression(const entryToExpression&);

    void operator=(const entryToExpression&);

protected:

    template<class Type>
    string toStr(
        const Type &data,
        const word &prefix
    );

public:
    //- Runtime type information
    TypeName("entryToExpression");

    declareRunTimeSelectionTable
    (
        autoPtr,
        entryToExpression,
        nothing,
        (
        ),
        ()
    );

    // Constructors

    //- Construct null
    entryToExpression();

   //- Return a reference to the selected value driver
    static autoPtr<entryToExpression> New
    (
        const word& name
    );

    // Destructor

    virtual ~entryToExpression();

    virtual string toExpr(const entry &) = 0;

    static string fromEntry(const entry &);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "entryToExpressionI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
