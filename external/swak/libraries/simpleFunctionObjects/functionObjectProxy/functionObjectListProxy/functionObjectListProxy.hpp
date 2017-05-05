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
    functionObjectListProxy

Description
    Basis for the other function objects here. Evaluates the most common options

SourceFiles
    functionObjectListProxy.cpp

Contributors/Copyright:
    2011-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef functionObjectListProxy_H
#define functionObjectListProxy_H

#include "functionObjectList.hpp"
#include "simpleFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class functionObjectListProxy Declaration
\*---------------------------------------------------------------------------*/

class functionObjectListProxy
:
    public simpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    functionObjectListProxy(const functionObjectListProxy&);

    //- Disallow default bitwise assignment
    void operator=(const functionObjectListProxy&);

protected:

    //- the actual function Objects that this encapsulates
    autoPtr<functionObjectList> functions_;

    //- get the functionObject list (read if not yet done so)
    functionObjectList &functions();

    //- initialize the function-objects
    virtual void initFunctions();

public:

    //- Runtime type information
    TypeName("functionObjectListProxy");


    // Constructors

    //- Construct from components
    functionObjectListProxy
    (
        const word&,
        const Time&,
        const dictionary&,
        // to switch off for FO that would require a virtual method
        bool allowReadingDuringConstruction=true
    );

    // Member Functions

    //- start is called at the start of the time-loop
    virtual bool start();

    //- execute is called at each ++ or += of the time-loop
    virtual bool execute(const bool forceWrite);

    //- Called when Time::run() determines that the time-loop exits.
    //  By default it simply calls execute().
    virtual bool end();

    //- Read and set the function object if its data have changed
    virtual bool read(const dictionary&);

    //- write from the simpleFunctionObjects
    virtual void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
