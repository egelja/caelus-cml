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
    executeIfFunctionObjectPresentFunctionObject

Description
    This proxy-functionObject only executs its childrens if a specific 
    functionObject is present


SourceFiles
    executeIfFunctionObjectPresentFunctionObject.cpp

Contributors/Copyright:
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef executeIfFunctionObjectPresentFunctionObject_H
#define executeIfFunctionObjectPresentFunctionObject_H

#include "conditionalFunctionObjectListProxy.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class executeIfFunctionObjectPresentFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class executeIfFunctionObjectPresentFunctionObject
:
    public conditionalFunctionObjectListProxy
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    executeIfFunctionObjectPresentFunctionObject(const executeIfFunctionObjectPresentFunctionObject&);
    
    //- Disallow default bitwise assignment
    void operator=(const executeIfFunctionObjectPresentFunctionObject&);

    //- Name of the functionObject
    word functionObjectName_;

protected:
    
    //- the actual conition that determines whether the FO should be executed
    virtual bool condition();

public:

    //- Runtime type information
    TypeName("executeIfFunctionObjectPresent");


    // Constructors

    //- Construct from components
    executeIfFunctionObjectPresentFunctionObject
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
