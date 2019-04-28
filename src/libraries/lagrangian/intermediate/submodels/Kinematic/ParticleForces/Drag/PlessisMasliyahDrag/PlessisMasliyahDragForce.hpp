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
    CML::PlessisMasliyahDragForce

Description
    Plessis-Masliyah drag model for spheres.

    Reference:
    \verbatim
        Du Plessis, J. P., & Masliyah, J. H. (1988).
        Mathematical modelling of flow through consolidated isotropic porous
        media.
        Transport in Porous Media, 3(2), 145-161.
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef PlessisMasliyahDragForce_HPP
#define PlessisMasliyahDragForce_HPP

#include "DenseDragForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class PlessisMasliyahDragForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PlessisMasliyahDragForce
:
    public DenseDragForce<CloudType>
{
public:

    //- Runtime type information
    TypeName("PlessisMasliyahDrag");


    // Constructors

        //- Construct from mesh
        PlessisMasliyahDragForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        PlessisMasliyahDragForce(const PlessisMasliyahDragForce<CloudType>& df);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType>> clone() const
        {
            return autoPtr<ParticleForce<CloudType>>
            (
                new PlessisMasliyahDragForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~PlessisMasliyahDragForce();


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


#include "volFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PlessisMasliyahDragForce<CloudType>::PlessisMasliyahDragForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    DenseDragForce<CloudType>(owner, mesh, dict, typeName)
{}


template<class CloudType>
CML::PlessisMasliyahDragForce<CloudType>::PlessisMasliyahDragForce
(
    const PlessisMasliyahDragForce<CloudType>& df
)
:
    DenseDragForce<CloudType>(df)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PlessisMasliyahDragForce<CloudType>::~PlessisMasliyahDragForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::forceSuSp CML::PlessisMasliyahDragForce<CloudType>::calcCoupled
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

    const scalar cbrtAlphap = pow(1 - alphac, 1.0/3.0);

    const scalar A =
        26.8*pow3(alphac)
       /(
            sqr(cbrtAlphap)
           *(1 - cbrtAlphap)
           *sqr(1 - sqr(cbrtAlphap))
          + SMALL
        );

    const scalar B =
        sqr(alphac)
       /sqr(1 - sqr(cbrtAlphap));

    return forceSuSp
    (
        Zero,
        (mass/p.rho())
       *(A*(1 - alphac)/alphac + B*Re)*muc/(alphac*sqr(p.d()))
    );
}


#endif
