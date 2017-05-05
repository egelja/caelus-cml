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
    CML::tensorEntryToExpression

Description
    Creates an expression from a tensor

SourceFiles
    tensorEntryToExpression.cpp

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef tensorEntryToExpression_H
#define tensorEntryToExpression_H

#include "entryToExpression.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class tensorEntryToExpression Declaration
\*---------------------------------------------------------------------------*/

class tensorEntryToExpression
:
    public entryToExpression
{
    // Private data

    //- Construct as copy
    tensorEntryToExpression(const tensorEntryToExpression&);

    void operator=(const tensorEntryToExpression&);

public:
    //- Runtime type information
    TypeName("tensorEntryToExpression");

    // Constructors

    //- Construct null
    tensorEntryToExpression();

    // Destructor

    virtual ~tensorEntryToExpression();

    virtual string toExpr(const entry &);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
