/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "solidParticleCloud.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTemplateTypeNameAndDebug(Cloud<solidParticle>, 0);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::solidParticle::move
(
    solidParticleCloud& cloud,
    trackingData& td,
    const scalar trackTime
)
{
    td.switchProcessor = false;
    td.keepParticle = true;

    while (td.keepParticle && !td.switchProcessor && stepFraction() < 1)
    {
        if (debug)
        {
            Info<< "Time = " << mesh().time().timeName()
                << " trackTime = " << trackTime
                << " steptFraction() = " << stepFraction() << endl;
        }


        const scalar sfrac = stepFraction();

        const scalar f = 1 - stepFraction();
        trackToAndHitFace(f*trackTime*U_, f, cloud, td);

        const scalar dt = (stepFraction() - sfrac)*trackTime;

        const tetIndices tetIs = this->currentTetIndices();
        scalar rhoc = td.rhoInterp().interpolate(this->coordinates(), tetIs);
        vector Uc = td.UInterp().interpolate(this->coordinates(), tetIs);
        scalar nuc = td.nuInterp().interpolate(this->coordinates(), tetIs);

        scalar rhop = cloud.rhop();
        scalar magUr = mag(Uc - U_);

        scalar ReFunc = 1.0;
        scalar Re = magUr*d_/nuc;

        if (Re > 0.01)
        {
            ReFunc += 0.15*pow(Re, 0.687);
        }

        scalar Dc = (24.0*nuc/d_)*ReFunc*(3.0/4.0)*(rhoc/(d_*rhop));

        U_ = (U_ + dt*(Dc*Uc + (1.0 - rhoc/rhop)*td.g()))/(1.0 + dt*Dc);
    }

    return td.keepParticle;
}


bool CML::solidParticle::hitPatch(solidParticleCloud&, trackingData&)
{
    return false;
}


void CML::solidParticle::hitProcessorPatch
(
    solidParticleCloud&,
    trackingData& td
)
{
    td.switchProcessor = true;
}


void CML::solidParticle::hitWallPatch(solidParticleCloud& cloud, trackingData&)
{
    const vector nw = normal();

    const scalar Un = U_ & nw;
    const vector Ut = U_ - Un*nw;

    if (Un > 0)
    {
        U_ -= (1.0 + cloud.e())*Un*nw;
    }

    U_ -= cloud.mu()*Ut;
}


void CML::solidParticle::transformProperties(const tensor& T)
{
    particle::transformProperties(T);
    U_ = transform(T, U_);
}


void CML::solidParticle::transformProperties(const vector& separation)
{
    particle::transformProperties(separation);
}


// ************************************************************************* //
