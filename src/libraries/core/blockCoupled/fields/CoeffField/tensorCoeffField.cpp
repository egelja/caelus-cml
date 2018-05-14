/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2016 H. Jasak
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


    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

\*---------------------------------------------------------------------------*/

#include "tensorCoeffField.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::CoeffField<CML::tensor>::CoeffField(const label size)
:
    DecoupledCoeffField<tensor>(size)
{}


CML::CoeffField<CML::tensor>::CoeffField(const CoeffField<tensor>& f)
:
    DecoupledCoeffField<tensor>(f)
{}


CML::CoeffField<CML::tensor>::CoeffField
(
    const DecoupledCoeffField<tensor>& f
)
:
    DecoupledCoeffField<tensor>(f)
{}


CML::CoeffField<CML::tensor>::CoeffField
(
    const tmp<DecoupledCoeffField<tensor> >& tf
)
:
    DecoupledCoeffField<tensor>(tf())
{}


CML::CoeffField<CML::tensor>::CoeffField(Istream& is)
:
    DecoupledCoeffField<tensor>(is)
{}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::CoeffField<CML::tensor>::operator=(const CoeffField<tensor>& f)
{
    DecoupledCoeffField<tensor>::operator=(f);
}


void CML::CoeffField<CML::tensor>::operator=
(
    const tmp<CoeffField<tensor> >& f
)
{
    DecoupledCoeffField<tensor>::operator=(f);
}


void CML::CoeffField<CML::tensor>::operator=
(
    const CoeffField<tensor>::scalarTypeField& f
)
{
    DecoupledCoeffField<tensor>::operator=(f);
}


void CML::CoeffField<CML::tensor>::operator=
(
    const tmp<CoeffField<tensor>::scalarTypeField>& f
)
{
    DecoupledCoeffField<tensor>::operator=(f);
}


void CML::CoeffField<CML::tensor>::operator=
(
    const CoeffField<tensor>::linearTypeField& f
)
{
    DecoupledCoeffField<tensor>::operator=(f);
}


void CML::CoeffField<CML::tensor>::operator=
(
    const tmp<CoeffField<tensor>::linearTypeField>& f
)
{
    DecoupledCoeffField<tensor>::operator=(f);
}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const CoeffField<tensor>& f)
{
    const DecoupledCoeffField<tensor>& df = f;
    return operator<<(os, df);
}


CML::Ostream& CML::operator<<
(
    Ostream& os,
    const tmp<CoeffField<tensor> >& tf
)
{
    const DecoupledCoeffField<tensor>& df = tf();
    os << df;
    tf.clear();
    return os;
}

/* * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * */

template<>
CML::tmp<CML::CoeffField<CML::tensor> > CML::inv
(
    const CoeffField<tensor>& f
)
{
    const DecoupledCoeffField<tensor>& df = f;

    return tmp<CoeffField<tensor> >(new CoeffField<tensor>(inv(df)()));
}


template<>
void CML::multiply
(
    tensorField& f,
    const CoeffField<tensor>& f1,
    const tensor& f2
)
{
    const DecoupledCoeffField<tensor>& df1 = f1;

    multiply(f, df1, f2);
}


template<>
void CML::multiply
(
    tensorField& f,
    const CoeffField<tensor>& f1,
    const tensorField& f2
)
{
    const DecoupledCoeffField<tensor>& df1 = f1;

    multiply(f, df1, f2);
}


template<>
void CML::multiply
(
    tensorField& f,
    const tensorField& f1,
    const CoeffField<tensor>& f2
)
{
    const DecoupledCoeffField<tensor>& df2 = f2;

    multiply(f, f1, df2);
}
