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

inline const CML::radiation::radiativeIntensityRay&
CML::radiation::fvDOM::IRay(const label rayI) const
{
    return  IRay_[rayI];
}


inline const CML::volScalarField&
CML::radiation::fvDOM::IRayLambda
(
    const label rayI,
    const label lambdaI
) const
{
    return IRay_[rayI].ILambda(lambdaI);
}


inline CML::label CML::radiation::fvDOM::nTheta() const
{
    return nTheta_;
}


inline CML::label CML::radiation::fvDOM::nPhi() const
{
    return nPhi_;
}


inline CML::label CML::radiation::fvDOM::nRay() const
{
    return nRay_;
}


inline CML::label CML::radiation::fvDOM::nLambda() const
{
    return nLambda_;
}


inline const CML::volScalarField& CML::radiation::fvDOM::a() const
{
    return a_;
}


inline const CML::volScalarField& CML::radiation::fvDOM::aLambda
(
    const label lambdaI
) const
{
    return aLambda_[lambdaI];
}


inline const CML::volScalarField& CML::radiation::fvDOM::G() const
{
    return G_;
}


inline const CML::volScalarField& CML::radiation::fvDOM::Qr() const
{
    return Qr_;
}

inline const CML::volScalarField& CML::radiation::fvDOM::Qin() const
{
    return Qin_;
}


inline const CML::volScalarField& CML::radiation::fvDOM::Qem() const
{
    return Qem_;
}

inline const CML::radiation::blackBodyEmission&
CML::radiation::fvDOM::blackBody() const
{
    return blackBody_;
}


// ************************************************************************* //
