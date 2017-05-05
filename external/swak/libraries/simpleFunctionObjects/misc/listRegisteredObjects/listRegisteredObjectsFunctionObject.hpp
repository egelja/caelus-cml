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
    listRegisteredObjectsFunctionObject

Description
    Prints a list of the objects registered with an object
    registry plus their type and whether they are auto-written

SourceFiles
    listRegisteredObjectsFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef listRegisteredObjectsFunctionObject_H
#define listRegisteredObjectsFunctionObject_H

#include "simpleFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class listRegisteredObjectsFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class listRegisteredObjectsFunctionObject
:
    public simpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    listRegisteredObjectsFunctionObject(const listRegisteredObjectsFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const listRegisteredObjectsFunctionObject&);

    //- dump the contents of the object registry to the screen
    void dumpObr();

public:

    //- Runtime type information
    TypeName("listRegisteredObjects");


    // Constructors

    //- Construct from components
    listRegisteredObjectsFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();

    void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
