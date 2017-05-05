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
    CML::dimensionedTensorEntryToExpression

Description
    Creates an expression from a dimensioned tensor

SourceFiles
    dimensionedTensorEntryToExpression.cpp

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef dimensionedTensorEntryToExpression_H
#define dimensionedTensorEntryToExpression_H

#include "entryToExpression.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class dimensionedTensorEntryToExpression Declaration
\*---------------------------------------------------------------------------*/

class dimensionedTensorEntryToExpression
:
    public entryToExpression
{
    // Private data

    //- Construct as copy
    dimensionedTensorEntryToExpression(const dimensionedTensorEntryToExpression&);

    void operator=(const dimensionedTensorEntryToExpression&);

public:
    //- Runtime type information
    TypeName("dimensionedTensorEntryToExpression");

    // Constructors

    //- Construct null
    dimensionedTensorEntryToExpression();

    // Destructor

    virtual ~dimensionedTensorEntryToExpression();

    virtual string toExpr(const entry &);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
