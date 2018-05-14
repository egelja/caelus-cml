/*---------------------------------------------------------------------------*\
Copyright (C) 2010 I. Clifford
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


    This file was obtained from Ivor Clifford at the OpenFOAM Workshop
    in PennState, USA, 2011.

    The original file contained "Copyright held by original author",
    but no author was named in the original file. Authorship confirmed by
    Ivor Clifford (2017).

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Authors
    Ivor Clifford
    Hrvoje Jasak
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#include "hyperVectorFields.hpp"


#define TEMPLATE
#include "FieldFunctionsM.hxx"

#define hyperVector_FieldFunctions(tensorType, vectorType, CmptType, args...) \
                                                                              \
UNARY_FUNCTION(CmptType, vectorType, cmptSum)                                 \
                                                                              \
BINARY_FUNCTION(vectorType, vectorType, vectorType, cmptMultiply)             \
BINARY_TYPE_FUNCTION(vectorType, vectorType, vectorType, cmptMultiply)        \
                                                                              \
BINARY_OPERATOR(vectorType, CmptType, vectorType, /, divide)                  \
BINARY_TYPE_OPERATOR(vectorType, CmptType, vectorType, /, divide)             \
                                                                              \
BINARY_OPERATOR(vectorType, vectorType, vectorType, +, add)                   \
BINARY_OPERATOR(vectorType, vectorType, vectorType, -, subtract)              \
                                                                              \
BINARY_TYPE_OPERATOR(vectorType, vectorType, vectorType, +, add)              \
BINARY_TYPE_OPERATOR(vectorType, vectorType, vectorType, -, subtract)         \


namespace CML
{

forAllHyperVectorTensorTypes(hyperVector_FieldFunctions)

}

#undef hyperVector_FieldFunctions

#include "undefFieldFunctionsM.hpp"
