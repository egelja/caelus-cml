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
    CML::StoredStackExpressionResult

Description

SourceFiles
    StoredStackExpressionResult.cpp
    StoredStackExpressionResultIO.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef StoredStackExpressionResult_H
#define StoredStackExpressionResult_H

#include "StackExpressionResult.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class StoredStackExpressionResult Declaration
\*---------------------------------------------------------------------------*/

class StoredStackExpressionResult
:
    public StackExpressionResult
{
    // Private data

protected:

    //- reset at new timestep according to type
    virtual void resetInternal();

public:

     //- Runtime type information
    TypeName("StoredStackExpressionResult");

    // Constructors

    //- Construct null
    StoredStackExpressionResult();

    //- Construct as copy
    StoredStackExpressionResult(const StoredStackExpressionResult&);

    //- Construct from a dictionary
    StoredStackExpressionResult(const dictionary&);

    // Destructor

    virtual ~StoredStackExpressionResult();

    void operator=(const StoredStackExpressionResult&);

    virtual autoPtr<ExpressionResult> clone() const {
        return autoPtr<ExpressionResult>(
            new StoredStackExpressionResult(*this)
        );
    }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
