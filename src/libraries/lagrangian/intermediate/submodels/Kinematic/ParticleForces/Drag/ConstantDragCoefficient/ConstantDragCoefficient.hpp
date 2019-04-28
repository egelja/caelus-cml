/*---------------------------------------------------------------------------*\
Copyright (C) 2015-2018 Applied CCM
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

Class
    CML::ConstantDragCoefficient

Description
    Drag model with user suppleid constant drag coefficient

Author
    Darrin W. Stephens

\*---------------------------------------------------------------------------*/

#ifndef ConstantDragCoefficient_H
#define ConstantDragCoefficient_H

#include "ParticleForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class ConstantDragCoefficient Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ConstantDragCoefficient
:
    public ParticleForce<CloudType>
{
    // Private data

        //- Drag coefficient
        scalar Cd_;


public:

    //- Runtime type information
    TypeName("ConstantDragCoefficient");


    // Constructors

        //- Construct from mesh
        ConstantDragCoefficient
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& forceType = typeName
        );

        //- Construct copy
        ConstantDragCoefficient(const ConstantDragCoefficient<CloudType>& df);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new ConstantDragCoefficient<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ConstantDragCoefficient();


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


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ConstantDragCoefficient<CloudType>::ConstantDragCoefficient
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& forceType
)
:
    ParticleForce<CloudType>(owner, mesh, dict, forceType, true),
    Cd_(readScalar(this->coeffs().lookup("Cd")))
{}


template<class CloudType>
CML::ConstantDragCoefficient<CloudType>::ConstantDragCoefficient
(
    const ConstantDragCoefficient<CloudType>& df
)
:
    ParticleForce<CloudType>(df),
    Cd_(df.Cd_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ConstantDragCoefficient<CloudType>::~ConstantDragCoefficient()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::forceSuSp CML::ConstantDragCoefficient<CloudType>::calcCoupled
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

    value.Sp() = mass*0.75*muc*Cd_*Re/(p.rho()*sqr(p.d()));

    return value;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
