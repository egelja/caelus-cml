/*---------------------------------------------------------------------------*\
Copyright (C) 2010 I. Clifford
Copyright (C) 2015 H. Jasak
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

#include "hyperTensorFields.hpp"
#include "transformField.hpp"


#define TEMPLATE
#include "FieldFunctionsM.hxx"
#include "ExpandHyperTensor.hpp"

#define hyperTensor_FieldFunctions(tensorType,                               \
                                   vectorType, CmptType, args...)            \
                                                                             \
UNARY_FUNCTION(CmptType, tensorType, det)                                    \
UNARY_FUNCTION(tensorType, tensorType, inv)                                  \
UNARY_FUNCTION(tensorType, tensorType, negSumDiag)                           \
                                                                             \
BINARY_OPERATOR(tensorType, CmptType, tensorType, /, divide)                 \
BINARY_TYPE_OPERATOR(tensorType, CmptType, tensorType, /, divide)            \
                                                                             \
BINARY_OPERATOR(vectorType, vectorType, tensorType, /, divide)               \
BINARY_TYPE_OPERATOR(vectorType, vectorType, tensorType, /, divide)          \
                                                                             \
BINARY_OPERATOR(tensorType, tensorType, tensorType, /, divide)               \
BINARY_TYPE_OPERATOR(tensorType, tensorType, tensorType, /, divide)          \
                                                                             \
BINARY_OPERATOR(tensorType, tensorType, tensorType, +, add)                  \
BINARY_OPERATOR(tensorType, tensorType, tensorType, -, subtract)             \
                                                                             \
BINARY_TYPE_OPERATOR(tensorType, tensorType, tensorType, +, add)             \
BINARY_TYPE_OPERATOR(tensorType, tensorType, tensorType, -, subtract)        \
                                                                             \

namespace CML
{

forAllHyperVectorTensorTypes(hyperTensor_FieldFunctions)

}

#undef hyperTensor_FieldFunctions

#include "undefFieldFunctionsM.hpp"
