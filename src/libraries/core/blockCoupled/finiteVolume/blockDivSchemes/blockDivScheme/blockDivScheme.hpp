/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 H. Jasak
Copyright (C) 2017-2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::fv::blockDivScheme

Description
    Abstract base class for block div schemes.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

\*---------------------------------------------------------------------------*/

#ifndef blockDivScheme_HPP
#define blockDivScheme_HPP

#include "tmp.hpp"
#include "volFieldsFwd.hpp"
#include "linear.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"
#include "BlockLduSystem.hpp"
#include "fv.hpp"


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
class blockDivScheme
:
    public refCount
{

protected:

    const fvMesh& mesh_;
    tmp<surfaceInterpolationScheme<Type> > tinterpScheme_;

    //- Disallow copy construct
    blockDivScheme(const blockDivScheme&);

    //- Disallow default bitwise assignment
    void operator=(const blockDivScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;

    // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        tmp,
        blockDivScheme,
        Istream,
        (const fvMesh& mesh, Istream& schemeData),
        (mesh, schemeData)
    );

    //- Construct from mesh
    blockDivScheme(const fvMesh& mesh)
    :
        mesh_(mesh),
        tinterpScheme_(new linear<Type>(mesh))
    {}

    //- Construct from mesh and Istream
    blockDivScheme(const fvMesh& mesh, Istream& is)
    :
        mesh_(mesh),
        tinterpScheme_(surfaceInterpolationScheme<Type>::New(mesh, is))
    {}

    // Selectors
    //- Return a pointer to a new blockDivScheme created on freestore
    static tmp<blockDivScheme<Type> > New
    (
        const fvMesh& mesh,
        Istream& schemeData
    );

    virtual ~blockDivScheme();

    // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
        return mesh_;
    }

    //- Return the BlockLduSystem corresponding to the implicit div
    // discretization. For block coupled system.
    virtual
    tmp<BlockLduSystem<vector, typename innerProduct<vector, Type>::type> >
    fvmUDiv
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;

    virtual
    tmp<BlockLduSystem<vector, typename innerProduct<vector, Type>::type> >
    fvmUDiv
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};


} // End namespace fv
} // End namespace CML


// Add the patch constructor functions to the hash tables

#define makeBlockFvDivTypeScheme(SS, Type)                                    \
                                                                              \
defineNamedTemplateTypeNameAndDebug(CML::fv::SS<CML::Type>, 0);               \
                                                                              \
    namespace CML                                                             \
    {                                                                         \
        namespace fv                                                          \
        {                                                                     \
            blockDivScheme<Type>::addIstreamConstructorToTable<SS<Type> >     \
                add##SS##Type##IstreamConstructorToTable_;                    \
        }                                                                     \
    }                                                      


#define makeBlockFvDivScheme(SS)                                              \
                                                                              \
makeBlockFvDivTypeScheme(SS, vector)


namespace CML
{

namespace fv
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
tmp<blockDivScheme<Type> > blockDivScheme<Type>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "blockDivScheme<Type>::New(const fvMesh&, Istream&) : "
               "constructing blockDivScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "block div scheme not specified" << endl << endl
            << "Valid block div schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "unknown block div scheme " << schemeName << endl << endl
            << "Valid block div schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, schemeData);
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
blockDivScheme<Type>::~blockDivScheme()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<BlockLduSystem<vector, typename innerProduct<vector, Type>::type> >
blockDivScheme<Type>::fvmUDiv
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    FatalErrorInFunction
        << "Implicit block div operator defined only for vector. "
        << abort(FatalError);

    typedef typename innerProduct<vector, Type>::type DivType;

    tmp<BlockLduSystem<vector, DivType> > tbs
    (
        new BlockLduSystem<vector, DivType>(vf.mesh())
    );

    return tbs;
}


template<class Type>
tmp<BlockLduSystem<vector, typename innerProduct<vector, Type>::type> >
blockDivScheme<Type>::fvmUDiv
(
    const surfaceScalarField& flux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    FatalErrorInFunction
        << "Implicit block div operator defined only for vector. "
        << abort(FatalError);

    typedef typename innerProduct<vector, Type>::type DivType;

    tmp<BlockLduSystem<vector, DivType> > tbs
    (
        new BlockLduSystem<vector, DivType>(vf.mesh())
    );

    return tbs;
}


} // End namespace fv
} // End namespace CML
#endif
