/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

inline const CML::fvMesh& CML::basicChemistryModel::mesh() const
{
    return mesh_;
}


inline CML::Switch CML::basicChemistryModel::chemistry() const
{
    return chemistry_;
}


inline const CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::basicChemistryModel::deltaTChem() const
{
    return deltaTChem_;
}


inline CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::basicChemistryModel::deltaTChem()
{
    return deltaTChem_;
}


// ************************************************************************* //
