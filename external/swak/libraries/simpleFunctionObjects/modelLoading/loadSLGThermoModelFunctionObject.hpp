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
    loadSLGThermoModelFunctionObject

Description
    Loads a SLGThermo model into memory

SourceFiles
    loadSLGThermoModelFunctionObject.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef loadSLGThermoModelFunctionObject_H
#define loadSLGThermoModelFunctionObject_H

#include "modelLoadingFunctionObject.hpp"
#include "SLGThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class loadSLGThermoModelFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class loadSLGThermoModelFunctionObject
:
    public modelLoadingFunctionObject<SLGThermo>
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    loadSLGThermoModelFunctionObject(const loadSLGThermoModelFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const loadSLGThermoModelFunctionObject&);

protected:
    virtual autoPtr<SLGThermo> initModel();

public:

    //- Runtime type information
    TypeName("loadSLGThermoModelFunctionObject");


    // Constructors

    //- Construct from components
    loadSLGThermoModelFunctionObject
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
