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
    CML::multivariateSelectionScheme

Description
    Generic multi-variate discretisation scheme class for which any of the
    NVD, CNVD or NVDV schemes may be selected for each variable.

SourceFiles
    multivariateSelectionScheme.cpp

\*---------------------------------------------------------------------------*/

#ifndef multivariateSelectionScheme_H
#define multivariateSelectionScheme_H

#include "multivariateSurfaceInterpolationScheme.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class multivariateSelectionScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class multivariateSelectionScheme
:
    public multivariateSurfaceInterpolationScheme<Type>
{
    // Private data

        dictionary schemes_;
        const surfaceScalarField& faceFlux_;
        surfaceScalarField weights_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        multivariateSelectionScheme(const multivariateSelectionScheme&);

        //- Disallow default bitwise assignment
        void operator=(const multivariateSelectionScheme&);


public:

    //- Runtime type information
    TypeName("multivariateSelection");


    // Constructors

        //- Construct for field, faceFlux and Istream
        multivariateSelectionScheme
        (
            const fvMesh& mesh,
            const typename multivariateSurfaceInterpolationScheme<Type>::
                fieldTable& fields,
            const surfaceScalarField& faceFlux,
            Istream& schemeData
        );


    // Member Operators

        //- surfaceInterpolationScheme sub-class returned by operator(field)
        //  which is used as the interpolation scheme for the field
        class fieldScheme
        :
            public multivariateSurfaceInterpolationScheme<Type>::fieldScheme
        {
            // Private data

                const surfaceScalarField& weights_;

        public:

            // Constructors

                fieldScheme
                (
                    const GeometricField<Type, fvPatchField, volMesh>& field,
                    const surfaceScalarField& weights
                )
                :
                    multivariateSurfaceInterpolationScheme<Type>::
                        fieldScheme(field),
                    weights_(weights)
                {}


            // Member Functions

                //- Return the interpolation weighting factors
                tmp<surfaceScalarField> weights
                (
                    const GeometricField<Type, fvPatchField, volMesh>&
                ) const
                {
                    return weights_;
                }
        };

        tmp<surfaceInterpolationScheme<Type> > operator()
        (
            const GeometricField<Type, fvPatchField, volMesh>& field
        ) const
        {
            return tmp<surfaceInterpolationScheme<Type> >
            (
                new fieldScheme(field, weights_)
            );
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "limitedSurfaceInterpolationScheme.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "upwind.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
CML::multivariateSelectionScheme<Type>::multivariateSelectionScheme
(
    const fvMesh& mesh,
    const typename multivariateSurfaceInterpolationScheme<Type>::
        fieldTable& fields,
    const surfaceScalarField& faceFlux,
    Istream& schemeData
)
:
    multivariateSurfaceInterpolationScheme<Type>
    (
        mesh,
        fields,
        faceFlux,
        schemeData
    ),
    schemes_(schemeData),
    faceFlux_(faceFlux),
    weights_
    (
        IOobject
        (
            "multivariateWeights",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        dimless
    )
{
    typename multivariateSurfaceInterpolationScheme<Type>::
        fieldTable::const_iterator iter = this->fields().begin();

    surfaceScalarField limiter
    (
        limitedSurfaceInterpolationScheme<Type>::New
        (
            mesh,
            faceFlux_,
            schemes_.lookup(iter()->name())
        )().limiter(*iter())
    );

    for (++iter; iter != this->fields().end(); ++iter)
    {
        limiter = min
        (
            limiter,
            limitedSurfaceInterpolationScheme<Type>::New
            (
                mesh,
                faceFlux_,
                schemes_.lookup(iter()->name())
            )().limiter(*iter())
        );
    }

    weights_ =
        limiter*mesh.surfaceInterpolation::weights()
      + (scalar(1) - limiter)*upwind<Type>(mesh, faceFlux_).weights();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
