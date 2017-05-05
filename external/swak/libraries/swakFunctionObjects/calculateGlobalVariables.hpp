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
    calculateGlobalVariables

Description
    Function object that calculates some variables and adds them to a globel
    namespace

SourceFiles
    calculateGlobalVariables.cpp

Contributors/Copyright:
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#ifndef calculateGlobalVariables_H
#define calculateGlobalVariables_H

#include "primitiveFieldsFwd.hpp"
#include "pointFieldFwd.hpp"

#include "CommonValueExpressionDriver.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

namespace CML
{
// Forward declaration of classes
class dictionary;
class objectRegistry;
class mapPolyMesh;
class polyMesh;

class calculateGlobalVariables
{
    //- Disallow default bitwise copy construct
    calculateGlobalVariables(const calculateGlobalVariables&);

    //- Disallow default bitwise assignment
    void operator=(const calculateGlobalVariables&);

    const objectRegistry& obr_;

   //- the expression driver
    autoPtr<CommonValueExpressionDriver> driver_;

    //- the global namespace the variables should go to
    word toGlobalNamespace_;

    //- list of variables that should go to the global namespace
    wordList toGlobalVariables_;

    //- whether or not to reset the variable
    bool noReset_;

protected:

    void executeAndWriteToGlobal();

public:

    //- Runtime type information
    TypeName("calculateGlobalVariables");


    // Constructors

    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    calculateGlobalVariables
    (
        const word& name,
        const objectRegistry&,
        const dictionary&,
        const bool loadFromFiles = false
    );


    //- Destructor
    virtual ~calculateGlobalVariables();


    // Member Functions

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
