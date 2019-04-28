/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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

Class
    CML::fv::laplacianScheme

Description
    Abstract base class for laplacian schemes.

SourceFiles
    laplacianScheme.cpp

\*---------------------------------------------------------------------------*/

#ifndef laplacianScheme_H
#define laplacianScheme_H

#include "tmp.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "linear.hpp"
#include "correctedSnGrad.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class fvMatrix;

class fvMesh;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                           Class laplacianScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class GType>
class laplacianScheme
:
    public refCount
{

protected:

    // Protected data

        const fvMesh& mesh_;
        tmp<surfaceInterpolationScheme<GType> > tinterpGammaScheme_;
        tmp<snGradScheme<Type> > tsnGradScheme_;


    // Private Member Functions

        //- Disallow copy construct
        laplacianScheme(const laplacianScheme&);

        //- Disallow default bitwise assignment
        void operator=(const laplacianScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            laplacianScheme,
            Istream,
            (const fvMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        laplacianScheme(const fvMesh& mesh)
        :
            mesh_(mesh),
            tinterpGammaScheme_(new linear<GType>(mesh)),
            tsnGradScheme_(new correctedSnGrad<Type>(mesh))
        {}

        //- Construct from mesh and Istream
        laplacianScheme(const fvMesh& mesh, Istream& is)
        :
            mesh_(mesh),
            tinterpGammaScheme_(nullptr),
            tsnGradScheme_(nullptr)
        {
            tinterpGammaScheme_ = tmp<surfaceInterpolationScheme<GType> >
            (
                surfaceInterpolationScheme<GType>::New(mesh, is)
            );

            tsnGradScheme_ = tmp<snGradScheme<Type> >
            (
                snGradScheme<Type>::New(mesh, is)
            );
        }

        //- Construct from mesh, interpolation and snGradScheme schemes
        laplacianScheme
        (
            const fvMesh& mesh,
            const tmp<surfaceInterpolationScheme<GType> >& igs,
            const tmp<snGradScheme<Type> >& sngs
        )
        :
            mesh_(mesh),
            tinterpGammaScheme_(igs),
            tsnGradScheme_(sngs)
        {}


    // Selectors

        //- Return a pointer to a new laplacianScheme created on freestore
        static tmp<laplacianScheme<Type, GType> > New
        (
            const fvMesh& mesh,
            Istream& schemeData
        );


    //- Destructor
    virtual ~laplacianScheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }

        virtual tmp<fvMatrix<Type> > fvmLaplacian
        (
            const GeometricField<GType, fvsPatchField, surfaceMesh>&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<fvMatrix<Type> > fvmLaplacian
        (
            const GeometricField<GType, fvPatchField, volMesh>&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const GeometricField<GType, fvsPatchField, surfaceMesh>&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
        (
            const GeometricField<GType, fvPatchField, volMesh>&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeFvLaplacianTypeScheme(SS, GType, Type)                             \
    typedef CML::fv::SS<CML::Type, CML::GType> SS##Type##GType;                \
    defineNamedTemplateTypeNameAndDebug(SS##Type##GType, 0);                   \
                                                                               \
    namespace CML                                                              \
    {                                                                          \
        namespace fv                                                           \
        {                                                                      \
            typedef SS<Type, GType> SS##Type##GType;                           \
                                                                               \
            laplacianScheme<Type, GType>::                                     \
                addIstreamConstructorToTable<SS<Type, GType> >                 \
                add##SS##Type##GType##IstreamConstructorToTable_;              \
        }                                                                      \
    }


#define makeFvLaplacianScheme(SS)                                              \
                                                                               \
makeFvLaplacianTypeScheme(SS, scalar, scalar)                                  \
makeFvLaplacianTypeScheme(SS, symmTensor, scalar)                              \
makeFvLaplacianTypeScheme(SS, tensor, scalar)                                  \
makeFvLaplacianTypeScheme(SS, scalar, vector)                                  \
makeFvLaplacianTypeScheme(SS, symmTensor, vector)                              \
makeFvLaplacianTypeScheme(SS, tensor, vector)                                  \
makeFvLaplacianTypeScheme(SS, scalar, sphericalTensor)                         \
makeFvLaplacianTypeScheme(SS, symmTensor, sphericalTensor)                     \
makeFvLaplacianTypeScheme(SS, tensor, sphericalTensor)                         \
makeFvLaplacianTypeScheme(SS, scalar, symmTensor)                              \
makeFvLaplacianTypeScheme(SS, symmTensor, symmTensor)                          \
makeFvLaplacianTypeScheme(SS, tensor, symmTensor)                              \
makeFvLaplacianTypeScheme(SS, scalar, tensor)                                  \
makeFvLaplacianTypeScheme(SS, symmTensor, tensor)                              \
makeFvLaplacianTypeScheme(SS, tensor, tensor)

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fv.hpp"
#include "HashTable.hpp"
#include "linear.hpp"
#include "fvMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type, class GType>
tmp<laplacianScheme<Type, GType> > laplacianScheme<Type, GType>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "laplacianScheme<Type, GType>::New(const fvMesh&, Istream&) : "
               "constructing laplacianScheme<Type, GType>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "Laplacian scheme not specified" << endl << endl
            << "Valid laplacian schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown laplacian scheme " << schemeName << nl << nl
            << "Valid laplacian schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type, class GType>
laplacianScheme<Type, GType>::~laplacianScheme()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacianScheme<Type, GType>::fvmLaplacian
(
    const GeometricField<GType, fvPatchField, volMesh>& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvmLaplacian(tinterpGammaScheme_().interpolate(gamma)(), vf);
}


template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh> >
laplacianScheme<Type, GType>::fvcLaplacian
(
    const GeometricField<GType, fvPatchField, volMesh>& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvcLaplacian(tinterpGammaScheme_().interpolate(gamma)(), vf);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
