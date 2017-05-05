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
    recalcPhiFunctionObject

Description
    Recalculate phi according to the current velocity and density

SourceFiles
    recalcPhiFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef recalcPhiFunctionObject_H
#define recalcPhiFunctionObject_H

#include "updateSimpleFunctionObject.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class recalcPhiFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class recalcPhiFunctionObject
:
    public updateSimpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    recalcPhiFunctionObject(const recalcPhiFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const recalcPhiFunctionObject&);

    //- name of the velocity field
    word UName_;

    //- name of the phi field
    word phiName_;

    //- name of the pressure field
    word pName_;

    //- name of the density field (if needed)
    word rhoName_;

    //- write the fields?
    bool writeFields_;

    //- write the old fields?
    bool writeOldFields_;

protected:
    //- do the actual phi calculation
    void recalc();

public:

    //- Runtime type information
    TypeName("recalcPhi");

    // Constructors

    //- Construct from components
    recalcPhiFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
