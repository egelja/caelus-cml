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
    CML::fv::divScheme

Description
    Abstract base class for div schemes.


\*---------------------------------------------------------------------------*/

#ifndef divScheme_H
#define divScheme_H

#include "tmp.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "linear.hpp"
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
                           Class divScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class divScheme
:
    public refCount
{

protected:

    // Protected data

        const fvMesh& mesh_;
        tmp<surfaceInterpolationScheme<Type> > tinterpScheme_;


    // Private Member Functions

        //- Disallow copy construct
        divScheme(const divScheme&);

        //- Disallow default bitwise assignment
        void operator=(const divScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            divScheme,
            Istream,
            (const fvMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        divScheme(const fvMesh& mesh)
        :
            mesh_(mesh),
            tinterpScheme_(new linear<Type>(mesh))
        {}

        //- Construct from mesh and Istream
        divScheme(const fvMesh& mesh, Istream& is)
        :
            mesh_(mesh),
            tinterpScheme_(surfaceInterpolationScheme<Type>::New(mesh, is))
        {}


    // Selectors

        //- Return a pointer to a new divScheme created on freestore
        static tmp<divScheme<Type> > New
        (
            const fvMesh& mesh,
            Istream& schemeData
        );


    //- Destructor
    virtual ~divScheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }

        virtual tmp
        <
            GeometricField
            <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
        > fvcDiv
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeFvDivTypeScheme(SS, Type)                                          \
    defineNamedTemplateTypeNameAndDebug(CML::fv::SS<CML::Type>, 0);            \
                                                                               \
    namespace CML                                                              \
    {                                                                          \
        namespace fv                                                           \
        {                                                                      \
            divScheme<Type>::addIstreamConstructorToTable<SS<Type> >           \
                add##SS##Type##IstreamConstructorToTable_;                     \
        }                                                                      \
    }

#define makeFvDivScheme(SS)                                                    \
                                                                               \
makeFvDivTypeScheme(SS, vector)                                                \
makeFvDivTypeScheme(SS, sphericalTensor)                                       \
makeFvDivTypeScheme(SS, symmTensor)                                            \
makeFvDivTypeScheme(SS, tensor)


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

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
tmp<divScheme<Type> > divScheme<Type>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "divScheme<Type>::New(const fvMesh&, Istream&) : "
               "constructing divScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "Div scheme not specified" << endl << endl
            << "Valid div schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "unknown div scheme "
            << schemeName << nl << nl
            << "Valid div schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
divScheme<Type>::~divScheme()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
