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

Class
    CML::multivariateSurfaceInterpolationScheme

Description
    Abstract base class for multi-variate surface interpolation schemes.


\*---------------------------------------------------------------------------*/

#ifndef multivariateSurfaceInterpolationScheme_H
#define multivariateSurfaceInterpolationScheme_H

#include "surfaceInterpolationScheme.hpp"
#include "HashTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class multivariateSurfaceInterpolationScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class multivariateSurfaceInterpolationScheme
:
    public refCount
{

public:

    //- fieldTable
    class fieldTable
    :
        public HashTable<const GeometricField<Type, fvPatchField, volMesh>*>
    {
    public:

        fieldTable()
        {}

        void add(const GeometricField<Type, fvPatchField, volMesh>& f)
        {
            this->insert(f.name(), &f);
        }
    };


private:

    // Private data

        //- Hold reference to mesh
        const fvMesh& mesh_;

        //- HashTable of pointers to the field set
        const fieldTable& fields_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        multivariateSurfaceInterpolationScheme
        (
            const multivariateSurfaceInterpolationScheme&
        );

        //- Disallow default bitwise assignment
        void operator=(const multivariateSurfaceInterpolationScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            multivariateSurfaceInterpolationScheme,
            Istream,
            (
                const fvMesh& mesh,
                const fieldTable& fields,
                const surfaceScalarField& faceFlux,
                Istream& is
            ),
            (mesh, fields, faceFlux, is)
        );


    // Constructors

        //- Construct for interpolating given field
        multivariateSurfaceInterpolationScheme
        (
            const fvMesh& mesh,
            const fieldTable& fields,
            const surfaceScalarField& faceFlux,
            Istream& schemeData
        );


    // Selectors

        //- Return a pointer to a new gradScheme created on freestore
        static tmp<multivariateSurfaceInterpolationScheme<Type> > New
        (
            const fvMesh& mesh,
            const fieldTable& fields,
            const surfaceScalarField& faceFlux,
            Istream& schemeData
        );


    //- Destructor
    virtual ~multivariateSurfaceInterpolationScheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }

        //- Return fields to be interpolated
        const fieldTable& fields() const
        {
            return fields_;
        }


    // Member Operators

        //- surfaceInterpolationScheme sub-class returned by operator(field)
        //  which is used as the interpolation scheme for the field
        class fieldScheme
        :
            public surfaceInterpolationScheme<Type>
        {

        public:

            // Constructors

                fieldScheme
                (
                    const GeometricField<Type, fvPatchField, volMesh>& field
                )
                :
                    surfaceInterpolationScheme<Type>(field.mesh())
                {}


            // Member Functions

                //- Return the interpolation weighting factors
                virtual tmp<surfaceScalarField> weights
                (
                    const GeometricField<Type, fvPatchField, volMesh>& field
                ) const = 0;
        };

        virtual tmp<surfaceInterpolationScheme<Type> > operator()
        (
            const GeometricField<Type, fvPatchField, volMesh>& field
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeMultivariateSurfaceInterpolationTypeScheme(SS, Type)               \
                                                                               \
defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                              \
                                                                               \
multivariateSurfaceInterpolationScheme<Type>::                                 \
addIstreamConstructorToTable<SS<Type> >                                        \
    add##SS##Type##ConstructorToTable_;


#define makeMultivariateSurfaceInterpolationScheme(SS)                         \
                                                                               \
makeMultivariateSurfaceInterpolationTypeScheme(SS, scalar)                     \
makeMultivariateSurfaceInterpolationTypeScheme(SS, vector)                     \
makeMultivariateSurfaceInterpolationTypeScheme(SS, sphericalTensor)            \
makeMultivariateSurfaceInterpolationTypeScheme(SS, symmTensor)                 \
makeMultivariateSurfaceInterpolationTypeScheme(SS, tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
