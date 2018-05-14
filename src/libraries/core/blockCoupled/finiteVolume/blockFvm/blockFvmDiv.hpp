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

InNamespace
    CML::blockFvm

Description
    Calculate the matrix for the divergence of the given field and flux.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.


\*---------------------------------------------------------------------------*/

#ifndef blockFvmDiv_HPP
#define blockFvmDiv_HPP

#include "BlockLduSystem.hpp"
#include "blockDivScheme.hpp"


namespace CML
{

namespace blockFvm
{

    template<class Type>
    tmp
    <
        BlockLduSystem<vector, typename innerProduct<vector, Type>::type>
    > UDiv
    (
        const GeometricField<Type, fvPatchField, volMesh>& vf,
        const word& name
    )
    {
        return fv::blockDivScheme<Type>::New
        (
            vf.mesh(),
            vf.mesh().divScheme(name)
        )().fvmUDiv(vf);
    }


    template<class Type>
    tmp
    <
        BlockLduSystem<vector, typename innerProduct<vector, Type>::type>
    > UDiv
    (
        const surfaceScalarField& rho,
        const GeometricField<Type, fvPatchField, volMesh>& vf,
        const word& name
    )
    {
        return fv::blockDivScheme<Type>::New
        (
            vf.mesh(),
            vf.mesh().divScheme(name)
        )().fvmUDiv(rho, vf);
    }


    template<class Type>
    tmp
    <
        BlockLduSystem<vector, typename innerProduct<vector, Type>::type>
    > UDiv
    (
        const tmp<surfaceScalarField>& trho,
        const GeometricField<Type, fvPatchField, volMesh>& vf,
        const word& name
    )
    {
        tmp
        <
            BlockLduSystem<vector, typename innerProduct<vector, Type>::type>
        >
        Div(blockFvm::UDiv(trho(), vf, name));
        trho.clear();
        return Div;
    }


    template<class Type>
    tmp
    <
        BlockLduSystem<vector, typename innerProduct<vector, Type>::type>
    > UDiv
    (
        const GeometricField<Type, fvPatchField, volMesh>& vf
    )
    {
        return blockFvm::UDiv
        (
            vf,
            "blockDiv(" + vf.name() + ')'
        );
    }


    template<class Type>
    tmp
    <
        BlockLduSystem<vector, typename innerProduct<vector, Type>::type>
    > UDiv
    (
        const surfaceScalarField& rho,
        const GeometricField<Type, fvPatchField, volMesh>& vf
    )
    {
        return blockFvm::UDiv
        (
            rho,
            vf,
            "blockDiv(" + rho.name() + ',' + vf.name() + ')'
        );
    }


    template<class Type>
    tmp
    <
        BlockLduSystem<vector, typename innerProduct<vector, Type>::type>
    > UDiv
    (
        const tmp<surfaceScalarField>& trho,
        const GeometricField<Type, fvPatchField, volMesh>& vf
    )
    {
        tmp
        <
            BlockLduSystem<vector, typename innerProduct<vector, Type>::type>
        >
        Div(blockFvm::UDiv(trho(), vf));
        trho.clear();
        return Div;
    }


} // End namespace blockFvm
} // End namespace CML
#endif
