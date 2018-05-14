/*---------------------------------------------------------------------------*\
Copyright (C) 2014 V. Vukcevic
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
    Calculate the BlockLduSystem (matrix & source) for the gradient
    of the field.
    Intended use: block coupled solvers. i.e. implicit grad(p) in momentum
    equation.

    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Vuko Vukcevic, FMENA Zagreb.

\*---------------------------------------------------------------------------*/

#ifndef blockFvmGrad_HPP
#define blockFvmGrad_HPP

#include "BlockLduSystem.hpp"
#include "blockGradScheme.hpp"


namespace CML
{

namespace blockFvm
{


    template<class Type>
    tmp
    <
        BlockLduSystem<vector, typename outerProduct<vector, Type>::type>
    > grad
    (
        const GeometricField<Type, fvPatchField, volMesh>& vf,
        const word& name
    )
    {
        return fv::blockGradScheme<Type>::New
        (
            vf.mesh(),
            vf.mesh().gradScheme(name)
        )().fvmGrad(vf);
    }


    template<class Type>
    tmp
    <
        BlockLduSystem<vector, typename outerProduct<vector, Type>::type>
    > grad
    (
        const GeometricField<Type, fvPatchField, volMesh>& vf
    )
    {
        return blockFvm::grad
        (
            vf,
            "blockGrad(" + vf.name() + ')'
        );
    }


} // End namespace blockFvm
} // End namespace CML
#endif
