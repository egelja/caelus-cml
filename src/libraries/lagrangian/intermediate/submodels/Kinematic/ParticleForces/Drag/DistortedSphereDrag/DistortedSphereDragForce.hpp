/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2019 OpenFOAM Foundation
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
    Drag model for distorted spheres.

    Reference:
    \verbatim
        Liu, A. B., Mather, D., & Reitz, R. D. (1993).
        Modeling the effects of drop drag and breakup on fuel sprays.
        SAE Transactions, 83-95.
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
    // Limit the drop distortion to y=0 (sphere) and y=1 (disk)
    const scalar y = min(max(p.y(), 0), 1);

    const scalar CdRe = SphereDragForce<CloudType>::CdRe(Re);

    return forceSuSp
    (
        Zero,
        mass*0.75*muc*CdRe*(1 + 2.632*y)/(p.rho()*sqr(p.d()))
    );
}


#endif
