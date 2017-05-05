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
    conditionalFunctionObjectListProxy

Description
    Basis for the other function objects here. Evaluates the most common options

SourceFiles
    conditionalFunctionObjectListProxy.cpp

Contributors/Copyright:
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef conditionalFunctionObjectListProxy_H
#define conditionalFunctionObjectListProxy_H

#include "functionObjectListProxy.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class conditionalFunctionObjectListProxy Declaration
\*---------------------------------------------------------------------------*/

class conditionalFunctionObjectListProxy
:
    public functionObjectListProxy
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    conditionalFunctionObjectListProxy(const conditionalFunctionObjectListProxy&);

    //- Disallow default bitwise assignment
    void operator=(const conditionalFunctionObjectListProxy&);

    autoPtr<functionObjectListProxy> else_;

    autoPtr<dictionary> elseDict_;

    //- read the else-part
    void readElse(const dictionary &dict);
protected:

    //- the actual conition that determines whether the FO should be executed
    virtual bool condition()=0;

public:

    //- Runtime type information
    TypeName("conditionalFunctionObjectListProxy");


    // Constructors

    //- Construct from components
    conditionalFunctionObjectListProxy
    (
        const word&,
        const Time&,
        const dictionary&
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
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
