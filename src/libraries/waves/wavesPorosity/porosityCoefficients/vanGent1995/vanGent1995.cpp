/*---------------------------------------------------------------------------*\
Copyright held by original author
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

#include "vanGent1995.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(vanGent1995, 0);
addToRunTimeSelectionTable(porosityCoefficient, vanGent1995, porosityCoefficient);

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

vanGent1995::vanGent1995
(
    const dictionary & poroProp
)
:
    porosityCoefficient(poroProp)
{
    dimensionedScalar d50(poroProperties_.lookup("d50"));

    dimensionedScalar alpha(poroProperties_.lookup("alpha"));

    dimensionedScalar beta(poroProperties_.lookup("beta"));

    dimensionedScalar KC
        (
        	poroProperties_.
        	lookupOrDefault<dimensionedScalar>
            (
            	"KC",
            	dimensionedScalar("KC",dimless, 10000)
            )
        );

//    scalar poro(readScalar(poroProperties_.lookup("porosity")));
    scalar poro(readResistancePorosity(poroProperties_));

    // Compute linear resistance coefficient
    dimensionedVector d(alpha*CML::sqr( 1 - poro )/CML::pow3(poro)/CML::sqr(d50)*vector::one);

    linearCoefficient_.value() = d.value();
    linearCoefficient_.dimensions().reset(d.dimensions());

    // Compute quadratic resistance coefficient
    dimensionedVector f(2.0*beta*(1.0 + 7.5/KC)*(1 - poro)/CML::pow3(poro)/d50*vector::one);

    quadraticCoefficient_.value() = f.value();
    quadraticCoefficient_.dimensions().reset(f.dimensions());

    Info << "Coefficients (vanGent1995): " << linearCoefficient_.value() << "\t" << quadraticCoefficient_.value() << "\n" << endl;
}

vanGent1995::~vanGent1995()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
