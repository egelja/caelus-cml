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

Class
    CML::expressionSource

Description
    Generates a field according to an expression

SourceFiles
    expressionSource.cpp

Contributors/Copyright:
    2010, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef expressionSource_H
#define expressionSource_H

#include "FieldValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class expressionSource Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class expressionSource
:
    protected FieldValueExpressionDriver
{
    // Private data

    expressionSource(const expressionSource&);

    exprString expression_;

public:

    // Constructors

        //- Construct from a dictionary
        expressionSource
        (
            const dictionary& ,
            const fvMesh&
        );

    // Destructor

        virtual ~expressionSource();

    typedef GeometricField<T,fvPatchField,volMesh> resultField;

    //- give back a Field
    tmp<resultField> operator()();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
