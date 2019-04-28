/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "NSRDSfunc2.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(NSRDSfunc2, 0);
    addToRunTimeSelectionTable(thermophysicalFunction, NSRDSfunc2, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::NSRDSfunc2::NSRDSfunc2
(
    const scalar a,
    const scalar b,
    const scalar c,
    const scalar d
)
:
    a_(a),
    b_(b),
    c_(c),
    d_(d)
{}


CML::NSRDSfunc2::NSRDSfunc2(const dictionary& dict)
:
    a_(readScalar(dict.lookup("a"))),
    b_(readScalar(dict.lookup("b"))),
    c_(readScalar(dict.lookup("c"))),
    d_(readScalar(dict.lookup("d")))
{}
