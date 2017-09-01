/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
Copyright (C) 2017 Applied CCM Pty Ltd 
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
    CML::relaxationSchemes::relaxationSchemeSpatialNoAlpha

Description
    An object which results in relaxation, which is only dependent on the
    SpatialNoAlpha location within a relaxationShape.

SourceFiles
    relaxationSchemeSpatialNoAlpha.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.

\*---------------------------------------------------------------------------*/

#ifndef relaxationSchemeSpatialNoAlpha_HPP
#define relaxationSchemeSpatialNoAlpha_HPP

#include "relaxationScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationSchemes
{

/*---------------------------------------------------------------------------*\
                           Class relaxationSchemeSpatialNoAlpha Declaration
\*---------------------------------------------------------------------------*/

class relaxationSchemeSpatialNoAlpha
:
    public relaxationScheme
{

protected:

    // Protected data
        scalar exponent_;
        scalarField weight_;

    // Protected member functions


public:

    //- Runtime type information
    TypeName("relaxationSchemeSpatialNoAlpha");

    // Constructors

        //- from components
        relaxationSchemeSpatialNoAlpha
        (
            const word& ,
            const fvMesh& mesh_,
            vectorField& U,
            scalarField& alpha
        );

    // Destructor

        virtual ~relaxationSchemeSpatialNoAlpha()
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
