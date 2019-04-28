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
    CML::ConstantLiftCoefficient

Description
    Constant lift coefficient.

Author
    Darrin W. Stephens


\*---------------------------------------------------------------------------*/

#ifndef ConstantLiftCoefficient_H
#define ConstantLiftCoefficient_H

#include "LiftForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class ConstantLiftCoefficient Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ConstantLiftCoefficient
:
    public LiftForce<CloudType>
{
protected:

    // Protected data

        //- Lift coefficient - typically 0.5
        scalar Cl_;

    // Protected Member Functions

        //- Calculate the lift coefficient
        virtual scalar Cl
        (
            const typename CloudType::parcelType& p,
            const typename CloudType::parcelType::trackingData& td,
            const vector& curlUc,
            const scalar Re,
            const scalar muc
        ) const;


public:

    //- Runtime type information
    TypeName("ConstantLiftCoefficient");


    // Constructors

        //- Construct from mesh
        ConstantLiftCoefficient
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& forceType = typeName
        );

        //- Construct copy
        ConstantLiftCoefficient(const ConstantLiftCoefficient& lf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new ConstantLiftCoefficient<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ConstantLiftCoefficient();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "mathematicalConstants.hpp"

using namespace CML::constant;

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::ConstantLiftCoefficient<CloudType>::ConstantLiftCoefficient::Cl
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const vector& curlUc,
    const scalar Re,
    const scalar muc
) const
{
    return Cl_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ConstantLiftCoefficient<CloudType>::ConstantLiftCoefficient
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& forceType
)
:
    LiftForce<CloudType>(owner, mesh, dict, forceType),
    Cl_(readScalar(this->coeffs().lookup("Cl")))
{}


template<class CloudType>
CML::ConstantLiftCoefficient<CloudType>::ConstantLiftCoefficient
(
    const ConstantLiftCoefficient& lf
)
:
    LiftForce<CloudType>(lf),
    Cl_(lf.Cl_)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ConstantLiftCoefficient<CloudType>::~ConstantLiftCoefficient()
{}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
