/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2015 H. Jasak
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
    Global functions for expansion and contraction of tensor field
    to diagonal type

    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved
    Darrin Stephens

SourceFiles
    ExpandHyperTensorField.cpp

\*---------------------------------------------------------------------------*/

#ifndef expandHyperTensorField_HPP
#define expandHyperTensorField_HPP

#include "Field.hpp"
#include "hyperVectorFieldTypes.hpp"


#define UNARY_FUNCTION(typeF1, typeF2, FUNC)                                  \
void FUNC(Field<typeF1>& f1, const UList<typeF2>& f2);                        \
void FUNC(Field<typeF1>& f1, const tmp<Field<typeF2> >& tf2);


#define ExpandFieldFunctions(tensorType, vectorType, cmptType, args...)       \
                                                                              \
UNARY_FUNCTION(cmptType, tensorType, contractScalar)                          \
UNARY_FUNCTION(cmptType, vectorType, contractScalar)                          \
                                                                              \
UNARY_FUNCTION(vectorType, tensorType, contractLinear)                        \
                                                                              \
UNARY_FUNCTION(vectorType, cmptType, expandScalar)                            \
UNARY_FUNCTION(tensorType, cmptType, expandScalar)                            \
                                                                              \
UNARY_FUNCTION(tensorType, vectorType, expandLinear)                          \
                                                                              \
UNARY_FUNCTION(vectorType, tensorType, sumToDiag)                             \
UNARY_FUNCTION(vectorType, tensorType, sumMagToDiag)


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

namespace CML
{

forAllHyperVectorTensorTypes(ExpandFieldFunctions)


} // End namespace CML


#undef UNARY_FUNCTION
#undef ExpandFieldFunctions
#endif
