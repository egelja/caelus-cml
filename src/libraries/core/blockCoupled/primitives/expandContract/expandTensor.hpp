/*---------------------------------------------------------------------------*\
Copyright 2004-2016 H. Jasak
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description
    Global functions for expansion and contraction of tensor coefficient
    to diagonal type.
    
    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#ifndef expandTensor_HPP
#define expandTensor_HPP

#include "Tensor_.hpp"


namespace CML
{

//- Return the diagonal of a tensor as a scalar
template <class Cmpt>
inline void contractScalar(Cmpt& result, const Tensor<Cmpt>& t)
{
    result = 1.0/3.0*(t.xx() + t.yy() + t.zz());
}


//- Return the diagonal of a tensor as a scalar
template <class Cmpt>
inline Cmpt contractScalar(const Tensor<Cmpt>& t)
{
    Cmpt result;
    contractScalar(result, t);
    return result;
}


//- Return the diagonal of a tensor as a vector
template <class Cmpt>
inline void contractLinear(Vector<Cmpt>& result, const Tensor<Cmpt>& t)
{
    result = Vector<Cmpt>(t.xx(), t.yy(), t.zz());
}


//- Return the diagonal of a tensor as a vector
template <class Cmpt>
inline Vector<Cmpt> contractLinear(const Tensor<Cmpt>& t)
{
    Vector<Cmpt> result;
    contractLinear(result, t);
    return result;
}


//- Return the vector given a scalar
template <class Cmpt>
inline void expandScalar(Vector<Cmpt>& result, const Cmpt& v)
{
    result = Vector<Cmpt>(v, v, v);
}


//- Return the tensor given a scalar
template <class Cmpt>
inline void expandScalar(Tensor<Cmpt>& result, const Cmpt& v)
{
    result = Tensor<Cmpt>
    (
        v,                    pTraits<Cmpt>::zero,     pTraits<Cmpt>::zero,
        pTraits<Cmpt>::zero,  v,                       pTraits<Cmpt>::zero,
        pTraits<Cmpt>::zero,  pTraits<Cmpt>::zero,     v
    );
}


//- Return the tensor given a diagonal vector
template <class Cmpt>
inline void expandLinear(Tensor<Cmpt>& result, const Vector<Cmpt>& v)
{
    result = Tensor<Cmpt>
    (
        v.x(),                pTraits<Cmpt>::zero,     pTraits<Cmpt>::zero,
        pTraits<Cmpt>::zero,  v.y(),                   pTraits<Cmpt>::zero,
        pTraits<Cmpt>::zero,  pTraits<Cmpt>::zero,     v.z()
    );
}


//- Sum row elements of a tensor as a vector
template <class Cmpt>
inline void sumToDiag(Vector<Cmpt>& result, const Tensor<Cmpt>& t)
{
    result = Vector<Cmpt>
    (
        t.xx() + t.xy() + t.xz(),
        t.yx() + t.yy() + t.yz(),
        t.zx() + t.zy() + t.zz()
    );
}


//- Sum row elements of a hyperTensor as a hyperVector
template <class Cmpt>
inline Vector<Cmpt> sumToDiag(const Tensor<Cmpt>& t)
{
    Vector<Cmpt> result;
    sumToDiag(result, t);
    return result;
}


//- Sum row elements of a tensor as a vector
template <class Cmpt>
inline void sumMagToDiag(Vector<Cmpt>& result, const Tensor<Cmpt>& t)
{
    result = Vector<Cmpt>
    (
        CML::mag(t.xx()) + CML::mag(t.xy()) + CML::mag(t.xz()),
        CML::mag(t.yx()) + CML::mag(t.yy()) + CML::mag(t.yz()),
        CML::mag(t.zx()) + CML::mag(t.zy()) + CML::mag(t.zz())
    );
}


//- Sum magnitudes of row elements of a hyperTensor as a hyperVector
template <class Cmpt>
inline Vector<Cmpt> sumMagToDiag(const Tensor<Cmpt>& t)
{
    Vector<Cmpt> result;
    sumMagToDiag(result, t);
    return result;
}


} // End namespace CML
#endif
