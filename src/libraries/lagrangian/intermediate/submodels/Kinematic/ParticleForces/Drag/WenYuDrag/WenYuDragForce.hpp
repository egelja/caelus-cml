/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2019 OpenFOAM Foundation
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
    CML::WenYuDragForce

Description
    Wen-Yu drag model for spheres.

    Reference:
    \verbatim
        Gidaspow, D. (1994).
        Multiphase flow and fluidization: continuum and kinetic theory
        descriptions.
        Academic press.
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef WenYuDragForce_HPP
#define WenYuDragForce_HPP

#include "DenseDragForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class WenYuDragForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class WenYuDragForce
:
    public DenseDragForce<CloudType>
{
public:

    //- Runtime type information
    TypeName("WenYuDrag");


    // Constructors

        //- Construct from mesh
        WenYuDragForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct from mesh
        WenYuDragForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& typeName
        );

        //- Construct copy
        WenYuDragForce(const WenYuDragForce<CloudType>& df);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType>> clone() const
        {
            return autoPtr<ParticleForce<CloudType>>
            (
                new WenYuDragForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~WenYuDragForce();


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


#include "SchillerNaumannDragForce.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::WenYuDragForce<CloudType>::WenYuDragForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    DenseDragForce<CloudType>(owner, mesh, dict, typeName)
{}


template<class CloudType>
CML::WenYuDragForce<CloudType>::WenYuDragForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& typeName
)
:
    DenseDragForce<CloudType>(owner, mesh, dict, typeName)
{}


template<class CloudType>
CML::WenYuDragForce<CloudType>::WenYuDragForce
(
    const WenYuDragForce<CloudType>& df
)
:
    DenseDragForce<CloudType>(df)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::WenYuDragForce<CloudType>::~WenYuDragForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::forceSuSp CML::WenYuDragForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    const scalar alphac =
        this->alphacInterp().interpolate
        (
            p.coordinates(),
            p.currentTetIndices()
        );
    const scalar CdRe = SchillerNaumannDragForce<CloudType>::CdRe(alphac*Re);

    return forceSuSp
    (
        Zero,
        0.75*(mass/p.rho())*CdRe*muc*pow(alphac, -2.65)/(alphac*sqr(p.d()))
    );
}


#endif
