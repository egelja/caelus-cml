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
    CML::fv::d2dt2Scheme

Description
    Abstract base class for d2dt2 schemes.


\*---------------------------------------------------------------------------*/

#ifndef d2dt2Scheme_H
#define d2dt2Scheme_H

#include "tmp.hpp"
#include "dimensionedType.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
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
                           Class d2dt2Scheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class d2dt2Scheme
:
    public refCount
{

protected:

    // Protected data

        const fvMesh& mesh_;


    // Private Member Functions

        //- Disallow copy construct
        d2dt2Scheme(const d2dt2Scheme&);

        //- Disallow default bitwise assignment
        void operator=(const d2dt2Scheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            d2dt2Scheme,
            Istream,
            (const fvMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        d2dt2Scheme(const fvMesh& mesh)
        :
            mesh_(mesh)
        {}

        //- Construct from mesh and Istream
        d2dt2Scheme(const fvMesh& mesh, Istream&)
        :
            mesh_(mesh)
        {}


    // Selectors

        //- Return a pointer to a new d2dt2Scheme created on freestore
        static tmp<d2dt2Scheme<Type> > New
        (
            const fvMesh& mesh,
            Istream& schemeData
        );


    //- Destructor
    virtual ~d2dt2Scheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcD2dt2
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcD2dt2
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<fvMatrix<Type> > fvmD2dt2
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<fvMatrix<Type> > fvmD2dt2
        (
            const dimensionedScalar&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;

        virtual tmp<fvMatrix<Type> > fvmD2dt2
        (
            const volScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeFvD2dt2TypeScheme(SS, Type)                                        \
    defineNamedTemplateTypeNameAndDebug(CML::fv::SS<CML::Type>, 0);            \
                                                                               \
    namespace CML                                                              \
    {                                                                          \
        namespace fv                                                           \
        {                                                                      \
            d2dt2Scheme<Type>::addIstreamConstructorToTable<SS<Type> >         \
                add##SS##Type##IstreamConstructorToTable_;                     \
        }                                                                      \
    }

#define makeFvD2dt2Scheme(SS)                                                  \
                                                                               \
makeFvD2dt2TypeScheme(SS, scalar)                                              \
makeFvD2dt2TypeScheme(SS, vector)                                              \
makeFvD2dt2TypeScheme(SS, sphericalTensor)                                     \
makeFvD2dt2TypeScheme(SS, symmTensor)                                          \
makeFvD2dt2TypeScheme(SS, tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fv.hpp"
#include "HashTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
tmp<d2dt2Scheme<Type> > d2dt2Scheme<Type>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "d2dt2Scheme<Type>::New(const fvMesh&, Istream&) : "
               "constructing d2dt2Scheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "D2dt2 scheme not specified" << endl << endl
            << "Valid d2dt2 schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown d2dt2 scheme " << schemeName << nl << nl
            << "Valid d2dt2 schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
d2dt2Scheme<Type>::~d2dt2Scheme()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
