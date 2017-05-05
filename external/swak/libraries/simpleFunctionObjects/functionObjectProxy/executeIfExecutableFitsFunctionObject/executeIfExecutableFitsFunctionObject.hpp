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
    executeIfExecutableFitsFunctionObject

Description
    This proxy-functionObject only executs its childrens if the name of the 
    current executable fits a regular expression


SourceFiles
    executeIfExecutableFitsFunctionObject.cpp

Contributors/Copyright:
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef executeIfExecutableFitsFunctionObject_H
#define executeIfExecutableFitsFunctionObject_H

#include "conditionalFunctionObjectListProxy.hpp"
#include "regExp.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class executeIfExecutableFitsFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class executeIfExecutableFitsFunctionObject
:
    public conditionalFunctionObjectListProxy
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    executeIfExecutableFitsFunctionObject(const executeIfExecutableFitsFunctionObject&);
    
    //- Disallow default bitwise assignment
    void operator=(const executeIfExecutableFitsFunctionObject&);

    //- the regular expression against which the executable name should be matched
    regExp executableNameRegexp_;

    //- reread the regular expression
    void readRegexp(const dictionary& dict);

    //- Name of the executable
    word executable_;

protected:
    
    //- the actual conition that determines whether the FO should be executed
    virtual bool condition();

public:

    //- Runtime type information
    TypeName("executeIfExecutableFits");


    // Constructors

    //- Construct from components
    executeIfExecutableFitsFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool read(const dictionary& dict);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
