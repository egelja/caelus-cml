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
    CML::StackExpressionResult

Description

SourceFiles
    StackExpressionResultI.hpp
    StackExpressionResult.cpp
    StackExpressionResultIO.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef StackExpressionResult_H
#define StackExpressionResult_H

#include "ExpressionResult.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class StackExpressionResult Declaration
\*---------------------------------------------------------------------------*/

class StackExpressionResult
:
    public ExpressionResult
{
    // Private data

    template<class T>
    ExpressionResult popInternal();

    template<class T>
    void pushInternal(ExpressionResult &);

public:

     //- Runtime type information
    TypeName("StackExpressionResult");

    // Constructors

    //- Construct null
    StackExpressionResult();

    //- Construct as copy
    StackExpressionResult(const StackExpressionResult&);

    //- Construct from a dictionary
    StackExpressionResult(const dictionary&);

    // Destructor

    virtual ~StackExpressionResult();

    void operator=(const StackExpressionResult&);

    void operator=(const ExpressionResult&);

    virtual autoPtr<ExpressionResult> clone() const {
        return autoPtr<ExpressionResult>(
            new StackExpressionResult(*this)
        );
    }

    // friend Ostream & operator<<(Ostream &,const StackExpressionResult &);

    // friend Istream & operator>>(Istream &,StackExpressionResult &);

    // friend bool operator!=(
    //     const StackExpressionResult &,const StackExpressionResult &
    // );

    ExpressionResult pop();

    void push(ExpressionResult &);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "StackExpressionResultI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
