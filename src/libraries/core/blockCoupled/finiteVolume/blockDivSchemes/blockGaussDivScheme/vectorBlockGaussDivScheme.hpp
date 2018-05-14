/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 H. Jasak
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

Typedef
    CML::fv::vectorBlockGaussDivScheme

Description
    Specialisation of blockGaussDivScheme for vectors. Needed for implicit
    fvmDiv operator for block coupled systems.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

SourceFiles
    vectorBlockGaussDivScheme.cpp

\*---------------------------------------------------------------------------*/

#ifndef vectorBlockGaussDivScheme_HPP
#define vectorBlockGaussDivScheme_HPP

#include "blockGaussDivScheme.hpp"


namespace CML
{
namespace fv
{

    template<>
    tmp<BlockLduSystem<vector, scalar> > blockGaussDivScheme<vector>::fvmUDiv
    (
        const GeometricField<vector, fvPatchField, volMesh>&
    ) const;


    template<>
    tmp<BlockLduSystem<vector, scalar> > blockGaussDivScheme<vector>::fvmUDiv
    (
        const surfaceScalarField& flux,
        const GeometricField<vector, fvPatchField, volMesh>&
    ) const;


} // End namespace fv
} // End namespace CML
#endif
