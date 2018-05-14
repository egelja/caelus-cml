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
    hyperTensor3

Description
    hyperTensor of 3 scalars.
    
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

#ifndef hyperTensor3_HPP
#define hyperTensor3_HPP

#include "hyperTensor.hpp"
#include "hyperVector3.hpp"
#include "contiguous.hpp"


namespace CML
{

typedef hyperTensor<scalar, 3> hyperTensor3;

//- Specify data associated with hyperTensor3 type is contiguous
template<>
inline bool contiguous<hyperTensor3>()
{
    return true;
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Return the determinant of a tensor
inline scalar det(const hyperTensor3& t)
{
    return
    (
        t[0]*t[4]*t[8] + t[1]*t[5]*t[6]
      + t[2]*t[3]*t[7] - t[0]*t[5]*t[7]
      - t[1]*t[3]*t[8] - t[2]*t[4]*t[6]
    );
}


//- Return the inverse of a tensor given the determinant
inline hyperTensor3 inv(const hyperTensor3& t)
{
    hyperTensor3 cofactor;

    cofactor[0] = t[4]*t[8] - t[7]*t[5];
    cofactor[1] = t[6]*t[5] - t[3]*t[8];
    cofactor[2] = t[3]*t[7] - t[4]*t[6];
    cofactor[3] = t[2]*t[7] - t[1]*t[8];
    cofactor[4] = t[0]*t[8] - t[2]*t[6];
    cofactor[5] = t[1]*t[6] - t[0]*t[7];
    cofactor[6] = t[1]*t[5] - t[2]*t[4];
    cofactor[7] = t[3]*t[2] - t[0]*t[5];
    cofactor[8] = t[0]*t[4] - t[3]*t[1];

    return cofactor/det(t);
}


} // End namespace CML
#endif
