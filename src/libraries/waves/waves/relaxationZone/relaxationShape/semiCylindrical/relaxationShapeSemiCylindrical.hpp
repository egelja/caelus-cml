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
    CML::relaxationShapes::relaxationShapeSemiCylindrical

Description
    An object used to define SEMICYLINDRICAL relaxation zones. See e.g.

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
    relaxationShapeSemiCylindrical.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.

\*---------------------------------------------------------------------------*/

#ifndef relaxationShapeSemiCylindrical_HPP
#define relaxationShapeSemiCylindrical_HPP

#include "relaxationShape.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationShapes
{

/*---------------------------------------------------------------------------*\
                           Class relaxationShapeSemiCylindrical Declaration
\*---------------------------------------------------------------------------*/

class relaxationShapeSemiCylindrical
:
    public relaxationShape
{

protected:

    // Protected data

        vector      centre_;
        vector      zeroAngleDirection_;
        scalar      rInner_;
        scalar      rOuter_;
        scalar      angleStart_;
        scalar      angleEnd_;

        scalar      width_;
        vector      piHalfAngleDirection_;

    // Protected member functions
        bool angleCheck
        (
            const scalar&
        ) const;

        bool insideZone
        (
            const label&
        ) const;

        virtual void computeSigmaCoordinate();

        virtual void findComputationalCells();

public:

    //- Runtime type information
    TypeName("relaxationShapeSemiCylindrical");

    // Constructors

        //- from components
        relaxationShapeSemiCylindrical
        (
            const word& ,
            const fvMesh& mesh_
        );


    // Destructor

        ~relaxationShapeSemiCylindrical()
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
