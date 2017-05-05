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
    writeAndEndFunctionObject

Description
    General function object for ending a run according to a condition

SourceFiles
    writeAndEndFunctionObject.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef writeAndEndFunctionObject_H
#define writeAndEndFunctionObject_H

#include "simpleFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class writeAndEndFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class writeAndEndFunctionObject
:
    public simpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    writeAndEndFunctionObject(const writeAndEndFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const writeAndEndFunctionObject&);

    //- flag if the run is already stopped
    bool isStopped_;

protected:

    //- do the actual checking
    virtual bool endRunNow() = 0;

    bool isStopped() const { return isStopped_; }

public:

    //- Runtime type information
    TypeName("writeAndEnd");


    // Constructors

    //- Construct from components
    writeAndEndFunctionObject
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
