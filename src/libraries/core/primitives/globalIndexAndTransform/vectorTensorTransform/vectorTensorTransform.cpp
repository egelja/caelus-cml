/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "vectorTensorTransform.hpp"
#include "IOstreams.hpp"
#include "OStringStream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const CML::vectorTensorTransform::typeName =
    "vectorTensorTransform";

const CML::vectorTensorTransform CML::vectorTensorTransform::zero
(
    vector::zero,
    tensor::zero,
    false
);


const CML::vectorTensorTransform CML::vectorTensorTransform::I
(
    vector::zero,
    sphericalTensor::I,
    false
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::vectorTensorTransform::vectorTensorTransform(Istream& is)
{
    is  >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::word CML::name(const vectorTensorTransform& s)
{
    OStringStream buf;

    buf << '(' << s.t() << ',' << s.R() << ')';

    return buf.str();
}


template<>
CML::tmp<CML::Field<bool> > CML::vectorTensorTransform::transform
(
    const Field<bool>& fld
) const
{
    return fld;
}
template<>
CML::tmp<CML::Field<CML::label> > CML::vectorTensorTransform::transform
(
    const Field<label>& fld
) const
{
    return fld;
}
template<>
CML::tmp<CML::Field<CML::scalar> > CML::vectorTensorTransform::transform
(
    const Field<scalar>& fld
) const
{
    return fld;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, vectorTensorTransform& tr)
{
    // Read beginning of vectorTensorTransform
    is.readBegin("vectorTensorTransform");

    is  >> tr.t_ >> tr.R_ >> tr.hasR_;

    // Read end of vectorTensorTransform
    is.readEnd("vectorTensorTransform");

    // Check state of Istream
    is.check("operator>>(Istream&, vectorTensorTransform&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const vectorTensorTransform& tr)
{
    os  << token::BEGIN_LIST
        << tr.t() << token::SPACE << tr.R() << token::SPACE << tr.hasR()
        << token::END_LIST;

    return os;
}


// ************************************************************************* //
