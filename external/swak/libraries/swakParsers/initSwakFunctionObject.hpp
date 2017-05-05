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
    initSwakFunctionObject

Description
    Basis for the other function objects here. Evaluates the most common options

SourceFiles
    initSwakFunctionObject.cpp

Contributors/Copyright:
    2008-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef initSwakFunctionObject_H
#define initSwakFunctionObject_H

#include "functionObject.hpp"
#include "dictionary.hpp"
#include "OFstream.hpp"
#include "HashPtrTable.hpp"
#include "boolList.hpp"
#include "Field.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"
#include "switchableNotImplemented.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class initSwakFunctionObject Declaration
\*---------------------------------------------------------------------------*/

    class objectRegistry;

class initSwakFunctionObject
:
    public functionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    initSwakFunctionObject(const initSwakFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const initSwakFunctionObject&);

    // Private data

    dictionary dict_;

public:

    //- Runtime type information
    TypeName("initSwakFunctionObject");


    // Constructors

    //- Construct from components
    initSwakFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );


    // Member Functions

    //- start is called at the start of the time-loop
    virtual bool start();

    //- execute is called at each ++ or += of the time-loop
    virtual bool execute(const bool forceWrite);

    //- Read and set the function object if it's data has changed
    virtual bool read(const dictionary& dict);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
