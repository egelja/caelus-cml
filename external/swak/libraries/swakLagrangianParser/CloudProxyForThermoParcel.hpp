/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2010 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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

Class
    CML::CloudProxyForThermoParcel

Description

SourceFiles
    CloudProxyForThermoParcel.cpp

Contributors/Copyright:
    2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef CloudProxyForThermoParcel_H
#define CloudProxyForThermoParcel_H

#include "CloudProxyForKinematicParcel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class CloudProxyForThermoParcel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CloudProxyForThermoParcel
:
    public CloudProxyForKinematicParcel<CloudType>
{
    //- Disallow default bitwise assignment
    void operator=(const CloudProxyForThermoParcel&);

    //- Disallow copy construct
    CloudProxyForThermoParcel(const CloudProxyForThermoParcel &);

public:

    // Constructors

    //- Construct from components
    CloudProxyForThermoParcel
    (
        const cloud &
    );

    // Destructor

    virtual ~CloudProxyForThermoParcel();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "DebugOStream.hpp"

#include "swakCloudTypes.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CloudProxyForThermoParcel<CloudType>::CloudProxyForThermoParcel
(
    const cloud& c
)
:
    CloudProxyForKinematicParcel<CloudType>(c)
{
    typedef CloudProxyForThermoParcel<CloudType> baseType;

    this->addScalarFunction(
        "T",
        "Temperature",
        new typename baseType::template ParticleMethodWrapperValue<scalar>(
            &CloudType::particleType::T
        )
    );
    this->addScalarFunction(
        "cp",
        "Specific heat capacity",
        new typename baseType::template ParticleMethodWrapperValue<scalar>(
            &CloudType::parcelType::Cp
        )
    );

    //- constant Properties
    const typename CloudType::particleType::constantProperties &constProps=
        this->theCloud().constProps();

    this->addScalarFunction(
        "T0",
        "Initial temperature (constant)",
        new typename baseType::template ParticleMethodWrapperConstant<scalar>(
            constProps.T0()
        )
    );
    this->addScalarFunction(
        "TMin",
        "Minimum temperature (constant)",
        new typename baseType::template ParticleMethodWrapperConstant<scalar>(
            constProps.TMin()
        )
    );
    this->addScalarFunction(
        "cp0",
        "Specific heat capacity (constant)",
        new typename baseType::template ParticleMethodWrapperConstant<scalar>(
            constProps.Cp0()
        )
    );
    this->addScalarFunction(
        "epsilon0",
        "Particle emissivity (constant)",
        new typename baseType::template ParticleMethodWrapperConstant<scalar>(
            constProps.epsilon0()
        )
    );
    this->addScalarFunction(
        "f0",
        "Particle scattering factor (constant)",
        new typename baseType::template ParticleMethodWrapperConstant<scalar>(
            constProps.f0()
        )
    );

}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CloudProxyForThermoParcel<CloudType>::~CloudProxyForThermoParcel()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace end

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
