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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::solidParticle::trackingData::trackingData
(
    const solidParticleCloud& spc,
    const interpolationCellPoint<scalar>& rhoInterp,
    const interpolationCellPoint<vector>& UInterp,
    const interpolationCellPoint<scalar>& nuInterp,
    const vector& g
)
:
    particle::trackingData(spc),
    rhoInterp_(rhoInterp),
    UInterp_(UInterp),
    nuInterp_(nuInterp),
    g_(g)
{}


inline CML::solidParticle::solidParticle
(
    const polyMesh& mesh,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti,
    const scalar d,
    const vector& U
)
:
    particle(mesh, coordinates, celli, tetFacei, tetPti),
    d_(d),
    U_(U)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::interpolationCellPoint<CML::scalar>&
CML::solidParticle::trackingData::rhoInterp() const
{
    return rhoInterp_;
}


inline const CML::interpolationCellPoint<CML::vector>&
CML::solidParticle::trackingData::UInterp() const
{
    return UInterp_;
}


inline const CML::interpolationCellPoint<CML::scalar>&
CML::solidParticle::trackingData::nuInterp() const
{
    return nuInterp_;
}

inline const CML::vector& CML::solidParticle::trackingData::g() const
{
    return g_;
}


inline CML::scalar CML::solidParticle::d() const
{
    return d_;
}


inline const CML::vector& CML::solidParticle::U() const
{
    return U_;
}


// ************************************************************************* //
