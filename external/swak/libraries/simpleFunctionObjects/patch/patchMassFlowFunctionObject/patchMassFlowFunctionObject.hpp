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
    patchMassFlowFunctionObject

Description
    Calculates the mass flow on the patches

SourceFiles
    patchMassFlowFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef patchMassFlowFunctionObject_H
#define patchMassFlowFunctionObject_H

#include "patchFunctionObject.hpp"
#include "SolverInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class patchMassFlowFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class patchMassFlowFunctionObject
:
    public patchFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    patchMassFlowFunctionObject(const patchMassFlowFunctionObject&);
    
    //- Disallow default bitwise assignment
    void operator=(const patchMassFlowFunctionObject&);

    SolverInfo solver_;

protected:

    //- Names of the files
    virtual wordList fileNames();

    word dirName();

public:

    //- Runtime type information
    TypeName("patchMassFlow");


    // Constructors

    //- Construct from components
    patchMassFlowFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );


    // Member Functions

    //- Sample and write
    void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
