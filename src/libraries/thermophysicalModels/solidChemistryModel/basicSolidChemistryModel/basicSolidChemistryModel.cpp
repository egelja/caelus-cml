/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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

#include "basicSolidChemistryModel.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace CML
{
    defineTypeNameAndDebug(basicSolidChemistryModel, 0);
    defineRunTimeSelectionTable(basicSolidChemistryModel, thermo);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::basicSolidChemistryModel::basicSolidChemistryModel
(
    solidReactionThermo& thermo
)
:
    basicChemistryModel(thermo),
    solidThermo_(thermo)
{}


const CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::basicSolidChemistryModel::RR(const label i) const
{
    NotImplemented;
    return (DimensionedField<scalar, volMesh>::null());
}


CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::basicSolidChemistryModel::RR(const label i)
{
    NotImplemented;

    return dynamic_cast<DimensionedField<scalar, volMesh>&>
    (
        const_cast<DimensionedField<scalar, volMesh>& >
        (
            DimensionedField<scalar, volMesh>::null()
        )
    );
}


CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::basicSolidChemistryModel::calculateRR
(
    const label reactionI,
    const label speciei
) const
{
    NotImplemented;

    return dynamic_cast<tmp<DimensionedField<scalar, volMesh> >&>
    (
        const_cast<DimensionedField<scalar, volMesh>& >
        (
            DimensionedField<scalar, volMesh>::null()
        )
    );
}
