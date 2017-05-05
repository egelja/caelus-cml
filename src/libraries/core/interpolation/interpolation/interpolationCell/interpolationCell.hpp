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
    CML::interpolationCell

Description
    Uses the cell value for any point in the cell

\*---------------------------------------------------------------------------*/

#ifndef interpolationCell_H
#define interpolationCell_H

#include "interpolation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;

/*---------------------------------------------------------------------------*\
                           Class interpolationCell Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class interpolationCell
:
    public interpolation<Type>
{

public:

    //- Runtime type information
    TypeName("cell");


    // Constructors

        //- Construct from components
        interpolationCell
        (
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );


    // Member Functions

        //- Interpolate field to the given point in the given cell
        Type interpolate
        (
            const vector& position,
            const label cellI,
            const label faceI = -1
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

template<class Type>
interpolationCell<Type>::interpolationCell
(
    const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
    interpolation<Type>(psi)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type interpolationCell<Type>::interpolate
(
    const vector&,
    const label cellI,
    const label
) const
{
    return this->psi_[cellI];
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
