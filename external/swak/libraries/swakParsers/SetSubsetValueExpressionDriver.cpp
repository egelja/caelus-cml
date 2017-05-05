/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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

Contributors/Copyright:
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>


\*---------------------------------------------------------------------------*/

#include "SetSubsetValueExpressionDriver.hpp"

#include "Random.hpp"

namespace CML {

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(SetSubsetValueExpressionDriver, 0);

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


SetSubsetValueExpressionDriver::SetSubsetValueExpressionDriver(const SetSubsetValueExpressionDriver& orig)
:
    SubsetValueExpressionDriver(orig),
    id_(orig.id_),
    origin_(orig.origin_)
{}

SetSubsetValueExpressionDriver::SetSubsetValueExpressionDriver(
    const dictionary& dict,
    const word &id,
    const SetOrigin origin
)
:
    SubsetValueExpressionDriver(dict),
    id_(id),
    origin_(origin)
{}

SetSubsetValueExpressionDriver::SetSubsetValueExpressionDriver(
        const word &id,
        const SetOrigin origin,
        bool autoInterpolate,
        bool warnAutoInterpolate
)
:
    SubsetValueExpressionDriver(autoInterpolate,warnAutoInterpolate),
    id_(id),
    origin_(origin)
{}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

SetSubsetValueExpressionDriver::~SetSubsetValueExpressionDriver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// ************************************************************************* //

} // namespace
