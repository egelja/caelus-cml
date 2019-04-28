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
    CML::ErgunWenYuDragForce

Description
    Ergun-Wen-Yu drag model for solid spheres.

    Reference:
    \verbatim
        Gidaspow, D. (1994).
        Multiphase flow and fluidization: continuum and kinetic theory
        descriptions.
        Academic press.
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef ErgunWenYuDragForce_HPP
#define ErgunWenYuDragForce_HPP

#include "WenYuDragForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class ErgunWenYuDragForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ErgunWenYuDragForce
:
    public WenYuDragForce<CloudType>
{
public:

    //- Runtime type information
    TypeName("ErgunWenYuDrag");


    // Constructors

        //- Construct from mesh
        ErgunWenYuDragForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        ErgunWenYuDragForce(const ErgunWenYuDragForce<CloudType>& df);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType>> clone() const
        {
            return autoPtr<ParticleForce<CloudType>>
            (
                new ErgunWenYuDragForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ErgunWenYuDragForce();


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
CML::ErgunWenYuDragForce<CloudType>::ErgunWenYuDragForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    WenYuDragForce<CloudType>(owner, mesh, dict, typeName)
{}


template<class CloudType>
CML::ErgunWenYuDragForce<CloudType>::ErgunWenYuDragForce
(
    const ErgunWenYuDragForce<CloudType>& df
)
:
    WenYuDragForce<CloudType>(df)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ErgunWenYuDragForce<CloudType>::~ErgunWenYuDragForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::forceSuSp CML::ErgunWenYuDragForce<CloudType>::calcCoupled
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

    if (alphac < 0.8)
    {
        return forceSuSp
        (
            Zero,
            mass/p.rho()*(150*(1 - alphac)/alphac + 1.75*Re)
           *muc/(alphac*sqr(p.d()))
        );
    }
    else
    {
        return WenYuDragForce<CloudType>::calcCoupled(p, td, dt, mass, Re, muc);
    }
}


#endif
