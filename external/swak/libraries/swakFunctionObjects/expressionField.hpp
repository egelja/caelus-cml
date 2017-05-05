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
    expressionField

Description
    Function object that generates a field

SourceFiles
    expressionField.cpp

Contributors/Copyright:
    2010-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#ifndef expressionField_H
#define expressionField_H

#include "primitiveFieldsFwd.hpp"
#include "volFieldsFwd.hpp"
#include "pointFieldFwd.hpp"
#include "Switch.hpp"
#include "autoPtr.hpp"
#include "dictionary.hpp"
#include "dimensionSet.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "exprString.hpp"

namespace CML
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;
class polyMesh;

class FieldValueExpressionDriver;
class regIOobject;

class expressionField
{
    //- Disallow default bitwise copy construct
    expressionField(const expressionField&);

    //- Disallow default bitwise assignment
    void operator=(const expressionField&);

protected:

    // Private data

    //- are we working?
    bool active_;

    //- Name of the field
    word name_;

    //- Expression
    exprString expression_;

    //- should the file be autowrite
    Switch autowrite_;

    const objectRegistry& obr_;

    autoPtr<regIOobject> field_;

    dictionary dict_;

    template<class T>
    void storeField(
        const T &data
    );

    autoPtr<FieldValueExpressionDriver> driver_;

    dimensionSet dimensions_;

    bool setDimensions_;

public:

    //- Runtime type information
    TypeName("expressionField");


    // Constructors

    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    expressionField
    (
        const word& name,
        const objectRegistry&,
        const dictionary&,
        const bool loadFromFiles = false
    );


    //- Destructor
    virtual ~expressionField();


    // Member Functions

    //- Return name of the set of forces
    virtual const word& name() const
        {
            return name_;
        }

    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();

    //- Read the forces data
    virtual void read(const dictionary&);

    //- Execute, currently does nothing
    virtual void execute();

    //- Execute at the final time-loop, currently does nothing
    virtual void end();

    //- Write the forces
    virtual void write();

    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
        {}

    //- Update for changes of mesh
    virtual void movePoints(const pointField&)
        {}

    //- Clear all data
    void clearData();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif
