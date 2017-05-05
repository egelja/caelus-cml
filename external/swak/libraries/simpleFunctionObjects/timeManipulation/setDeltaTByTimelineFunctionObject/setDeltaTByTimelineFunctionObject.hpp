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
    setDeltaTByTimelineFunctionObject

Description
    Set the time-step according to a linear piecewise function

SourceFiles
    setDeltaTByTimelineFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef setDeltaTByTimelineFunctionObject_H
#define setDeltaTByTimelineFunctionObject_H

#include "timeManipulationFunctionObject.hpp"

#include "interpolationTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class setDeltaTByTimelineFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class setDeltaTByTimelineFunctionObject
:
    public timeManipulationFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    setDeltaTByTimelineFunctionObject(const setDeltaTByTimelineFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const setDeltaTByTimelineFunctionObject&);

    interpolationTable<scalar> deltaTTable_;

public:

    //- Runtime type information
    TypeName("setDeltaTByTimeline");


    // Constructors

    //- Construct from components
    setDeltaTByTimelineFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    virtual scalar deltaT();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
