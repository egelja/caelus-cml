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

#include "nativeCaelus.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(nativeCaelus, 0);
addToRunTimeSelectionTable(porosityCoefficient, nativeCaelus, porosityCoefficient);

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

nativeCaelus::nativeCaelus
(
    const dictionary & poroProp
)
:
    porosityCoefficient(poroProp)
{
    dimensionedVector d(poroProperties_.lookup("d"));

    linearCoefficient_.value() = d.value();
    linearCoefficient_.dimensions().reset(d.dimensions());

    dimensionedVector f(poroProperties_.lookup("f"));

    quadraticCoefficient_.value() = f.value();
    quadraticCoefficient_.dimensions().reset(f.dimensions());

    Info << "Coefficients (nativeCaelus): " << linearCoefficient_.value() << "\t" << quadraticCoefficient_.value() << "\n" << endl;
}

nativeCaelus::~nativeCaelus()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
