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

#include "porosityCoefficient.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(porosityCoefficient, 0);
defineRunTimeSelectionTable(porosityCoefficient, porosityCoefficient);

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //


// This method has been added, such that it is possible to have porosity and
// moving meshes at the same time. If resistancePorosity is applied in the
// input dictionary, it is simply possible to apply "porosity 1.0" for the
// hydrodynamic calculations. This option is required for boundedness issues
// in MULES
// NGJ 16/03/2015
scalar porosityCoefficient::readResistancePorosity
(
	const dictionary& dict
) const
{
    if (dict.found("resistancePorosity"))
    {
    	Info << "Resistance coefficients are based on the porosity given by the"
    	     << " keyword 'resistancePorosity'.\n" << endl;
        return readScalar(dict.lookup("resistancePorosity"));
    }
    else
    {
    	Info << "Resistance coefficients are based on the porosity given by the"
    	     << " keyword 'porosity'.\n" << endl;
    	return readScalar(dict.lookup("porosity"));
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

porosityCoefficient::porosityCoefficient
(
    const dictionary & poroProp
)
:
    poroProperties_(poroProp),

    linearCoefficient_(dimensionedVector("null", dimless, vector::zero)),

    quadraticCoefficient_(dimensionedVector("null", dimless, vector::zero)),

    KCQuadraticCoefficient_(dimensionedVector("null", dimless, vector::zero)),

    scaledKC_(dimensionedScalar("null", dimless, 0.0))
{

}


porosityCoefficient::~porosityCoefficient()
{}

autoPtr<porosityCoefficient> porosityCoefficient::New
(
    const dictionary & poroProp
)
{
    word poroForm = poroProp.lookup("resistanceFormulation");

    porosityCoefficientConstructorTable::iterator cstrIter =
            porosityCoefficientConstructorTablePtr_->find(poroForm);

    if (cstrIter == porosityCoefficientConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown resistance formulation: " << poroForm
            << endl << endl
            << "Valid methods are :" << endl
            << porosityCoefficientConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<porosityCoefficient>(cstrIter()(poroProp));
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
