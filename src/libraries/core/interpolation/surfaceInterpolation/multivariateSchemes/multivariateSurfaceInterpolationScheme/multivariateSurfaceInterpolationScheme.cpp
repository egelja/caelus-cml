/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

Description
    Abstract base class for surface interpolation schemes.

\*---------------------------------------------------------------------------*/

#include "fv.hpp"
#include "multivariateSurfaceInterpolationScheme.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from face-flux field and coefficient
template<class Type>
multivariateSurfaceInterpolationScheme<Type>::
multivariateSurfaceInterpolationScheme
(
    const fvMesh& mesh,
	// CAELUSFIX added typename
    const typename multivariateSurfaceInterpolationScheme<Type>::fieldTable& vtfs,
    const surfaceScalarField&,
    Istream&
)
:
    mesh_(mesh),
    fields_(vtfs)
{}


// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

// Return weighting factors for scheme given by name in dictionary
template<class Type>
tmp<multivariateSurfaceInterpolationScheme<Type> >
multivariateSurfaceInterpolationScheme<Type>::New
(
    const fvMesh& mesh,
	// CAELUSFIX added typename
    const typename multivariateSurfaceInterpolationScheme<Type>::fieldTable& vtfs,
    const surfaceScalarField& faceFlux,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "multivariateSurfaceInterpolationScheme<Type>::New"
               "(const fvMesh& mesh, const fieldTable&, "
               "const surfaceScalarField&, Istream&) : "
               "constructing surfaceInterpolationScheme<Type>"
            << endl;
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator constructorIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (constructorIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown discretisation scheme " << schemeName << nl << nl
            << "Valid schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return constructorIter()(mesh, vtfs, faceFlux, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
multivariateSurfaceInterpolationScheme<Type>::
~multivariateSurfaceInterpolationScheme()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
