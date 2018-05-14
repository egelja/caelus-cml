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
    hyperTensor2

Description
    hyperTensor of 2 scalars.
    
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

#ifndef hyperTensor2_HPP
#define hyperTensor2_HPP

#include "hyperTensor.hpp"
#include "hyperVector2.hpp"
#include "contiguous.hpp"


namespace CML
{

typedef hyperTensor<scalar, 2> hyperTensor2;

//- Specify data associated with hyperTensor2 type is contiguous
template<>
inline bool contiguous<hyperTensor2>()
{
    return true;
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Return tensor transpose
template<>
inline hyperTensor2 hyperTensor2::T() const
{
    hyperTensor2 transpose;

    transpose[0] = this->operator[](0);
    transpose[1] = this->operator[](2);
    transpose[2] = this->operator[](1);
    transpose[3] = this->operator[](3);

    return transpose;
}

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

//- Inner-product between two tensors
inline hyperTensor2
operator&(const hyperTensor2& t1, const hyperTensor2& t2)
{
    hyperTensor2 result;

    result[0] = t1[0]*t2[0] + t1[1]*t2[2];
    result[1] = t1[0]*t2[1] + t1[1]*t2[3];
    result[2] = t1[2]*t2[0] + t1[3]*t2[2];
    result[3] = t1[2]*t2[1] + t1[3]*t2[3];

    return result;
}


//- Inner-product between a tensor and a vector
inline hyperVector2
operator&(const hyperTensor2& t, const hyperVector2& v)
{
    hyperVector2 result;

    result[0] = t[0]*v[0] + t[1]*v[1];
    result[1] = t[2]*v[0] + t[3]*v[1];

    return result;
}


//- Inner-product between a vector and a tensor
inline hyperVector2
operator&(const hyperVector2& v, const hyperTensor2& t)
{
    hyperVector2 result;

    result[0] = v[0]*t[0] + v[1]*t[2];
    result[1] = v[0]*t[1] + v[1]*t[3];

    return result;
}


//- Outer-product between two vectors
inline hyperTensor2
operator*(const hyperVector2& v1, const hyperVector2& v2)
{
    hyperTensor2 result;

    result[0] = v1[0]*v2[0];
    result[1] = v1[0]*v2[1];
    result[2] = v1[1]*v2[0];
    result[3] = v1[1]*v2[1];

    return result;
}


//- Return the determinant of a tensor
inline scalar det(const hyperTensor2& t)
{
    return (t[0]*t[3]-t[1]*t[2]);
}


//- Return the inverse of a tensor given the determinant
inline hyperTensor2 inv(const hyperTensor2& t)
{
    hyperTensor2 cofactor;

    cofactor[0] = t[3];
    cofactor[1] = -t[1];
    cofactor[2] = -t[2];
    cofactor[3] = t[0];

    return cofactor/det(t);
}


} // End namespace CML
#endif
