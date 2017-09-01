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
    CML::relaxationSchemes::relaxationSchemeSpatialInterpolation

Description
    This relaxation scheme utilises interpolation over a pre-defined interval
    for the intersection. This speed of the execution of the intersections 
    with a huge factor. Especially when using irregular waves with a lot of 
    wave components.
    Should be combined with a relaxation shape that implements the interpolation
    method.

SourceFiles
    relaxationSchemeSpatialInterpolation.cpp

Author
    Niels Gjøl Jacobsen, Deltares.

\*---------------------------------------------------------------------------*/

#ifndef relaxationSchemeSpatialInterpolation_HPP
#define relaxationSchemeSpatialInterpolation_HPP

#include "relaxationScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationSchemes
{

/*---------------------------------------------------------------------------*\
            Class relaxationSchemeSpatialInterpolation Declaration
\*---------------------------------------------------------------------------*/

class relaxationSchemeSpatialInterpolation
:
    public relaxationScheme
{

protected:

    // Protected data
        scalar exponent_;
        scalarField weight_;

        scalarField surfaceElevation_;

    // Protected member functions

        //- Returns a scalarField of the signed distance to an arbitrary
        //  surface
        virtual void signedPointToSurfaceDistance
        (
            const pointField&,
            scalarField&
        );


        //- Returns a scalar of the signed distance to an arbitrary surface
        virtual scalar signedPointToSurfaceDistance
        (
            const point&
        ) const;

public:

    //- Runtime type information
    TypeName("relaxationSchemeSpatialInterpolation");

    // Constructors

        //- from components
        relaxationSchemeSpatialInterpolation
        (
            const word& ,
            const fvMesh& mesh_,
            vectorField& U,
            scalarField& alpha
        );

    // Destructor

        virtual ~relaxationSchemeSpatialInterpolation()
        {}

    // Member Functions
        virtual void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationSchemes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
