/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

#include "removeSubDirs.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(removeSubDirs, 0);
addToRunTimeSelectionTable
(
    postProcessingWaves,
    removeSubDirs,
    postProcessingWaves
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


removeSubDirs::removeSubDirs
(
    const Time& rT,
    const dictionary& actionProp,
    const word& action
)
:
    postProcessingWaves(rT, actionProp, action)
{
}


removeSubDirs::~removeSubDirs()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void removeSubDirs::evaluate()
{
    fileName fn(directDir_);

    CML::fileNameList fnl = CML::readDir(fn, CML::fileName::DIRECTORY);

    forAll (fnl, i)
    {
        CML::rmDir(fn + fnl[i]);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
