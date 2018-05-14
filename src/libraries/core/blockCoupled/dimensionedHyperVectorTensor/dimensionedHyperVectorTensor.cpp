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

#include "dimensionedHyperVectorTensor.hpp"


namespace CML
{


#define UNARY_FUNCTION(returnType, type, fun, text)                            \
inline dimensioned< returnType > fun(const dimensioned< type >& t)             \
{                                                                              \
    return dimensioned< returnType >                                           \
    (                                                                          \
        #text "(" + t.name() + ')',                                            \
        fun(t.dimensions()),                                                   \
        fun(t.value())                                                         \
    );                                                                         \
}


#define BINARY_OPERATOR(returnType, type1, type2, op, text)                    \
dimensioned< returnType > op(const dimensioned< type1 >& dt1,                  \
    const dimensioned< type2 >& dt2)                                           \
{                                                                              \
    return dimensioned<returnType>                                             \
    (                                                                          \
        '(' + dt1.name() + #text + dt2.name() + ')',                           \
        op(dt1.dimensions(), dt2.dimensions()),                                \
        op(dt1.value(), dt2.value())                                           \
    );                                                                         \
}

#define dimensionedType_Funs(tensorType, vectorType, cmptType, args...)        \
UNARY_FUNCTION(tensorType, tensorType, inv, inv)                               \
                                                                               \

    forAllHyperVectorTensorTypes(dimensionedType_Funs)

#undef dimensionedType_Funs
#undef UNARY_FUNCTION
#undef BINARY_OPERATOR


} // End namespace CML
