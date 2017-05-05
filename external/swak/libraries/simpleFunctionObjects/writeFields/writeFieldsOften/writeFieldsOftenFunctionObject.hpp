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
    writeFieldsOftenFunctionObject

Description
    Lets the user specify an additional set of fields that is to be written
    more frequently than specified in the controlDict

    writeControl accepts the same values

Example usage: writes Temperature every third timestep after t=11

functions
(
    writeT {
        functionObjectLibs ("libsimpleFunctionObjects.so");
        type writeFieldsOften;
        fieldNames ( T );
        after 11;
        writeControl timeStep;
        writeIntervall 3;
    }
);

SourceFiles
    writeFieldsOftenFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef writeFieldsOftenFunctionObject_H
#define writeFieldsOftenFunctionObject_H

#include "writeFieldsGeneralFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class writeFieldsOftenFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class writeFieldsOftenFunctionObject
:
    public writeFieldsGeneralFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    writeFieldsOftenFunctionObject(const writeFieldsOftenFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const writeFieldsOftenFunctionObject&);

protected:

    //- Overwrite the default output time
    virtual bool outputTime(const bool forceWrite);

    //- how writting is controlled
    Time::writeControls writeControl_;

    //- how often we write
    scalar writeInterval_;

    //- store the last output
    label outputTimeIndex_;
public:

    //- Runtime type information
    TypeName("writeFieldsOften");


    // Constructors

    //- Construct from components
    writeFieldsOftenFunctionObject
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
