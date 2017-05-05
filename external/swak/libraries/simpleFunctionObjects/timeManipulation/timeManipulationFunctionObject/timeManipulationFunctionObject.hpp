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
    timeManipulationFunctionObject

Description
    Basis for functionObjects that manipulate the runTime for instance by
    - setting the timesteps
    - setting the endTime
    Concrete subclasses have to override the runTime

SourceFiles
    timeManipulationFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef timeManipulationFunctionObject_H
#define timeManipulationFunctionObject_H

#include "simpleFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class timeManipulationFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class timeManipulationFunctionObject
:
    public simpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    timeManipulationFunctionObject(const timeManipulationFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const timeManipulationFunctionObject&);

    bool tolerateAdaptiveTimestep_;

    scalar myEndTime_;

public:

    //- Runtime type information
    TypeName("timeManipulation");


    // Constructors

    //- Construct from components
    timeManipulationFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();

    void write();

    //- new timestep. To be overridden. This is only a dummy-implementation
    virtual scalar deltaT();

    //- new endTime. To be overridden. This is only a dummy-implementation
    virtual scalar endTime();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
