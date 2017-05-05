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
    EvolveSolidParticleCloudFunctionObject

Description
    Function object that creates and evolves a SolidParticleCloud at every timestep


SourceFiles
    EvolveSolidParticleCloudFunctionObject.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef EvolveSolidParticleCloudFunctionObject_H
#define EvolveSolidParticleCloudFunctionObject_H

#include "EvolveCloudFunctionObject.hpp"
#include "solidParticleCloud.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class EvolveSolidParticleCloudFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class EvolveSolidParticleCloudFunctionObject
:
    public EvolveCloudFunctionObject<solidParticleCloud>
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    EvolveSolidParticleCloudFunctionObject(const EvolveSolidParticleCloudFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const EvolveSolidParticleCloudFunctionObject&);

public:

    //- Runtime type information
    TypeName("EvolveSolidParticleCloudFunctionObject");


    // Constructors

    //- Construct from components
    EvolveSolidParticleCloudFunctionObject
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
