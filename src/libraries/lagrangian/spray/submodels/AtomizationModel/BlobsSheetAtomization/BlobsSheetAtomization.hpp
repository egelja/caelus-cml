/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::BlobsSheetAtomization

Description
    Primary Breakup Model for pressure swirl atomizers.

    Accurate description in
    @verbatim
    Z. Han, S. Parrish, P.V. Farrell, R.D. Reitz
    "Modeling Atomization Processes Of Pressure Swirl Hollow-Cone Fuel Sprays"
    Atomization and Sprays, vol. 7, pp. 663-684, 1997

    and

    L. Allocca, G. Bella, A. De Vita, L. Di Angelo
    "Experimental Validation of a GDI Spray Model"
    SAE Technical Paper Series, 2002-01-1137
    @endverbatim


\*---------------------------------------------------------------------------*/

#ifndef BlobsSheetAtomization_H
#define BlobsSheetAtomization_H

#include "AtomizationModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                   Class BlobsSheetAtomization Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class BlobsSheetAtomization
:
    public AtomizationModel<CloudType>
{
private:

    scalar B_;
    scalar angle_;


public:

    //- Runtime type information
    TypeName("blobsSheetAtomization");


    // Constructors

        //- Construct from dictionary
        BlobsSheetAtomization(const dictionary& dict, CloudType& cloud);

        //- Construct copy
        BlobsSheetAtomization(const BlobsSheetAtomization<CloudType>& am);

        //- Construct and return a clone
        virtual autoPtr<AtomizationModel<CloudType> > clone() const
        {
            return autoPtr<AtomizationModel<CloudType> >
            (
                new BlobsSheetAtomization<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~BlobsSheetAtomization();


    // Member Functions

        //- Initial value of liquidCore
        virtual scalar initLiquidCore() const;

        //- Flag to indicate if chi needs to be calculated
        virtual bool calcChi() const;

        virtual void update
        (
            const scalar dt,
            scalar& d,
            scalar& liquidCore,
            scalar& tc,
            const scalar rho,
            const scalar mu,
            const scalar sigma,
            const scalar volFlowRate,
            const scalar rhoAv,
            const scalar Urel,
            const vector& pos,
            const vector& injectionPos,
            const scalar pAmbient,
            const scalar chi,
            Random& rndGen
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::BlobsSheetAtomization<CloudType>::BlobsSheetAtomization
(
    const dictionary& dict,
    CloudType& owner
)
:
    AtomizationModel<CloudType>(dict, owner, typeName),
    B_(readScalar(this->coeffDict().lookup("B"))),
    angle_(readScalar(this->coeffDict().lookup("angle")))
{}


template<class CloudType>
CML::BlobsSheetAtomization<CloudType>::BlobsSheetAtomization
(
    const BlobsSheetAtomization<CloudType>& am
)
:
    AtomizationModel<CloudType>(am),
    B_(am.B_),
    angle_(am.angle_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::BlobsSheetAtomization<CloudType>::~BlobsSheetAtomization()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::BlobsSheetAtomization<CloudType>::initLiquidCore() const
{
    return 1.0;
}


template<class CloudType>
bool CML::BlobsSheetAtomization<CloudType>::calcChi() const
{
    return false;
}


template<class CloudType>
void CML::BlobsSheetAtomization<CloudType>::update
(
    const scalar dt,
    scalar& d,
    scalar& liquidCore,
    scalar& tc,
    const scalar rho,
    const scalar mu,
    const scalar sigma,
    const scalar volFlowRate,
    const scalar rhoAv,
    const scalar Urel,
    const vector& pos,
    const vector& injectionPos,
    const scalar pAmbient,
    const scalar chi,
    Random& rndGen
) const
{
    scalar lBU =
        B_
      * sqrt
        (
            rho*sigma*d*cos(angle_*constant::mathematical::pi/360.0)
          / sqr(rhoAv*Urel)
        );

    scalar pWalk = mag(pos - injectionPos);

    if (pWalk > lBU)
    {
        liquidCore = 0.0;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
