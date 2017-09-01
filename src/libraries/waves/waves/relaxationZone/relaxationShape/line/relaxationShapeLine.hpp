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
    CML::relaxationShapes::relaxationShapeLine

Description
    A class derived from the rectangular relaxation shape, but it also
    implements an interpolation routine along a line, such that the surface 
    elevation can be pre-computed on this line prior to the evaluation of the
    surface-cell intersections.

SourceFiles
    relaxationShapeLine.cpp

Author
    Niels Gjøl Jacobsen, Deltares.

\*---------------------------------------------------------------------------*/

#ifndef relaxationShapeLine_HPP
#define relaxationShapeLine_HPP

#include "relaxationShapeRectangular.hpp"
#include "interpolateXY.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationShapes
{

/*---------------------------------------------------------------------------*\
                           Class relaxationShapeLine Declaration
\*---------------------------------------------------------------------------*/

class relaxationShapeLine
:
    public relaxationShapeRectangular
{

protected:

    // Protected data
        label N_;
        scalarField dataRange_;
        scalarField localCoordinates_;
        pointField globalCoordinates_;

        label updated_;

    // Protected member functions
        void dataRange();

        scalar localCoordinate(const point&) const;

        point globalCoordinate(const scalar&) const;

        void localCoordinates();

        void globalCoordinates();

public:

    //- Runtime type information
    TypeName("relaxationShapeLine");

    // Constructors

        //- from components
        relaxationShapeLine
        (
            const word& ,
            const fvMesh& mesh_
        );


    // Destructor

        ~relaxationShapeLine()
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
