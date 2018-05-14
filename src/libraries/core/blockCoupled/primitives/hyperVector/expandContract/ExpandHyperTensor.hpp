/*---------------------------------------------------------------------------*\
Copyright 2009-2016 H. Jasak
Copyright 2009-2011 I. Clifford
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

Description
    Global functions for expansion and contraction of hyperTensor coefficient
    to diagonal and scalar type.
    
    This file was obtained from Ivor Clifford at the OpenFOAM Workshop
    in PennState, USA, 2011.
    
    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.
    
Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved
    Ivor Clifford
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef expandHyperTensor_HPP
#define expandHyperTensor_HPP

#include "hyperVector.hpp"
#include "hyperVectorFieldTypes.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define UNARY_TEMPLATE_FUNCTION(ReturnType, Type, Func)                      \
template<class Cmpt, int length>                                             \
inline void Func(ReturnType<Cmpt, length>&, const Type<Cmpt, length>&);

#define UNARY_TEMPLATE_FUNCTION_VS(ReturnType, Func)                         \
template<class Cmpt, int length>                                             \
inline void Func(ReturnType<Cmpt, length>&, const Cmpt&);

#define UNARY_TEMPLATE_FUNCTION_SV(Type, Func)                               \
template<class Cmpt, int length>                                             \
inline void Func(Cmpt&, const Type<Cmpt, length>&);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

UNARY_TEMPLATE_FUNCTION_SV(hyperTensor, contractScalar)
UNARY_TEMPLATE_FUNCTION_SV(hyperVector, contractScalar)

UNARY_TEMPLATE_FUNCTION(hyperVector, hyperTensor, contractLinear)

UNARY_TEMPLATE_FUNCTION_VS(hyperVector, expandScalar)
UNARY_TEMPLATE_FUNCTION_VS(hyperTensor, expandScalar)

UNARY_TEMPLATE_FUNCTION(hyperTensor, hyperVector, expandLinear)

UNARY_TEMPLATE_FUNCTION(hyperVector, hyperTensor, sumToDiag)
UNARY_TEMPLATE_FUNCTION(hyperVector, hyperTensor, sumMagToDiag)

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Return the average of a vector as a scalar
template <class Cmpt, int length>
inline void contractScalar(Cmpt& result, const hyperVector<Cmpt, length>& t)
{
    result = pTraits<Cmpt>::zero;

    for (register int i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        result += t[i];
    }

    // Modified 2009/11/3 by I. Clifford
    result *= 1.0/hyperVector<Cmpt, length>::nComponents;
}


//- Return the Average of a vector as a scalar
template <class Cmpt, int length>
inline Cmpt contractScalar(const hyperVector<Cmpt, length>& t)
{
    Cmpt result;
    contractScalar(result, t);
    return result;
}


//- Return the diagonal of a hyperTensor as a scalar
template <class Cmpt, int length>
inline void contractScalar(Cmpt& result, const hyperTensor<Cmpt, length>& t)
{
    result = pTraits<Cmpt>::zero;

    register int j = 0;
    for (register int i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        result += t[j];
        j += hyperTensor<Cmpt, length>::rowLength + 1;
    }

    // Modified 2009/11/3 by I. Clifford
    result *= 1.0/hyperTensor<Cmpt, length>::rowLength;
}


//- Return the diagonal of a hyperTensor as a scalar
template <class Cmpt, int length>
inline Cmpt contractScalar(const hyperTensor<Cmpt, length>& t)
{
    Cmpt result;
    contractScalar(result, t);
    return result;
}


//- Return the diagonal of a hyperTensor as a hyperVector
template <class Cmpt, int length>
inline void contractLinear
(
    hyperVector<Cmpt, length>& result,
    const hyperTensor<Cmpt, length>& t
)
{
    register int j = 0;
    for (register int i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        result[i] = t[j];
        j += hyperTensor<Cmpt, length>::rowLength + 1;
    }
}


//- Return the diagonal of a hyperTensor as a hyperVector
template <class Cmpt, int length>
inline hyperVector<Cmpt, length> contractLinear
(
    const hyperTensor<Cmpt, length>& t
)
{
    hyperVector<Cmpt, length> result;
    contractLinear(result, t);
    return result;
}


//- Return the hyperVector given a scalar
template <class Cmpt, int length>
inline void expandScalar(hyperVector<Cmpt, length>& result, const Cmpt& v)
{
    for (register int i = 0; i < hyperVector<Cmpt, length>::nComponents; i++)
    {
        result[i] = v;
    }
}


//- Return the hyperTensor given a scalar
template <class Cmpt, int length>
inline void expandScalar(hyperTensor<Cmpt, length>& result, const Cmpt& v)
{
    result = hyperTensor<Cmpt, length>::zero;

    register int j = 0;
    for (register int i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        result[j] = v;
        j += hyperTensor<Cmpt, length>::rowLength + 1;
    }
}


//- Return the hyperTensor given a diagonal hyperVector
template <class Cmpt, int length>
inline void expandLinear
(
    hyperTensor<Cmpt, length>& result,
    const hyperVector<Cmpt, length>& v
)
{
    result = hyperTensor<Cmpt, length>::zero;

    register int j = 0;
    for (register int i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        result[j] = v[i];
        j += hyperTensor<Cmpt, length>::rowLength + 1;
    }
}


//- Sum row elements of a hyperTensor as a hyperVector
template <class Cmpt, int length>
inline void sumToDiag
(
    hyperVector<Cmpt, length>& result,
    const hyperTensor<Cmpt, length>& t
)
{
    result = hyperVector<Cmpt, length>::zero;

    for (register int i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        for (register int j = 0; j < hyperTensor<Cmpt, length>::rowLength; j++)
        {
            result[i] += t(i, j);
        }
    }
}


//- Sum row elements of a hyperTensor as a hyperVector
template <class Cmpt, int length>
inline hyperVector<Cmpt, length> sumToDiag(const hyperTensor<Cmpt, length>& t)
{
    hyperVector<Cmpt, length> result;
    sumToDiag(result, t);
    return result;
}


//- Sum magnitudes of row elements of a hyperTensor as a hyperVector
template <class Cmpt, int length>
inline void sumMagToDiag
(
    hyperVector<Cmpt, length>& result,
    const hyperTensor<Cmpt, length>& t
)
{
    result = hyperVector<Cmpt, length>::zero;

    for (register int i = 0; i < hyperTensor<Cmpt, length>::rowLength; i++)
    {
        for (register int j = 0; j < hyperTensor<Cmpt, length>::rowLength; j++)
        {
            result[i] += CML::mag(t(i, j));
        }
    }
}


//- Sum magnitudes of row elements of a hyperTensor as a hyperVector
template <class Cmpt, int length>
inline hyperVector<Cmpt, length> sumMagToDiag
(
    const hyperTensor<Cmpt, length>& t
)
{
    hyperVector<Cmpt, length> result;
    sumMagToDiag(result, t);
    return result;
}


} // End namespace CML

#undef UNARY_TEMPLATE_FUNCTION
#undef UNARY_TEMPLATE_FUNCTION_VS
#undef UNARY_TEMPLATE_FUNCTION_SV

#endif
