/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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
    CML::featureEdgeMesh

Description
    edgeMesh + IO.

    See also extendedFeatureEdgeMesh type which stores additional classification
    of features.

SourceFiles
    featureEdgeMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef featureEdgeMesh_H
#define featureEdgeMesh_H

#include "edgeMesh.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class featureEdgeMesh Declaration
\*---------------------------------------------------------------------------*/

class featureEdgeMesh
:
    public regIOobject,
    public edgeMesh
{

public:

    TypeName("featureEdgeMesh");


    // Constructors

        //- Construct (read) given an IOobject
        featureEdgeMesh(const IOobject&);

        //- Construct from featureEdgeMesh data
        featureEdgeMesh
        (
            const IOobject&,
            const pointField&,
            const edgeList&
        );

        //- Construct as copy
        featureEdgeMesh(const IOobject&, const featureEdgeMesh&);


        //- ReadData function required for regIOobject read operation
        virtual bool readData(Istream&);

        //- WriteData function required for regIOobject write operation
        virtual bool writeData(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
