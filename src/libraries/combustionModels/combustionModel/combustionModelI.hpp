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

inline const CML::fvMesh& CML::combustionModel::mesh() const
{
    return mesh_;
}


inline const CML::surfaceScalarField& CML::combustionModel::phi() const
{
    if (turbulencePtr_)
    {
        return turbulencePtr_->phi();
    }
    else
    {
        FatalErrorIn
        (
            "const CML::compressible::turbulenceModel& "
            "CML::combustionModel::turbulence() const "
        )   << "turbulencePtr_ is empty. Please use "
            << "combustionModel::setTurbulence "
            << "(compressible::turbulenceModel& )"
            << abort(FatalError);

        return turbulencePtr_->phi();
    }
}


inline const CML::compressible::turbulenceModel&
CML::combustionModel::turbulence() const
{
    if (turbulencePtr_)
    {
        return *turbulencePtr_;
    }
    else
    {
        FatalErrorIn
        (
            "const CML::compressible::turbulenceModel& "
            "CML::combustionModel::turbulence() const "
        )   << "turbulencePtr_ is empty. Please use "
            << "combustionModel::setTurbulence "
            << "(compressible::turbulenceModel& )"
            << abort(FatalError);

        return *turbulencePtr_;
    }
}


inline const CML::Switch& CML::combustionModel::active() const
{
    return active_;
}


inline void CML::combustionModel::setTurbulence
(
    compressible::turbulenceModel& turbModel
)
{
    turbulencePtr_ = &turbModel;
}


inline const CML::dictionary& CML::combustionModel::coeffs() const
{
    return coeffs_;
}

// ************************************************************************* //
