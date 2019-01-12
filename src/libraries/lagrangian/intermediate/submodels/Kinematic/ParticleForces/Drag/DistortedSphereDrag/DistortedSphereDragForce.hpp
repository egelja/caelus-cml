/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::DistortedSphereDragForce

Description
    Drag model based on assumption of distorted spheres according to:

    \verbatim
        "Effects of Drop Drag and Breakup on Fuel Sprays"
        Liu, A.B., Mather, D., Reitz, R.D.,
        SAE Paper 930072,
        SAE Transactions, Vol. 102, Section 3, Journal of Engines, 1993,
        pp. 63-95
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef DistortedSphereDragForce_HPP
#define DistortedSphereDragForce_HPP

#include "ParticleForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class DistortedSphereDragForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class DistortedSphereDragForce
:
    public ParticleForce<CloudType>
{
    // Private Member Functions

        //- Drag coefficient multiplied by Reynolds number
        scalar CdRe(const scalar Re) const;


public:

    //- Runtime type information
    TypeName("distortedSphereDrag");


    // Constructors

        //- Construct from mesh
        DistortedSphereDragForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        DistortedSphereDragForce(const DistortedSphereDragForce<CloudType>& df);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType>> clone() const
        {
            return autoPtr<ParticleForce<CloudType>>
            (
                new DistortedSphereDragForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~DistortedSphereDragForce();


    // Member Functions

        // Evaluation

            //- Calculate the coupled force
            virtual forceSuSp calcCoupled
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar dt,
                const scalar mass,
                const scalar Re,
                const scalar muc
            ) const;
};


} // End namespace CML


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::DistortedSphereDragForce<CloudType>::CdRe
(
    const scalar Re
) const
{
    if (Re > 1000.0)
    {
        return 0.424*Re;
    }
    else
    {
        return 24.0*(1.0 + (1.0/6.0)*pow(Re, 2.0/3.0));
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::DistortedSphereDragForce<CloudType>::DistortedSphereDragForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, false)
{}


template<class CloudType>
CML::DistortedSphereDragForce<CloudType>::DistortedSphereDragForce
(
    const DistortedSphereDragForce<CloudType>& df
)
:
    ParticleForce<CloudType>(df)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::DistortedSphereDragForce<CloudType>::~DistortedSphereDragForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::forceSuSp CML::DistortedSphereDragForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value(Zero, 0.0);

    // Limit the drop distortion to y=0 (sphere) and y=1 (disk)
    scalar y = min(max(p.y(), 0), 1);

    value.Sp() = mass*0.75*muc*CdRe(Re)*(1 + 2.632*y)/(p.rho()*sqr(p.d()));

    return value;
}


#endif
