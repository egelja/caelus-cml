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

#include "sphericalTensorCoeffField.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::CoeffField<CML::sphericalTensor>::CoeffField(const label size)
:
    DecoupledCoeffField<sphericalTensor>(size)
{}


CML::CoeffField<CML::sphericalTensor>::CoeffField
(
    const CoeffField<sphericalTensor>& f
)
:
    DecoupledCoeffField<sphericalTensor>(f)
{}


CML::CoeffField<CML::sphericalTensor>::CoeffField
(
    const DecoupledCoeffField<sphericalTensor>& f
)
:
    DecoupledCoeffField<sphericalTensor>(f)
{}


CML::CoeffField<CML::sphericalTensor>::CoeffField
(
    const tmp<DecoupledCoeffField<sphericalTensor> >& tf
)
:
    DecoupledCoeffField<sphericalTensor>(tf())
{}


CML::CoeffField<CML::sphericalTensor>::CoeffField(Istream& is)
:
    DecoupledCoeffField<sphericalTensor>(is)
{}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::CoeffField<CML::sphericalTensor>::operator=
(
    const CoeffField<sphericalTensor>& f
)
{
    DecoupledCoeffField<sphericalTensor>::operator=(f);
}


void CML::CoeffField<CML::sphericalTensor>::operator=
(
    const tmp<CoeffField<sphericalTensor> >& f
)
{
    DecoupledCoeffField<sphericalTensor>::operator=(f);
}


void CML::CoeffField<CML::sphericalTensor>::operator=
(
    const CoeffField<sphericalTensor>::scalarTypeField& f
)
{
    DecoupledCoeffField<sphericalTensor>::operator=(f);
}


void CML::CoeffField<CML::sphericalTensor>::operator=
(
    const tmp<CoeffField<sphericalTensor>::scalarTypeField>& f
)
{
    DecoupledCoeffField<sphericalTensor>::operator=(f);
}


void CML::CoeffField<CML::sphericalTensor>::operator=
(
    const CoeffField<sphericalTensor>::linearTypeField& f
)
{
    DecoupledCoeffField<sphericalTensor>::operator=(f);
}


void CML::CoeffField<CML::sphericalTensor>::operator=
(
    const tmp<CoeffField<sphericalTensor>::linearTypeField>& f
)
{
    DecoupledCoeffField<sphericalTensor>::operator=(f);
}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<
(
    Ostream& os,
    const CoeffField<sphericalTensor>& f
)
{
    const DecoupledCoeffField<sphericalTensor>& df = f;
    return operator<<(os, df);
}


CML::Ostream& CML::operator<<
(
    Ostream& os,
    const tmp<CoeffField<sphericalTensor> >& tf
)
{
    const DecoupledCoeffField<sphericalTensor>& df = tf();
    os << df;
    tf.clear();
    return os;
}

/* * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * */

CML::tmp<CML::CoeffField<CML::sphericalTensor> > CML::inv
(
    const CoeffField<sphericalTensor>& f
)
{
    const DecoupledCoeffField<sphericalTensor>& df = f;

    return tmp<CoeffField<sphericalTensor> >
    (
        new CoeffField<sphericalTensor>(inv(df)())
    );
}


template<>
void CML::multiply
(
    sphericalTensorField& f,
    const CoeffField<sphericalTensor>& f1,
    const sphericalTensor& f2
)
{
    const DecoupledCoeffField<sphericalTensor>& df1 = f1;

    multiply(f, df1, f2);
}


template<>
void CML::multiply
(
    sphericalTensorField& f,
    const CoeffField<sphericalTensor>& f1,
    const sphericalTensorField& f2
)
{
    const DecoupledCoeffField<sphericalTensor>& df1 = f1;

    multiply(f, df1, f2);
}


template<>
void CML::multiply
(
    sphericalTensorField& f,
    const sphericalTensorField& f1,
    const CoeffField<sphericalTensor>& f2
)
{
    const DecoupledCoeffField<sphericalTensor>& df2 = f2;

    multiply(f, f1, df2);
}
