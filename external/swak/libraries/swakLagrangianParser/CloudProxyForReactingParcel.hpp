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
    CML::CloudProxyForReactingParcel

Description

SourceFiles
    CloudProxyForReactingParcel.cpp

Contributors/Copyright:
    2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef CloudProxyForReactingParcel_H
#define CloudProxyForReactingParcel_H

#include "CloudProxyForThermoParcel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class CloudProxyForReactingParcel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CloudProxyForReactingParcel
:
    public CloudProxyForThermoParcel<CloudType>
{
    //- Disallow default bitwise assignment
    void operator=(const CloudProxyForReactingParcel&);

    //- Disallow copy construct
    CloudProxyForReactingParcel(const CloudProxyForReactingParcel &);

public:

    // Constructors

    //- Construct from components
    CloudProxyForReactingParcel
    (
        const cloud &
    );

    // Destructor

    virtual ~CloudProxyForReactingParcel();

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
CloudProxyForReactingParcel<CloudType>::CloudProxyForReactingParcel
(
    const cloud& c
)
:
    CloudProxyForThermoParcel<CloudType>(c)
{
    typedef CloudProxyForReactingParcel<CloudType> baseType;

    this->addScalarFunction(
        "mass0",
        "Initial mass",
        new typename baseType::template ParticleMethodWrapperValue<scalar>(
            &CloudType::particleType::mass0
        )
    );

    const wordList& phaseTypes = this->theCloud().composition().phaseTypes();
    forAll(phaseTypes,i) {
        const word &name=phaseTypes[i];
        this->addScalarFunction(
            "Y"+name,
            "Mass fraction of "+name,
            new typename baseType::template ParticleMethodWrapperFieldElement<scalar>(
                &CloudType::particleType::Y,
                i
            )
        );
    }


    //- constant Properties
    const typename CloudType::particleType::constantProperties &constProps=
        this->theCloud().constProps();

    this->addScalarFunction(
        "pMin",
        "Minimum pressure (constant)",
        new typename baseType::template ParticleMethodWrapperConstant<scalar>(
            constProps.pMin()
        )
    );
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CloudProxyForReactingParcel<CloudType>::~CloudProxyForReactingParcel()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


} // namespace end

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
