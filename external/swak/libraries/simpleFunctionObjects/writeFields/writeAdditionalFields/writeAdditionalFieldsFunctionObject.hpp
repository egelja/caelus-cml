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
    writeAdditionalFieldsFunctionObject

Description
    Lets the user specify an additional set of fields that is to be written with the other
    fields the are written by AUTO_WRITE

Example usage: writes Enthalpy and viscosity after t=15

functions
(
    writeEnthalpy {
        functionObjectLibs ("libsimpleFunctionObjects.so");
        type writeAdditionalFields;
        fieldNames ( h mu );
        after 15;
    }
);

SourceFiles
    writeAdditionalFieldsFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef writeAdditionalFieldsFunctionObject_H
#define writeAdditionalFieldsFunctionObject_H

#include "writeFieldsGeneralFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class writeAdditionalFieldsFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class writeAdditionalFieldsFunctionObject
:
    public writeFieldsGeneralFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    writeAdditionalFieldsFunctionObject(const writeAdditionalFieldsFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const writeAdditionalFieldsFunctionObject&);

protected:

    // //- Overwrite the default output time
    // virtual bool outputTime();
public:

    //- Runtime type information
    TypeName("writeAdditionalFields");


    // Constructors

    //- Construct from components
    writeAdditionalFieldsFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
