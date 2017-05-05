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
    EvolveReactingCloudFunctionObject

Description
    Function object that creates and evolves a ReactingCloud at every timestep

SourceFiles
    EvolveReactingCloudFunctionObject.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef EvolveReactingCloudFunctionObject_H
#define EvolveReactingCloudFunctionObject_H

#include "EvolveCloudFunctionObject.hpp"

#include "swakCloudTypes.hpp"

#include "basicReactingCloud.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class EvolveReactingCloudFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class EvolveReactingCloudFunctionObject
:
    public EvolveCloudFunctionObject<basicReactingCloud>
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    EvolveReactingCloudFunctionObject(const EvolveReactingCloudFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const EvolveReactingCloudFunctionObject&);

public:

    //- Runtime type information
    TypeName("EvolveReactingCloudFunctionObject");


    // Constructors

    //- Construct from components
    EvolveReactingCloudFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );


    // Member Functions

    //- start is called at the start of the time-loop
    virtual bool start();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
