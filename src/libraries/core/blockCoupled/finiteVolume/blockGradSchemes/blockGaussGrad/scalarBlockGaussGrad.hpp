/*---------------------------------------------------------------------------*\
Copyright (C) 2010-2015 H. Jasak
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
    CML::fv::scalarBlockGaussGrad

Description
    Specialisation of blockGaussGrad for scalars. Needed for implicit fvmGrad
    operator for block coupled systems.

    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

SourceFiles
    scalarBlockGaussGrad.cpp

\*---------------------------------------------------------------------------*/

#ifndef scalarBlockGaussGrad_HPP
#define scalarBlockGaussGrad_HPP

#include "blockGaussGrad.hpp"


namespace CML
{
namespace fv
{

template<>
tmp<BlockLduSystem<vector, vector> > blockGaussGrad<scalar>::fvmGrad
(
    const GeometricField<scalar, fvPatchField, volMesh>&
) const;


} // End namespace fv
} // End namespace CML
#endif
