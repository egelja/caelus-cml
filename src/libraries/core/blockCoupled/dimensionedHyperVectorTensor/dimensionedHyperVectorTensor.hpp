/*---------------------------------------------------------------------------*\
Copyright (C) 2010 I. Clifford
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

Class
    dimensionedHyperVectorTensor

Description
    Dimensioned hyperVector and hyperTensor obtained from generic dimensioned
    type.

    This file was originally obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Ivor Clifford
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef dimensionedHyperVectorTensor_HPP
#define dimensionedHyperVectorTensor_HPP

#include "dimensionedType.hpp"
#include "hyperVectorFieldTypes.hpp"


namespace CML
{

#define doMakeTypedef(type, Type, args...)      \
    typedef dimensioned<type> dimensioned##Type;

    forAllHyperVectorTypes(doMakeTypedef)

    forAllHyperTensorTypes(doMakeTypedef)
#undef doMakeTypedef


// Friend functions
dimensionSet diag(const dimensionSet& ds);
dimensionSet cmptSum(const dimensionSet& ds);
dimensionSet contractLinear(const dimensionSet& ds);
dimensionSet contractScalar(const dimensionSet& ds);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define UNARY_FUNCTION(returnType, type, fun, text)                            \
inline dimensioned< returnType > fun(const dimensioned< type >& t);


#define BINARY_OPERATOR(returnType, type1, type2, op, text)                    \
dimensioned< returnType > op(const dimensioned< type1 >&,                      \
    const dimensioned< type2 >&);


#define dimensionedType_Funs(tensorType, vectorType, cmptType, args...)        \
                                                                               \
UNARY_FUNCTION(tensorType, tensorType, inv, inv)                               \
                                                                               \
UNARY_FUNCTION(vectorType, vectorType, cmptSum, cmptSum)                       \
                                                                               \

    forAllHyperVectorTensorTypes(dimensionedType_Funs)
#undef dimensionedType_Funs
#undef UNARY_FUNCTION
#undef BINARY_OPERATOR

// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

inline dimensionSet diag(const dimensionSet& ds)
{
    return ds;
}


inline dimensionSet cmptSum(const dimensionSet& ds)
{
    return ds;
}


inline dimensionSet contractLinear(const dimensionSet& ds)
{
    return ds;
}


inline dimensionSet contractScalar(const dimensionSet& ds)
{
    return ds;
}


} // End namespace CML
#endif
