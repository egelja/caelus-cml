/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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
    CML::fv::secondOrderCorrectedSnGrad

Description
    Simple central-difference snGrad scheme with non-orthogonal correction.
	Note: This scheme should only be used for wall resolved meshes.

Author
    Darrin Stephens 

\*---------------------------------------------------------------------------*/

#ifndef secondOrderCorrectedSnGrad_H
#define secondOrderCorrectedSnGrad_H

#include "snGradScheme.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "linear.hpp"
#include "fvcGrad.hpp"
#include "gaussGrad.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                 Class secondOrderCorrectedSnGrad Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class secondOrderCorrectedSnGrad
:
    public snGradScheme<Type>
{
    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const secondOrderCorrectedSnGrad&);


public:

    //- Runtime type information
    TypeName("secondOrderCorrected");


    // Constructors

        //- Construct from mesh
        secondOrderCorrectedSnGrad(const fvMesh& mesh)
        :
            snGradScheme<Type>(mesh)
        {}


        //- Construct from mesh and data stream
        secondOrderCorrectedSnGrad(const fvMesh& mesh, Istream&)
        :
            snGradScheme<Type>(mesh)
        {}


    //- Destructor
    virtual ~secondOrderCorrectedSnGrad();


    // Member Functions

        //- Return the interpolation weighting factors for the given field
        virtual tmp<surfaceScalarField> deltaCoeffs
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const
        {
            return this->mesh().nonOrthDeltaCoeffs();
        }

        //- Return true if this scheme uses an explicit correction
        virtual bool corrected() const
        {
            return true;
        }

        //- Return the explicit correction to the secondOrderCorrectedSnGrad
        //  for the given field using the gradient of the field
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        fullGradCorrection
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;

        //- Return the explicit correction to the secondOrderCorrectedSnGrad
        //  for the given field using the gradients of the field components
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        correction(const GeometricField<Type, fvPatchField, volMesh>&) const;
};


// * * * * * * * * Template Member Function Specialisations  * * * * * * * * //

template<>
tmp<surfaceScalarField> secondOrderCorrectedSnGrad<scalar>::correction
(
    const volScalarField& vsf
) const;


template<>
tmp<surfaceVectorField> secondOrderCorrectedSnGrad<vector>::correction
(
    const volVectorField& vvf
) const;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::fv::secondOrderCorrectedSnGrad<Type>::~secondOrderCorrectedSnGrad()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >
CML::fv::secondOrderCorrectedSnGrad<Type>::fullGradCorrection
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    const fvMesh& mesh = this->mesh();

    // Make a copy of the nonOrthoCorrection vector as we need to change boundaryField
    tmp<surfaceVectorField> nonOrthCorrectionVectorsCP = mesh.nonOrthCorrectionVectors();

    forAll(vf.boundaryField(), patchi)
    {
        fvsPatchVectorField& patchCorrVecs = nonOrthCorrectionVectorsCP().boundaryField()[patchi];

        if (vf.boundaryField()[patchi].fixesValue())
        {
		    // For fixedValue boundary conditions set the boundaryField values to the correctly
			// calculated quantities
		    patchCorrVecs = mesh.fvNonOrthCorrectionVectors()[patchi];
        }
    }
		
    // construct GeometricField<Type, fvsPatchField, surfaceMesh>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tssf =

        linear<typename outerProduct<vector, Type>::type>(mesh).dotInterpolate
        (
            nonOrthCorrectionVectorsCP(),
            gradScheme<Type>::New
            (
                mesh,
                mesh.gradScheme("grad(" + vf.name() + ')')
            )().grad(vf, "grad(" + vf.name() + ')')
        );
    tssf().rename("snGradCorr(" + vf.name() + ')');

    return tssf;
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >
CML::fv::secondOrderCorrectedSnGrad<Type>::correction
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    const fvMesh& mesh = this->mesh();

    // construct GeometricField<Type, fvsPatchField, surfaceMesh>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tssf
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "snGradCorr("+vf.name()+')',
                vf.instance(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            vf.dimensions()*mesh.nonOrthDeltaCoeffs().dimensions()
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& ssf = tssf();

    for (direction cmpt = 0; cmpt < pTraits<Type>::nComponents; cmpt++)
    {
        ssf.replace
        (
            cmpt,
            linear
            <
                typename
                outerProduct<vector, typename pTraits<Type>::cmptType>::type
            >(mesh).dotInterpolate
            (
                mesh.nonOrthCorrectionVectors(),
                gradScheme<typename pTraits<Type>::cmptType>::New
                (
                    mesh,
                    mesh.gradScheme("grad(" + ssf.name() + ')')
                )()
                //gaussGrad<typename pTraits<Type>::cmptType>(mesh)
               .grad(vf.component(cmpt))
            )
        );
    }

    return tssf;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
