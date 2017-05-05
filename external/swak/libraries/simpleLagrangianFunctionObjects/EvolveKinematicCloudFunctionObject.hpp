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
    EvolveKinematicCloudFunctionObject

Description
    Function object that creates and evolves a KinematicCloud at every timestep


SourceFiles
    EvolveKinematicCloudFunctionObject.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef EvolveKinematicCloudFunctionObject_H
#define EvolveKinematicCloudFunctionObject_H

#include "EvolveCloudFunctionObject.hpp"
#include "basicKinematicCloud.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class EvolveKinematicCloudFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class EvolveKinematicCloudFunctionObject
:
    public EvolveCloudFunctionObject<basicKinematicCloud>
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    EvolveKinematicCloudFunctionObject(const EvolveKinematicCloudFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const EvolveKinematicCloudFunctionObject&);

public:

    //- Runtime type information
    TypeName("EvolveKinematicCloudFunctionObject");


    // Constructors

    //- Construct from components
    EvolveKinematicCloudFunctionObject
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
