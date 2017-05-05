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
    executeIfEnvironmentVariableFunctionObject

Description
    This proxy-functionObject only executs its childrens if an environment
    variable fits a certain condition:
    - exists
    - does not exist
    - fits a regular expression

SourceFiles
    executeIfEnvironmentVariableFunctionObject.cpp

Contributors/Copyright:
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef executeIfEnvironmentVariableFunctionObject_H
#define executeIfEnvironmentVariableFunctionObject_H

#include "conditionalFunctionObjectListProxy.hpp"
#include "regExp.hpp"
#include "NamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class executeIfEnvironmentVariableFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class executeIfEnvironmentVariableFunctionObject
:
    public conditionalFunctionObjectListProxy
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    executeIfEnvironmentVariableFunctionObject(const executeIfEnvironmentVariableFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const executeIfEnvironmentVariableFunctionObject&);

    //- Name of the variable
    word variableName_;

    //- the regular expression against which the content of the environment variable should be matched
    regExp contentRegexp_;

    //- reread the data
    void readData(const dictionary& dict);

public:
    // some compilers need that
    enum fitVariableMode {
        fvmExists,
        fvmDoesNotExist,
        fvmFitsRegexp
    };

private:

    static const NamedEnum<fitVariableMode,3> fitVariableModeNames_;

    //- the mode with which the variable should be fitted
    fitVariableMode fitVariableMode_;
protected:

    //- the actual conition that determines whether the FO should be executed
    virtual bool condition();

public:

    //- Runtime type information
    TypeName("executeIfEnvironmentVariable");


    // Constructors

    //- Construct from components
    executeIfEnvironmentVariableFunctionObject
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
