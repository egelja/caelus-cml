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

inline const CML::volScalarField&
CML::radiation::radiativeIntensityRay::I() const
{
    return I_;
}


inline const CML::volScalarField&
CML::radiation::radiativeIntensityRay::qr() const
{
    return qr_;
}


inline CML::volScalarField& CML::radiation::radiativeIntensityRay::qr()
{
    return qr_;
}

inline const CML::volScalarField& CML::radiation::
radiativeIntensityRay::qin() const
{
    return qin_;
}


inline CML::volScalarField& CML::radiation::radiativeIntensityRay::qin()
{
    return qin_;
}


inline const CML::volScalarField& CML::radiation::
radiativeIntensityRay::qem() const
{
    return qem_;
}


inline CML::volScalarField& CML::radiation::radiativeIntensityRay::qem()
{
    return qem_;
}


inline const CML::vector& CML::radiation::radiativeIntensityRay::d() const
{
    return d_;
}


inline const CML::vector& CML::radiation::radiativeIntensityRay::dAve() const
{
    return dAve_;
}


inline CML::scalar CML::radiation::radiativeIntensityRay::nLambda() const
{
    return nLambda_;
}


inline CML::scalar CML::radiation::radiativeIntensityRay::phi() const
{
    return phi_;
}


inline CML::scalar CML::radiation::radiativeIntensityRay::theta() const
{
    return theta_;
}


inline CML::scalar CML::radiation::radiativeIntensityRay::omega() const
{
    return omega_;
}


inline const CML::volScalarField&
CML::radiation::radiativeIntensityRay::ILambda
(
    const label lambdaI
) const
{
    return ILambda_[lambdaI];
}


// ************************************************************************* //
