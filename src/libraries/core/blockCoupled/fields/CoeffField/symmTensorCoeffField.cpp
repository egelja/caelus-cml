/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2016 H. Jasak
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.


    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

\*---------------------------------------------------------------------------*/

#include "symmTensorCoeffField.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::CoeffField<CML::symmTensor>::CoeffField(const label size)
:
    DecoupledCoeffField<symmTensor>(size)
{}


CML::CoeffField<CML::symmTensor>::CoeffField
(
    const CoeffField<symmTensor>& f
)
:
    DecoupledCoeffField<symmTensor>(f)
{}


CML::CoeffField<CML::symmTensor>::CoeffField
(
    const DecoupledCoeffField<symmTensor>& f
)
:
    DecoupledCoeffField<symmTensor>(f)
{}


CML::CoeffField<CML::symmTensor>::CoeffField
(
    const tmp<DecoupledCoeffField<symmTensor> >& tf
)
:
    DecoupledCoeffField<symmTensor>(tf())
{}


CML::CoeffField<CML::symmTensor>::CoeffField(Istream& is)
:
    DecoupledCoeffField<symmTensor>(is)
{}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::CoeffField<CML::symmTensor>::operator=
(
    const CoeffField<symmTensor>& f
)
{
    DecoupledCoeffField<symmTensor>::operator=(f);
}


void CML::CoeffField<CML::symmTensor>::operator=
(
    const tmp<CoeffField<symmTensor> >& f
)
{
    DecoupledCoeffField<symmTensor>::operator=(f);
}


void CML::CoeffField<CML::symmTensor>::operator=
(
    const CoeffField<symmTensor>::scalarTypeField& f
)
{
    DecoupledCoeffField<symmTensor>::operator=(f);
}


void CML::CoeffField<CML::symmTensor>::operator=
(
    const tmp<CoeffField<symmTensor>::scalarTypeField>& f
)
{
    DecoupledCoeffField<symmTensor>::operator=(f);
}


void CML::CoeffField<CML::symmTensor>::operator=
(
    const CoeffField<symmTensor>::linearTypeField& f
)
{
    DecoupledCoeffField<symmTensor>::operator=(f);
}


void CML::CoeffField<CML::symmTensor>::operator=
(
    const tmp<CoeffField<symmTensor>::linearTypeField>& f
)
{
    DecoupledCoeffField<symmTensor>::operator=(f);
}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const CoeffField<symmTensor>& f)
{
    const DecoupledCoeffField<symmTensor>& df = f;
    return operator<<(os, df);
}


CML::Ostream& CML::operator<<
(
    Ostream& os,
    const tmp<CoeffField<symmTensor> >& tf
)
{
    const DecoupledCoeffField<symmTensor>& df = tf();
    os << df;
    tf.clear();
    return os;
}

/* * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * */

CML::tmp<CML::CoeffField<CML::symmTensor> > CML::inv
(
    const CoeffField<symmTensor>& f
)
{
    const DecoupledCoeffField<symmTensor>& df = f;

    return tmp<CoeffField<symmTensor> >
    (
        new CoeffField<symmTensor>(inv(df)())
    );
}


template<>
void CML::multiply
(
    symmTensorField& f,
    const CoeffField<symmTensor>& f1,
    const symmTensor& f2
)
{
    const DecoupledCoeffField<symmTensor>& df1 = f1;

    multiply(f, df1, f2);
}


template<>
void CML::multiply
(
    symmTensorField& f,
    const CoeffField<symmTensor>& f1,
    const symmTensorField& f2
)
{
    const DecoupledCoeffField<symmTensor>& df1 = f1;

    multiply(f, df1, f2);
}


template<>
void CML::multiply
(
    symmTensorField& f,
    const symmTensorField& f1,
    const CoeffField<symmTensor>& f2
)
{
    const DecoupledCoeffField<symmTensor>& df2 = f2;

    multiply(f, f1, df2);
}
