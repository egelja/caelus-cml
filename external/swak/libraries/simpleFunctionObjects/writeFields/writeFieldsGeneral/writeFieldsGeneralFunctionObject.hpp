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
    writeFieldsGeneralFunctionObject

Description
    General Class for writting Fields

SourceFiles
    writeFieldsGeneralFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef writeFieldsGeneralFunctionObject_H
#define writeFieldsGeneralFunctionObject_H

#include "simpleFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class writeFieldsGeneralFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class writeFieldsGeneralFunctionObject
:
    public simpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    writeFieldsGeneralFunctionObject(const writeFieldsGeneralFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const writeFieldsGeneralFunctionObject&);

    //- Name of the fields that are to be written
    wordList fieldNames_;

    //- wrapper for the writting
    template <class T>
    bool writeField(const word &name) const;

protected:
    // //- Overwrite the default output time
    // virtual bool outputTime();
public:

    // Constructors

    //- Construct from components
    writeFieldsGeneralFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();

    void write();

    const wordList &fieldNames() const { return fieldNames_; }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "volumeFieldFunctionObject.hpp"
#include "volFields.hpp"
#include "IOmanip.hpp"
#include "fvMesh.hpp"
#include "fvCFD.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class T>
bool writeFieldsGeneralFunctionObject::writeField(const word &name) const
{
    if(obr_.foundObject<T>(name)) {
        obr_.lookupObject<T>(name).write();

        return true;
    } else {
        return false;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
