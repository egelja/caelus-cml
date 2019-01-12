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
    CML::fv::snGradScheme

Description
    Abstract base class for snGrad schemes.


\*---------------------------------------------------------------------------*/

#ifndef snGradScheme_H
#define snGradScheme_H

#include "tmp.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                 Class snGradScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class snGradScheme
:
    public refCount
{
    // Private data

        //- Hold reference to mesh
        const fvMesh& mesh_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const snGradScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            snGradScheme,
            Mesh,
            (const fvMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        snGradScheme(const fvMesh& mesh)
        :
            mesh_(mesh)
        {}


    // Selectors

        //- Return new tmp interpolation scheme
        static tmp<snGradScheme<Type> > New
        (
            const fvMesh& mesh,
            Istream& schemeData
        );


    //- Destructor
    virtual ~snGradScheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }


        //- Return the snGrad of the given cell field with the given deltaCoeffs
        static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        snGrad
        (
            const GeometricField<Type, fvPatchField, volMesh>&,
            const tmp<surfaceScalarField>&,
            const word& snGradName = "snGrad"
        );

        //- Return the interpolation weighting factors for the given field
        virtual tmp<surfaceScalarField> deltaCoeffs
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const = 0;

        //- Return true if this scheme uses an explicit correction
        virtual bool corrected() const
        {
            return false;
        }

        //- Return the explicit correction to the snGrad
        //  for the given field
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        correction(const GeometricField<Type, fvPatchField, volMesh>&) const
        {
            return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >(nullptr);
        }

        //- Return the snGrad of the given cell field
        //  with explicit correction
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        snGrad(const GeometricField<Type, fvPatchField, volMesh>&) const;

        //- Return the snGrad of the given tmp cell field
        //  with explicit correction
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        snGrad
        (
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeSnGradTypeScheme(SS, Type)                                         \
    defineNamedTemplateTypeNameAndDebug(CML::fv::SS<CML::Type>, 0);            \
                                                                               \
    namespace CML                                                              \
    {                                                                          \
        namespace fv                                                           \
        {                                                                      \
            snGradScheme<Type>::addMeshConstructorToTable<SS<Type> >           \
                add##SS##Type##MeshConstructorToTable_;                        \
        }                                                                      \
    }

#define makeSnGradScheme(SS)                                                   \
                                                                               \
makeSnGradTypeScheme(SS, scalar)                                               \
makeSnGradTypeScheme(SS, vector)                                               \
makeSnGradTypeScheme(SS, sphericalTensor)                                      \
makeSnGradTypeScheme(SS, symmTensor)                                           \
makeSnGradTypeScheme(SS, tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fv.hpp"
#include "snGradScheme.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "HashTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
tmp<snGradScheme<Type> > snGradScheme<Type>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "snGradScheme<Type>::New(const fvMesh&, Istream&)"
               " : constructing snGradScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "Discretisation scheme not specified"
            << endl << endl
            << "Valid schemes are :" << endl
            << MeshConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename MeshConstructorTable::iterator constructorIter =
        MeshConstructorTablePtr_->find(schemeName);

    if (constructorIter == MeshConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown discretisation scheme "
            << schemeName << nl << nl
            << "Valid schemes are :" << endl
            << MeshConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return constructorIter()(mesh, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
snGradScheme<Type>::~snGradScheme()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
snGradScheme<Type>::snGrad
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const tmp<surfaceScalarField>& tdeltaCoeffs,
    const word& snGradName
)
{
    const fvMesh& mesh = vf.mesh();

    // construct GeometricField<Type, fvsPatchField, surfaceMesh>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                snGradName + "("+vf.name()+')',
                vf.instance(),
                vf.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            vf.dimensions()*tdeltaCoeffs().dimensions()
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& ssf = tsf();

    // set reference to difference factors array
    const scalarField& deltaCoeffs = tdeltaCoeffs().internalField();

    // owner/neighbour addressing
    const labelUList& owner = mesh.owner();
    const labelUList& neighbour = mesh.neighbour();

    forAll(owner, faceI)
    {
        ssf[faceI] =
            deltaCoeffs[faceI]*(vf[neighbour[faceI]] - vf[owner[faceI]]);
    }

    forAll(vf.boundaryField(), patchI)
    {
        const fvPatchField<Type>& pvf = vf.boundaryField()[patchI];

        if (pvf.coupled())
        {
            ssf.boundaryField()[patchI] =
                pvf.snGrad(tdeltaCoeffs().boundaryField()[patchI]);
        }
        else
        {
            ssf.boundaryField()[patchI] = pvf.snGrad();
        }
    }

    return tsf;
}


//- Return the face-snGrad of the given cell field
//  with explicit correction
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
snGradScheme<Type>::snGrad
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
        = snGrad(vf, deltaCoeffs(vf));

    if (corrected())
    {
        tsf() += correction(vf);
    }

    return tsf;
}


//- Return the face-snGrad of the given cell field
//  with explicit correction
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
snGradScheme<Type>::snGrad
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
) const
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tinterpVf
        = snGrad(tvf());
    tvf.clear();
    return tinterpVf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
