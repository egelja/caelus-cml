/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
Copyright (C) 2019 Applied CCM Pty Ltd
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

#include "mxLESdelta.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineRunTimeSelectionTable(LESdelta, dictionary);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

LESdelta::LESdelta(const word& name, const fvMesh& mesh)
:
    cmlLESdelta(name, mesh)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<cmlLESdelta> LESdelta::New
(
    const word& name,
    const fvMesh& mesh,
    const dictionary& dict
)
{
    return cmlLESdelta::New(name, mesh, dict, *dictionaryConstructorTablePtr_);
}


} // End namespace mixture
} // End namespace CML
