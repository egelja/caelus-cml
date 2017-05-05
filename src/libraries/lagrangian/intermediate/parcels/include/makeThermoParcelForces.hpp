/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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

#ifndef makeThermoParcelForces_H
#define makeThermoParcelForces_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "SphereDragForce.hpp"
#include "NonSphereDragForce.hpp"
#include "ConstantDragCoefficient.hpp"

#include "ConstantLiftCoefficient.hpp"
#include "SaffmanMeiLiftForce.hpp"
#include "TomiyamaLiftForce.hpp"

#include "BrownianMotionForce.hpp"
#include "GravityForce.hpp"
#include "NonInertialFrameForce.hpp"
#include "ParamagneticForce.hpp"
#include "PressureGradientForce.hpp"
#include "SRFForce.hpp"
#include "VirtualMassForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeThermoParcelForces(CloudType)                                     \
                                                                              \
    makeParticleForceModel(CloudType);                                        \
    makeParticleForceModelType(SphereDragForce, CloudType);                   \
    makeParticleForceModelType(NonSphereDragForce, CloudType);                \
    makeParticleForceModelType(ConstantDragCoefficient, CloudType);           \
    makeParticleForceModelType(ConstantLiftCoefficient, CloudType);           \
    makeParticleForceModelType(SaffmanMeiLiftForce, CloudType);               \
    makeParticleForceModelType(TomiyamaLiftForce, CloudType);                 \
    makeParticleForceModelType(BrownianMotionForce, CloudType);               \
    makeParticleForceModelType(GravityForce, CloudType);                      \
    makeParticleForceModelType(NonInertialFrameForce, CloudType);             \
    makeParticleForceModelType(ParamagneticForce, CloudType);                 \
    makeParticleForceModelType(PressureGradientForce, CloudType);             \
    makeParticleForceModelType(SRFForce, CloudType);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
