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
    recalcThermoHeFunctionObject

Description
    Recalculate energy (internal or enthalpy) according to the
    current temperature

SourceFiles
    recalcThermoHeFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013, 2016-2017 Bernhard F.W. Gschaider <bgschaid@hfd-research.com>

\*---------------------------------------------------------------------------*/

#ifndef recalcThermoHeFunctionObject_H
#define recalcThermoHeFunctionObject_H

#include "updateSimpleFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class recalcThermoHeFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class recalcThermoHeFunctionObject
:
    public updateSimpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    recalcThermoHeFunctionObject(const recalcThermoHeFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const recalcThermoHeFunctionObject&);

protected:
    //- do the actual calculation
    void recalc();

public:

    //- Runtime type information
    TypeName("recalcThermoHe");

    // Constructors

    //- Construct from components
    recalcThermoHeFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
