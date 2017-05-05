/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::rhoChemistryModel&
CML::combustionModels::rhoChemistryCombustionModel::pChemistry()
{
    return pChemistry_();
}

inline const CML::rhoChemistryModel&
CML::combustionModels::rhoChemistryCombustionModel::
pChemistry() const
{
    return pChemistry_();
}

inline CML::tmp<CML::volScalarField>
CML::combustionModels::rhoChemistryCombustionModel::rho() const
{
    return pChemistry_->thermo().rho();
}

inline const CML::hsReactionThermo&
CML::combustionModels::rhoChemistryCombustionModel::thermo() const
{
    return pChemistry_->thermo();
}

inline CML::hsReactionThermo&
CML::combustionModels::rhoChemistryCombustionModel::thermo()
{
    return pChemistry_->thermo();
}

// ************************************************************************* //
