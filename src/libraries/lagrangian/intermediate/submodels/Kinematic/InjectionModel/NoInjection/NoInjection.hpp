/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::NoInjection

Description
    Place holder for 'none' option


\*---------------------------------------------------------------------------*/

#ifndef NoInjection_H
#define NoInjection_H

#include "InjectionModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class NoInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoInjection
:
    public InjectionModel<CloudType>
{

public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from components
        NoInjection(const dictionary&, CloudType&, const word&);

        //- Construct copy
        NoInjection(const NoInjection<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new NoInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoInjection();


    // Member Functions

        //- Flag to indicate whether model activates injection model
        bool active() const;

        //- Return the end-of-injection time
        scalar timeEnd() const;

        //- Number of parcels to introduce relative to SOI
        virtual label parcelsToInject(const scalar time0, const scalar time1);

        //- Volume of parcels to introduce relative to SOI
        virtual scalar volumeToInject(const scalar time0, const scalar time1);


        // Injection geometry

            //- Set the injection position and owner cell, tetFace and tetPt
            virtual void setPositionAndCell
            (
                const label parcelI,
                const label nParcels,
                const scalar time,
                vector& position,
                label& cellOwner,
                label& tetFacei,
                label& tetPti
            );

            virtual void setProperties
            (
                const label parcelI,
                const label nParcels,
                const scalar time,
                typename CloudType::parcelType& parcel
            );

            //- Flag to identify whether model fully describes the parcel
            virtual bool fullyDescribed() const;

            //- Return flag to identify whether or not injection of parcelI is
            //  permitted
            virtual bool validInjection(const label parcelI);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoInjection<CloudType>::NoInjection
(
    const dictionary&,
    CloudType& owner,
    const word&
)
:
    InjectionModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoInjection<CloudType>::NoInjection(const NoInjection<CloudType>& im)
:
    InjectionModel<CloudType>(im.owner_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoInjection<CloudType>::~NoInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoInjection<CloudType>::active() const
{
    return false;
}


template<class CloudType>
CML::scalar CML::NoInjection<CloudType>::timeEnd() const
{
    return 0.0;
}


template<class CloudType>
CML::label CML::NoInjection<CloudType>::parcelsToInject
(
    const scalar,
    const scalar
)
{
    return 0;
}


template<class CloudType>
CML::scalar CML::NoInjection<CloudType>::volumeToInject
(
    const scalar,
    const scalar
)
{
    return 0.0;
}


template<class CloudType>
void CML::NoInjection<CloudType>::setPositionAndCell
(
    const label,
    const label,
    const scalar,
    vector&,
    label&,
    label&,
    label&
)
{}


template<class CloudType>
void CML::NoInjection<CloudType>::setProperties
(
    const label,
    const label,
    const scalar,
    typename CloudType::parcelType& parcel
)
{
    // set particle velocity
    parcel.U() = Zero;

    // set particle diameter
    parcel.d() = 0.0;
}


template<class CloudType>
bool CML::NoInjection<CloudType>::fullyDescribed() const
{
    return false;
}


template<class CloudType>
bool CML::NoInjection<CloudType>::validInjection(const label)
{
    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
