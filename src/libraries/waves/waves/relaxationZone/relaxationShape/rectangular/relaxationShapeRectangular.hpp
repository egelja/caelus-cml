/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
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
    CML::relaxationShapes::relaxationShapeRectangular

Description
    An object used to define RECTANGULAR relaxation zones, which can be
    arbitrarily orientated in space. See e.g.

    @article { jacobsenFuhrmanFredsoe2011,
        Author = {Jacobsen, N G and Fuhrman, D R and Freds\o{}e, J},
        title = {{A Wave Generation Toolbox for the Open-Source CFD Library: OpenFoam\textregistered{}}},
        Journal = {{Int. J. for Numer. Meth. Fluids}},
        Year = {2012},
        Volume = {70},
        Number = {9},
        Pages = {1073-1088},
        DOI = {{10.1002/fld.2726}},
    }

SourceFiles
    relaxationShapeRectangular.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.

\*---------------------------------------------------------------------------*/

#ifndef relaxationShapeRectangular_HPP
#define relaxationShapeRectangular_HPP

#include "relaxationShape.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationShapes
{

/*---------------------------------------------------------------------------*\
                           Class relaxationShapeRectangular Declaration
\*---------------------------------------------------------------------------*/

class relaxationShapeRectangular
:
    public relaxationShape
{

protected:

    // Protected data
        vectorField cornerNodes_;
        vector      orient_;
        vector      crossOrient_;
        vector      xAxis_; // The 'x-axis in the rotated coordinate system'
        vector      yAxis_; // The 'y-axis in the rotated coordinate system'

        scalar      width_;


    // Protected member functions
        bool insideZone
        (
            const label&
        ) const;

        virtual void computeSigmaCoordinate();

        virtual void findComputationalCells();

public:

    //- Runtime type information
    TypeName("relaxationShapeRectangular");

    // Constructors

        //- from components
        relaxationShapeRectangular
        (
            const word& ,
            const fvMesh& mesh_
        );


    // Destructor

        ~relaxationShapeRectangular()
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
