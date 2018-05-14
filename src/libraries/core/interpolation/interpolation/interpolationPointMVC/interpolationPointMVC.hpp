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
    CML::interpolationPointMVC

Description
    Given cell centre values interpolates to vertices and uses these to
    do a Mean Value Coordinates interpolation.

\*---------------------------------------------------------------------------*/

#ifndef interpolationPointMVC_H
#define interpolationPointMVC_H

#include "interpolation.hpp"
#include "pointMVCWeight.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class interpolationPointMVC Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class interpolationPointMVC
:
    public interpolation<Type>
{
protected:

    // Protected data

        //- Interpolated volfield
        const GeometricField<Type, pointPatchField, pointMesh> psip_;


public:

    //- Runtime type information
    TypeName("pointMVC");


    // Constructors

        //- Construct from components
        interpolationPointMVC
        (
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );


    // Member Functions

        //- Inherit interpolate from interpolation
        using interpolation<Type>::interpolate;
    
        //- Interpolate field for the given cellPointWeight
        inline Type interpolate(const pointMVCWeight& cpw) const;

        //- Interpolate field to the given point in the given cell
        inline Type interpolate
        (
            const vector& position,
            const label cellI,
            const label faceI = -1
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
inline Type CML::interpolationPointMVC<Type>::interpolate
(
    const pointMVCWeight& cpw
) const
{
    return cpw.interpolate(psip_);
}


template<class Type>
inline Type CML::interpolationPointMVC<Type>::interpolate
(
    const vector& position,
    const label cellI,
    const label faceI
) const
{
    return interpolate
    (
        pointMVCWeight(this->pMesh_, position, cellI, faceI)
    );
}


#include "volPointInterpolation.hpp"

// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

template<class Type>
CML::interpolationPointMVC<Type>::interpolationPointMVC
(
    const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
    interpolation<Type>(psi),
    psip_(volPointInterpolation::New(psi.mesh()).interpolate(psi))
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
