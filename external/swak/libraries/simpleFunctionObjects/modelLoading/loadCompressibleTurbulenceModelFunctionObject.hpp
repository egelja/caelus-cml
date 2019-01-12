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
    loadCompressibleTurbulenceModelFunctionObject

Description
    Loads a turbulence model into memory

SourceFiles
    loadCompressibleTurbulenceModelFunctionObject.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
 
\*---------------------------------------------------------------------------*/

#ifndef loadCompressibleTurbulenceModelFunctionObject_H
#define loadCompressibleTurbulenceModelFunctionObject_H

#include "modelLoadingFunctionObject.hpp"
#include "compressible/turbulenceModel/compressibleTurbulenceModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class loadCompressibleTurbulenceModelFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class loadCompressibleTurbulenceModelFunctionObject
:
    public modelLoadingFunctionObject<compressible::turbulenceModel>
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    loadCompressibleTurbulenceModelFunctionObject(const loadCompressibleTurbulenceModelFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const loadCompressibleTurbulenceModelFunctionObject&);

protected:
    virtual autoPtr<compressible::turbulenceModel> initModel();

public:

    //- Runtime type information
    TypeName("loadCompressibleTurbulenceModelFunctionObject");


    // Constructors

    //- Construct from components
    loadCompressibleTurbulenceModelFunctionObject
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
