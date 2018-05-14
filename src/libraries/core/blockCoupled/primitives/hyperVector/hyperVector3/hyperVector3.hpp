/*---------------------------------------------------------------------------*\
Copyright (C) 2013 H. Jasak
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
    hyperVector3

Description
    hyperVector3 obtained from generic hyperVector
    
    No author was named in the original file. Assumed to be Hrvoje Jasak
    as he authored the commit aac5de to first introduce this file into the
    FOAM-Extend-4.0 repository
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/
    
    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd. (see Description)
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef hyperVector3_HPP
#define hyperVector3_HPP

#include "scalar.hpp"
#include "hyperVector.hpp"
#include "contiguous.hpp"


namespace CML
{

typedef hyperVector<scalar, 3> hyperVector3;

//- Specify data associated with hyperVector3 type is contiguous
template<>
inline bool contiguous<hyperVector3>()
{
    return true;
}


} // End namespace CML
#endif
