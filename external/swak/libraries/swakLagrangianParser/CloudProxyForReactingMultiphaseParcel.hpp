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
    CML::CloudProxyForReactingMultiphaseParcel

Description

SourceFiles
    CloudProxyForReactingMultiphaseParcel.cpp

Contributors/Copyright:
    2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef CloudProxyForReactingMultiphaseParcel_H
#define CloudProxyForReactingMultiphaseParcel_H

#include "CloudProxyForReactingParcel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class CloudProxyForReactingMultiphaseParcel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CloudProxyForReactingMultiphaseParcel
:
    public CloudProxyForReactingParcel<CloudType>
{
    //- Disallow default bitwise assignment
    void operator=(const CloudProxyForReactingMultiphaseParcel&);

    //- Disallow copy construct
    CloudProxyForReactingMultiphaseParcel(const CloudProxyForReactingMultiphaseParcel &);

public:

    // Constructors

    //- Construct from components
    CloudProxyForReactingMultiphaseParcel
    (
        const cloud &
    );

    // Destructor

    virtual ~CloudProxyForReactingMultiphaseParcel();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "DebugOStream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CloudProxyForReactingMultiphaseParcel<CloudType>::CloudProxyForReactingMultiphaseParcel
(
    const cloud& cl
)
:
    CloudProxyForReactingParcel<CloudType>(cl)
{
    typedef CloudProxyForReactingMultiphaseParcel<CloudType> baseType;

    const CloudType &c=this->theCloud();

    const label idGas = c.composition().idGas();
    const wordList& gasNames = c.composition().componentNames(idGas);
    forAll(gasNames,i) {
        const word &name=gasNames[i];
        this->addScalarFunction(
            "Y"+name+"(g)",
            "Mass fraction of "+name+" (gas)",
            new typename baseType::template ParticleMethodWrapperFieldElement<scalar>(
                &CloudType::particleType::YGas,
                i
            )
        );
        this->addScalarFunction(
            "YGas_"+name,
            "Mass fraction of "+name+" (gas) - alias",
            new typename baseType::template ParticleMethodWrapperFieldElement<scalar>(
                &CloudType::particleType::YGas,
                i
            )
        );
    }

    const label idLiquid = c.composition().idLiquid();
    const wordList& liquidNames = c.composition().componentNames(idLiquid);
    forAll(liquidNames,i) {
        const word &name=liquidNames[i];
        this->addScalarFunction(
            "Y"+name+"(l)",
            "Mass fraction of "+name+" (liquid)",
            new typename baseType::template ParticleMethodWrapperFieldElement<scalar>(
                &CloudType::particleType::YLiquid,
                i
            )
        );
        this->addScalarFunction(
            "YLiquid_"+name,
            "Mass fraction of "+name+" (liquid) - alias",
            new typename baseType::template ParticleMethodWrapperFieldElement<scalar>(
                &CloudType::particleType::YLiquid,
                i
            )
        );
    }

    const label idSolid = c.composition().idSolid();
    const wordList& solidNames = c.composition().componentNames(idSolid);
    forAll(solidNames,i) {
        const word &name=solidNames[i];
        this->addScalarFunction(
            "Y"+name+"(s)",
            "Mass fraction of "+name+" (solid)",
            new typename baseType::template ParticleMethodWrapperFieldElement<scalar>(
                &CloudType::particleType::YSolid,
                i
            )
        );
        this->addScalarFunction(
            "YSolid_"+name,
            "Mass fraction of "+name+" (solid) - alias",
            new typename baseType::template ParticleMethodWrapperFieldElement<scalar>(
                &CloudType::particleType::YSolid,
                i
            )
        );
    }

    //- constant Properties
    const typename CloudType::particleType::constantProperties &constProps=
        this->theCloud().constProps();

    this->addScalarFunction(
        "LDevol",
        "Latent heat of devolatilisation (constant)",
        new typename baseType::template ParticleMethodWrapperConstant<scalar>(
            constProps.LDevol()
        )
    );
    this->addScalarFunction(
        "hRetentionCoeff",
        "Fraction of enthalpy retained due to surface reactions(constant)",
        new typename baseType::template ParticleMethodWrapperConstant<scalar>(
            constProps.hRetentionCoeff()
        )
    );
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CloudProxyForReactingMultiphaseParcel<CloudType>::~CloudProxyForReactingMultiphaseParcel()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


} // namespace end

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
