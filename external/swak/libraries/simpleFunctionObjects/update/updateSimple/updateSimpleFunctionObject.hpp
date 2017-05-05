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
    updateSimpleFunctionObject

Description
    A function object base class for initializing or updating fields

SourceFiles
    updateSimpleFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef updateSimpleFunctionObject_H
#define updateSimpleFunctionObject_H

#include "simpleFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class updateSimpleFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class updateSimpleFunctionObject
:
    public simpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    updateSimpleFunctionObject(const updateSimpleFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const updateSimpleFunctionObject&);

    //- do this at startup or every time
    bool onlyAtStartup_;

    //- only do if this isn't a restart
    bool runIfStartTime_;

protected:
    //- do the actual recalculation
    virtual void recalc() = 0;

public:

    //- Runtime type information
    TypeName("updateSimple");

    // Constructors

    //- Construct from components
    updateSimpleFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();

    void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
