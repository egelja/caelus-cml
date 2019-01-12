/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2013-2016 H. Jasak
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
    CML::fv::blockGradScheme

Description
    Abstract base class for block gradient schemes.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

\*---------------------------------------------------------------------------*/

#ifndef blockGradScheme_HPP
#define blockGradScheme_HPP

#include "tmp.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"
#include "BlockLduSystem.hpp"
#include "fv.hpp"


namespace CML
{

class fvMesh;

namespace fv
{

template<class Type>
class blockGradScheme
:
    public refCount
{
public:

    typedef Field<Type> FieldType;
    typedef GeometricField<Type, fvPatchField, volMesh> GeoFieldType;
    typedef typename GeoFieldType::GeometricBoundaryField GeoBoundaryFieldType;

    typedef Field<typename outerProduct<vector, Type>::type> GradFieldType;
    typedef GeometricField
    <
        typename outerProduct<vector, Type>::type, fvPatchField, volMesh
    > GeoGradFieldType;
    typedef BlockLduSystem<vector, typename outerProduct<vector, Type>::type>
    GradMatrixType;

    const fvMesh& mesh_;

    //- Disallow copy construct
    blockGradScheme(const blockGradScheme&);

    //- Disallow default bitwise assignment
    void operator=(const blockGradScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;

    // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        tmp,
        blockGradScheme,
        Istream,
        (const fvMesh& mesh, Istream& schemeData),
        (mesh, schemeData)
    );

    //- Construct from mesh
    blockGradScheme(const fvMesh& mesh)
    :
        mesh_(mesh)
    {}

    // Selectors
    //- Return a pointer to a new blockGradScheme created on freestore
    static tmp<blockGradScheme<Type> > New
    (
        const fvMesh& mesh,
        Istream& schemeData
    );

    virtual ~blockGradScheme();

    // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
        return mesh_;
    }

    // Gradient functions

    //- Return the BlockLduSystem corresponding to the implicit grad
    // discretization. For block coupled systems.
    virtual tmp<GradMatrixType> fvmGrad
    (
        const GeoFieldType&
    ) const;
};


} // End namespace fv
} // End namespace CML


// Add the patch constructor functions to the hash tables

#define makeBlockFvGradTypeScheme(SS, Type)                                   \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(CML::fv::SS<CML::Type>, 0);           \
                                                                              \
    namespace CML                                                             \
    {                                                                         \
        namespace fv                                                          \
        {                                                                     \
            blockGradScheme<Type>::addIstreamConstructorToTable<SS<Type> >    \
                add##SS##Type##IstreamConstructorToTable_;                    \
        }                                                                     \
    }


#define makeBlockFvGradScheme(SS)                                             \
                                                                              \
makeBlockFvGradTypeScheme(SS, scalar)

                                                                             
namespace CML
{
namespace fv
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
tmp<blockGradScheme<Type> > blockGradScheme<Type>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "blockGradScheme<Type>::New"
               "(const fvMesh& mesh, Istream& schemeData) : "
               "constructing blockGradScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "Block grad scheme not specified" << endl << endl
            << "Valid block grad schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown block grad scheme " << schemeName << nl << nl
            << "Valid block grad schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, schemeData);
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
blockGradScheme<Type>::~blockGradScheme()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp
<
    BlockLduSystem<vector, typename outerProduct<vector, Type>::type>
>
fv::blockGradScheme<Type>::fvmGrad
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    FatalErrorInFunction
        << "Implicit block gradient operator currently defined only for "
        << "Gauss linear and leastSquares "
        << "(cell and face limiters are optional)."
        << abort(FatalError);

    typedef typename outerProduct<vector, Type>::type GradType;

    tmp<BlockLduSystem<vector, GradType> > tbs
    (
        new BlockLduSystem<vector, GradType>(vf.mesh())
    );

    return tbs;
}


} // End namespace fv
} // End namespace CML
#endif
