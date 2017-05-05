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
    recalcThermoHsFunctionObject

Description
    Recalculate sensible enthalpy according to the current temperature

SourceFiles
    recalcThermoHsFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef recalcThermoHsFunctionObject_H
#define recalcThermoHsFunctionObject_H

#include "updateSimpleFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class recalcThermoHsFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class recalcThermoHsFunctionObject
:
    public updateSimpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    recalcThermoHsFunctionObject(const recalcThermoHsFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const recalcThermoHsFunctionObject&);

protected:
    //- do the actual calculation
    void recalc();

public:

    //- Runtime type information
    TypeName("recalcThermoHs");

    // Constructors

    //- Construct from components
    recalcThermoHsFunctionObject
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
