/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2015 H. Jasak
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

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

SourceFiles
    expandTensorField.cpp


\*---------------------------------------------------------------------------*/

#ifndef expandTensorField_HPP
#define expandTensorField_HPP

#include "tensorField.hpp"

#define TEMPLATE
#include "FieldFunctionsM.hpp"


namespace CML
{

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

void contractScalar(Field<scalar>& res, const UList<tensor>& f);
void contractLinear(Field<vector>& res, const UList<tensor>& f);

void expandScalar(Field<vector>& res, const UList<scalar>& f);

void expandScalar(Field<tensor>& res, const UList<scalar>& f);
void expandLinear(Field<tensor>& res, const UList<vector>& f);

void sumToDiag(Field<vector>& res, const UList<tensor>& f);
void sumMagToDiag(Field<vector>& res, const UList<tensor>& f);

} // End namespace CML
#endif

#include "undefFieldFunctionsM.hpp"
