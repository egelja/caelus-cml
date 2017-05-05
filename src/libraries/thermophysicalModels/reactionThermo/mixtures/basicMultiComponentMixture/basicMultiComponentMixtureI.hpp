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

inline CML::PtrList<CML::volScalarField>&
CML::basicMultiComponentMixture::Y()
{
    return Y_;
}


inline const CML::PtrList<CML::volScalarField>&
CML::basicMultiComponentMixture::Y() const
{
    return Y_;
}


inline CML::volScalarField& CML::basicMultiComponentMixture::Y(const label i)
{
    return Y_[i];
}


inline const CML::volScalarField& CML::basicMultiComponentMixture::Y
(
    const label i
) const
{
    return Y_[i];
}


inline CML::volScalarField& CML::basicMultiComponentMixture::Y
(
    const word& specieName
)
{
    return Y_[species_[specieName]];
}


inline const CML::volScalarField& CML::basicMultiComponentMixture::Y
(
    const word& specieName
) const
{
    return Y_[species_[specieName]];
}


inline bool CML::basicMultiComponentMixture::contains
(
    const word& specieName
) const
{
    return species_.contains(specieName);
}


inline CML::scalar CML::basicMultiComponentMixture::fres
(
    const scalar ft,
    const scalar stoicRatio
) const
{
    return max(ft - (1.0 - ft)/stoicRatio, 0.0);
}


inline CML::tmp<CML::volScalarField> CML::basicMultiComponentMixture::fres
(
    const volScalarField& ft,
    const dimensionedScalar& stoicRatio
) const
{
    return max(ft - (scalar(1) - ft)/stoicRatio.value(), scalar(0));
}


// ************************************************************************* //
