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

\*---------------------------------------------------------------------------*/

#include "tensorField.hpp"
#include "expandTensor.hpp"

#define TEMPLATE
#include "FieldFunctionsM.hxx"


namespace CML
{

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

void contractScalar(Field<scalar>& res, const UList<tensor>& f)
{
    forAll (res, i)
    {
        contractScalar(res[i], f[i]);
    }
}


void contractLinear(Field<vector>& res, const UList<tensor>& f)
{
    forAll (res, i)
    {
        contractLinear(res[i], f[i]);
    }
}


void expandScalar(Field<vector>& res, const UList<scalar>& f)
{
    forAll (res, i)
    {
        expandScalar(res[i], f[i]);
    }
}


void expandScalar(Field<tensor>& res, const UList<scalar>& f)
{
    forAll (res, i)
    {
        expandScalar(res[i], f[i]);
    }
}


void expandLinear(Field<tensor>& res, const UList<vector>& f)
{
    forAll (res, i)
    {
        expandLinear(res[i], f[i]);
    }
}


void sumToDiag(Field<vector>& res, const UList<tensor>& f)
{
    forAll (res, i)
    {
        sumToDiag(res[i], f[i]);
    }
}


void sumMagToDiag(Field<vector>& res, const UList<tensor>& f)
{
    forAll (res, i)
    {
        sumMagToDiag(res[i], f[i]);
    }
}


} // End namespace Foam


#include "undefFieldFunctionsM.hpp"
