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


inline CML::PtrList<CML::volScalarField>& CML::basicSolidMixture::Y()
{
    return Y_;
}


inline const CML::PtrList<CML::volScalarField>& CML::basicSolidMixture::Y()
const
{
    return Y_;
}


inline CML::volScalarField& CML::basicSolidMixture::Y(const label i)
{
    return Y_[i];
}


inline const CML::volScalarField& CML::basicSolidMixture::Y
(
    const label i
) const
{
    return Y_[i];
}


inline CML::volScalarField& CML::basicSolidMixture::Y(const word& specieName)
{
    return Y_[components_[specieName]];
}


inline const CML::volScalarField& CML::basicSolidMixture::Y
(
    const word& specieName
) const
{
    return Y_[components_[specieName]];
}


inline bool CML::basicSolidMixture::contains(const word& specieName) const
{
    return components_.contains(specieName);
}


// ************************************************************************* //
