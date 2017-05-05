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

Namespace
    CML::limitFuncs

Description
    Namespace for limiting functions


Class
    CML::limitFuncs::LimitFuncs

Description
    Class to create NVD/TVD limited weighting-factors.

    The particular differencing scheme class is supplied as a template
    argument, the weight function of which is called by the weight function
    of this class for the internal faces as well as faces of coupled
    patches (e.g. processor-processor patches). The weight function is
    supplied the central-differencing weighting factor, the face-flux, the
    cell and face gradients (from which the normalised variable
    distribution may be created) and the cell centre distance.

    This code organisation is both neat and efficient, allowing for
    convenient implementation of new schemes to run on parallelised cases.


\*---------------------------------------------------------------------------*/

#ifndef LimitFuncs_H
#define LimitFuncs_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace limitFuncs
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
class null
{
public:

    null()
    {}

    inline tmp<GeometricField<Type, fvPatchField, volMesh> > operator()
    (
        const GeometricField<Type, fvPatchField, volMesh>& phi
    ) const
    {
        return phi;
    }
};


template<class Type>
class magSqr
{
public:

    magSqr()
    {}

    inline tmp<volScalarField> operator()
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};


template<class Type>
class rhoMagSqr
{
public:

    rhoMagSqr()
    {}

    inline tmp<volScalarField> operator()
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace limitFuncs
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace limitFuncs
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
inline tmp<volScalarField> magSqr<Type>::operator()
(
    const GeometricField<Type, fvPatchField, volMesh>& phi
) const
{
    return CML::magSqr(phi);
}


template<>
inline tmp<volScalarField> magSqr<scalar>::operator()
(
    const volScalarField& phi
) const
{
    return phi;
}


template<>
inline tmp<volScalarField> magSqr<tensor>::operator()
(
    const volTensorField& phi
) const
{
    return CML::tr(phi);
}


template<class Type>
inline tmp<volScalarField> rhoMagSqr<Type>::operator()
(
    const GeometricField<Type, fvPatchField, volMesh>& phi
) const
{
    const volScalarField& rho =
        phi.db().objectRegistry::template lookupObject<volScalarField>("rho");
    return CML::magSqr(phi/rho);
}


template<>
inline tmp<volScalarField> rhoMagSqr<scalar>::operator()
(
    const volScalarField& phi
) const
{
    const volScalarField& rho =
        phi.db().objectRegistry::lookupObject<volScalarField>("rho");
    return phi/rho;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace limitFuncs
} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
