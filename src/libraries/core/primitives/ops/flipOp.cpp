/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2016 OpenCML Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "flipOp.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<>
CML::scalar CML::flipOp::operator()(const scalar& v) const
{
    return -v;
}


template<> CML::vector CML::flipOp::operator()(const vector& v) const
{
    return -v;
}


template<>CML::sphericalTensor CML::flipOp::operator()
(
    const sphericalTensor& v
) const
{
    return -v;
}


template<> CML::symmTensor CML::flipOp::operator()
(
    const symmTensor& v
) const
{
    return -v;
}


template<> CML::tensor CML::flipOp::operator()(const tensor& v) const
{
    return -v;
}


template<> CML::triad CML::flipOp::operator()
(
    const triad& v
) const
{
    return -v;
}


// ************************************************************************* //
