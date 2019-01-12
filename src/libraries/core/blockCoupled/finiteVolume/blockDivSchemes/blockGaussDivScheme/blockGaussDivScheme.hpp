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
    CML::fv::blockGaussDivScheme

Description
    Basic second-order block div using face-gradients and Gauss' theorem.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

SourceFiles
    blockGaussDivScheme.cpp

\*---------------------------------------------------------------------------*/

#ifndef gaussBlockDivScheme_HPP
#define gaussBlockDivScheme_HPP

#include "blockDivScheme.hpp"


namespace CML
{
namespace fv
{

template<class Type>
class blockGaussDivScheme
:
    public fv::blockDivScheme<Type>
{

    //- Disallow default bitwise copy construct
    blockGaussDivScheme(const blockGaussDivScheme&);

    //- Disallow default bitwise assignment
    void operator=(const blockGaussDivScheme&);


public:

    //- Runtime type information
    TypeName("Gauss");

    //- Construct null
    blockGaussDivScheme(const fvMesh& mesh)
    :
        blockDivScheme<Type>(mesh)
    {}

    //- Construct from Istream
    blockGaussDivScheme(const fvMesh& mesh, Istream& is)
    :
        blockDivScheme<Type>(mesh, is)
    {}

    //- Return the BlockLduSystem corresponding to the implicit div
    // discretization. For block coupled system.
    tmp<BlockLduSystem<vector, typename innerProduct<vector, Type>::type> >
    fvmUDiv
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;

    tmp<BlockLduSystem<vector, typename innerProduct<vector, Type>::type> >
    fvmUDiv
    (
        const surfaceScalarField& flux,
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};


template<class Type>
tmp<BlockLduSystem<vector, typename innerProduct<vector, Type>::type> >
blockGaussDivScheme<Type>::fvmUDiv
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    FatalErrorInFunction
        << "Implicit block div operator defined only for vector."
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
blockGaussDivScheme<Type>::fvmUDiv
(
    const surfaceScalarField& flux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    FatalErrorInFunction
        << "Implicit block div operator defined only for vector."
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
