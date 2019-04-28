/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak All rights reserved
Copyright (C) 2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus. If not, see <http://www.gnu.org/licenses/>.

Description
    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

\*---------------------------------------------------------------------------*/

#include "scalarBlockCoeff.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::BlockCoeff<CML::scalar>::BlockCoeff()
:
    scalarCoeff_(pTraits<scalar>::zero)
{}


CML::BlockCoeff<CML::scalar>::BlockCoeff(const BlockCoeff<scalar>& f)
:
    scalarCoeff_(f.scalarCoeff_)
{}


CML::BlockCoeff<CML::scalar>::BlockCoeff(Istream& is)
:
    scalarCoeff_(readScalar(is))
{}


CML::BlockCoeff<CML::scalar> CML::BlockCoeff<CML::scalar>::clone() const
{
    return BlockCoeff<scalar>(*this);
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::BlockCoeff<CML::scalar>::~BlockCoeff()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::blockCoeffBase::activeLevel
CML::BlockCoeff<CML::scalar>::activeType() const
{
    return blockCoeffBase::SCALAR;
}


CML::scalar CML::BlockCoeff<CML::scalar>::component(const direction) const
{
    return scalarCoeff_;
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::BlockCoeff<CML::scalar>::operator=(const BlockCoeff<scalar>& f)
{
    if (this == &f)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    scalarCoeff_ = f.scalarCoeff_;
}

// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const BlockCoeff<scalar>& f)
{
    os << f.scalarCoeff_;

    return os;
}
