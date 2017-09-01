/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Deltares.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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
    CML::relaxationShapes::relaxationShapeFrozen

Description
    This frozen-type relaxation zone is a wrapper relaxationZone for all of the
    other relaxation zones, but it does not update the relaxationZone coeffi-
    cients on moving meshes.
    For certain applications this is desired over the updating, and rather than
    adding an if-sentence to be evaluated at rather low level, this wrapper was
    developed.

SourceFiles
    relaxationShapeFrozen.cpp

Author
    Niels Gjøl Jacobsen, Deltares.

\*---------------------------------------------------------------------------*/

#ifndef relaxationShapeFrozen_HPP
#define relaxationShapeFrozen_HPP

#include "relaxationShape.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationShapes
{

/*---------------------------------------------------------------------------*\
                           Class relaxationShapeFrozen Declaration
\*---------------------------------------------------------------------------*/

class relaxationShapeFrozen
:
    public relaxationShape
{

protected:

    // Protected data

    // Protected member functions
        virtual void computeSigmaCoordinate();

        virtual void findComputationalCells();

public:

    //- Runtime type information
    TypeName("relaxationShapeFrozen");

    // Constructors

        //- from components
        relaxationShapeFrozen
        (
            const word& ,
            const fvMesh& mesh_
        );


    // Destructor

        ~relaxationShapeFrozen()
        {}


    // Member Functions
        virtual const pointField& pointSet();

        virtual scalar interpolation(const scalarField&, const point&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationShapes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
