/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute itand/or modify it
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
    CML::fv::convectionScheme

Description
    Abstract base class for convection schemes.


\*---------------------------------------------------------------------------*/

#ifndef convectionScheme_H
#define convectionScheme_H

#include "tmp.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"
#include "multivariateSurfaceInterpolationScheme.hpp"

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
                           Class convectionScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class convectionScheme
:
    public refCount
{
    // Private data

        const fvMesh& mesh_;


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            convectionScheme,
            Istream,
            (
                const fvMesh& mesh,
                const surfaceScalarField& faceFlux,
                Istream& schemeData
            ),
            (mesh, faceFlux, schemeData)
        );

        declareRunTimeSelectionTable
        (
            tmp,
            convectionScheme,
            Multivariate,
            (
                const fvMesh& mesh,
                const typename multivariateSurfaceInterpolationScheme<Type>::
                    fieldTable& fields,
                const surfaceScalarField& faceFlux,
                Istream& schemeData
            ),
            (mesh, fields, faceFlux, schemeData)
        );


    // Constructors

        //- Copy construct
        convectionScheme(const convectionScheme&);

        //- Construct from mesh, flux and Istream
        convectionScheme
        (
            const fvMesh& mesh,
            const surfaceScalarField&
        )
        :
            mesh_(mesh)
        {}


    // Selectors

        //- Return a pointer to a new convectionScheme created on freestore
        static tmp<convectionScheme<Type> > New
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux,
            Istream& schemeData
        );


        //- Return a pointer to a new multivariate convectionScheme
        //  created on freestore
        static tmp<convectionScheme<Type> > New
        (
            const fvMesh& mesh,
            const typename multivariateSurfaceInterpolationScheme<Type>::
                fieldTable& fields,
            const surfaceScalarField& faceFlux,
            Istream& schemeData
        );


    //- Destructor
    virtual ~convectionScheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }

        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        interpolate
        (
            const surfaceScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const = 0;

        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
        (
            const surfaceScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const = 0;

        virtual tmp<fvMatrix<Type> > fvmDiv
        (
            const surfaceScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDiv
        (
            const surfaceScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const = 0;

        virtual tmp<surfaceInterpolationScheme<Type> > 
        interpScheme() const
        {
            FatalErrorInFunction
            << "Current scheme does not implement interpScheme() function" 
            << abort(FatalError);
            // Dummy return - the code should have failed before the return
            tmp<surfaceInterpolationScheme<Type> > x;
            return x;
        }


    // Member operators

        void operator=(const convectionScheme<Type>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeFvConvectionTypeScheme(SS, Type)                                   \
    defineNamedTemplateTypeNameAndDebug(CML::fv::SS<CML::Type>, 0);            \
                                                                               \
    namespace CML                                                              \
    {                                                                          \
        namespace fv                                                           \
        {                                                                      \
            convectionScheme<Type>::addIstreamConstructorToTable<SS<Type> >    \
                add##SS##Type##IstreamConstructorToTable_;                     \
        }                                                                      \
    }

#define makeFvConvectionScheme(SS)                                             \
                                                                               \
makeFvConvectionTypeScheme(SS, scalar)                                         \
makeFvConvectionTypeScheme(SS, vector)                                         \
makeFvConvectionTypeScheme(SS, sphericalTensor)                                \
makeFvConvectionTypeScheme(SS, symmTensor)                                     \
makeFvConvectionTypeScheme(SS, tensor)


#define makeMultivariateFvConvectionTypeScheme(SS, Type)                       \
    defineNamedTemplateTypeNameAndDebug(CML::fv::SS<CML::Type>, 0);            \
                                                                               \
    namespace CML                                                              \
    {                                                                          \
        namespace fv                                                           \
        {                                                                      \
            convectionScheme<Type>::                                           \
                addMultivariateConstructorToTable<SS<Type> >                   \
                add##SS##Type##MultivariateConstructorToTable_;                \
        }                                                                      \
    }


#define makeMultivariateFvConvectionScheme(SS)                                 \
                                                                               \
makeMultivariateFvConvectionTypeScheme(SS, scalar)                             \
makeMultivariateFvConvectionTypeScheme(SS, vector)                             \
makeMultivariateFvConvectionTypeScheme(SS, sphericalTensor)                    \
makeMultivariateFvConvectionTypeScheme(SS, symmTensor)                         \
makeMultivariateFvConvectionTypeScheme(SS, tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fv.hpp"
#include "HashTable.hpp"
#include "linear.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
convectionScheme<Type>::convectionScheme(const convectionScheme& cs)
:
    refCount(),
    mesh_(cs.mesh_)
{}


// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
tmp<convectionScheme<Type> > convectionScheme<Type>::New
(
    const fvMesh& mesh,
    const surfaceScalarField& faceFlux,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "convectionScheme<Type>::New"
               "(const fvMesh&, const surfaceScalarField&, Istream&) : "
               "constructing convectionScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "Convection scheme not specified" << endl << endl
            << "Valid convection schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown convection scheme " << schemeName << nl << nl
            << "Valid convection schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, faceFlux, schemeData);
}


template<class Type>
tmp<convectionScheme<Type> > convectionScheme<Type>::New
(
    const fvMesh& mesh,
    const typename multivariateSurfaceInterpolationScheme<Type>::
        fieldTable& fields,
    const surfaceScalarField& faceFlux,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "convectionScheme<Type>::New"
               "(const fvMesh&, "
               "const typename multivariateSurfaceInterpolationScheme<Type>"
               "::fieldTable&, const surfaceScalarField&, Istream&) : "
               "constructing convectionScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "Convection scheme not specified" << endl << endl
            << "Valid convection schemes are :" << endl
            << MultivariateConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename MultivariateConstructorTable::iterator cstrIter =
        MultivariateConstructorTablePtr_->find(schemeName);

    if (cstrIter == MultivariateConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown convection scheme " << schemeName << nl << nl
            << "Valid convection schemes are :" << endl
            << MultivariateConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, fields, faceFlux, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
convectionScheme<Type>::~convectionScheme()
{}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void convectionScheme<Type>::operator=(const convectionScheme<Type>& cs)
{
    if (this == &cs)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
