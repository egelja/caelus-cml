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
    simpleFunctionObject

Description
    Basis for the other function objects here. Creates the directory and the files

SourceFiles
    simpleFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef simpleDataFunctionObject_H
#define simpleDataFunctionObject_H

#include "simpleFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class simpleDataFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class simpleDataFunctionObject
:
    public simpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    simpleDataFunctionObject(const simpleDataFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const simpleDataFunctionObject&);

protected:

    //- Get the path to the data directory
    virtual fileName dataDir();

    //- Get the path to the base directory
    fileName baseDir();

    //- Name of the sub-dictionary of the case
    virtual word dirName()=0;

public:

    //- Runtime type information
    TypeName("simpleDataFunctionObject");


    // Constructors

    //- Construct from components
    simpleDataFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
