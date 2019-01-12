/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
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
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "recalcThermoHeFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

#include "basicThermo.hpp"

#include "fixedEnergyFvPatchScalarField.hpp"
#include "gradientEnergyFvPatchScalarField.hpp"
#include "mixedEnergyFvPatchScalarField.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

    defineTypeNameAndDebug(recalcThermoHeFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        recalcThermoHeFunctionObject,
        dictionary
    );

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

recalcThermoHeFunctionObject::recalcThermoHeFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    updateSimpleFunctionObject(name,t,dict)
{
}

void recalcThermoHeFunctionObject::recalc()
{
    basicThermo &thermo=const_cast<basicThermo&>(
        obr_.lookupObject<basicThermo>("thermophysicalProperties")
    );
    Info << "Recalculating enthalpy h" << endl;

    const volScalarField &T=thermo.T();
    const volScalarField &p=thermo.p();

    volScalarField &h=thermo.he();

    labelList allCells(T.size());
    forAll(allCells,cellI) {
        allCells[cellI]=cellI;
    }
    h.internalField()=thermo.he(
        p.internalField(),
        T.internalField(),
        allCells
    );
    forAll(h.boundaryField(), patchi)
    {
        h.boundaryField()[patchi] ==
            thermo.he(
                p.boundaryField()[patchi],
                T.boundaryField()[patchi],
                patchi
            );
    }

    // hBoundaryCorrection
    volScalarField::GeometricBoundaryField& hbf = h.boundaryField();

    forAll(hbf, patchi)
    {
        if (isA<gradientEnergyFvPatchScalarField>(hbf[patchi]))
        {
            refCast<gradientEnergyFvPatchScalarField>(hbf[patchi]).gradient()
                = hbf[patchi].fvPatchField::snGrad();
        }
        else if (isA<mixedEnergyFvPatchScalarField>(hbf[patchi]))
        {
            refCast<mixedEnergyFvPatchScalarField>(hbf[patchi]).refGrad()
                = hbf[patchi].fvPatchField::snGrad();
        }
    }

}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace CML


// ************************************************************************* //
