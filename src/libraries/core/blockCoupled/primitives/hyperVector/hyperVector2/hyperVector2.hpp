/*---------------------------------------------------------------------------*\
Copyright (C) 2010 I. Clifford
Copyright (C) 2010 H. Jasak
Copyright (C) 2017 Applied CCM Pty Ltd
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

Type
    hyperVector2

Description
    hyperVector2 obtained from generic hyperVector.
    
    This file was obtained from Ivor Clifford at the OpenFOAM Workshop
    in PennState, USA, 2011.
    
    No author was named in the original file. Authorship confirmed by
    Ivor Clifford (2017).

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.
    
Author
    Ivor Clifford
    Hrvoje Jasak
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef hyperVector2_HPP
#define hyperVector2_HPP

#include "scalar.hpp"
#include "hyperVector.hpp"
#include "contiguous.hpp"


namespace CML
{

typedef hyperVector<scalar, 2> hyperVector2;

//- Specify data associated with hyperVector2 type is contiguous
template<>
inline bool contiguous<hyperVector2>()
{
    return true;
}


} // End namespace CML
#endif
