/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2013 OpenFOAM Foundation
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
    CML::InjectionModelList

Description
    List of injection models

\*---------------------------------------------------------------------------*/

#ifndef InjectionModelList_H
#define InjectionModelList_H

#include "PtrList.hpp"
#include "InjectionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class InjectionModelList Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class InjectionModelList
:
    public PtrList<InjectionModel<CloudType> >
{
public:

    // Constructors

        //- Construct null from owner
        InjectionModelList(CloudType& owner);

        //- Construct from dictionary and cloud owner
        InjectionModelList(const dictionary& dict, CloudType& owner);

        //- Construct copy
        InjectionModelList(const InjectionModelList<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModelList<CloudType> > clone() const
        {
            return autoPtr<InjectionModelList<CloudType> >
            (
                new InjectionModelList<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~InjectionModelList();



    // Member Functions

        // Access

            //- Return the minimum start-of-injection time
            scalar timeStart() const;

            //- Return the maximum end-of-injection time
            scalar timeEnd() const;

            //- Volume of parcels to introduce relative to SOI
            scalar volumeToInject(const scalar time0, const scalar time1);

            //- Return the average parcel mass
            scalar averageParcelMass();


        // Edit

            //- Set injector locations when mesh is updated
            void updateMesh();


        // Per-injection event functions

            //- Main injection loop
            template<class TrackCloudType>
            void inject
            (
                TrackCloudType& cloud,
                typename CloudType::parcelType::trackingData& td
            );

            //- Main injection loop - steady-state
            template<class TrackCloudType>
            void injectSteadyState
            (
                TrackCloudType& cloud,
                typename CloudType::parcelType::trackingData& td,
                const scalar trackTime
            );


        // I-O

            //- Write injection info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::InjectionModelList<CloudType>::InjectionModelList(CloudType& owner)
:
    PtrList<InjectionModel<CloudType> >()
{}


template<class CloudType>
CML::InjectionModelList<CloudType>::InjectionModelList
(
    const dictionary& dict,
    CloudType& owner
)
:
    PtrList<InjectionModel<CloudType> >()
{
    wordList modelNames(dict.toc());

    Info<< "Constructing particle injection models" << endl;

    if (modelNames.size() > 0)
    {
        this->setSize(modelNames.size());

        label i = 0;
        forAllConstIter(IDLList<entry>, dict, iter)
        {
            const word& model = iter().keyword();
            Info<< "Creating injector: " << model << endl;
            const dictionary& props = iter().dict();

            this->set
            (
                i++,
                InjectionModel<CloudType>::New
                (
                    props,
                    model,
                    props.lookup("type"),
                    owner
                )
            );
        }
    }
    else
    {
        this->setSize(1);

        this->set
        (
            0,
            InjectionModel<CloudType>::New
            (
                dict,
                "none",
                "none",
                owner
            )
        );
    }
}


template<class CloudType>
CML::InjectionModelList<CloudType>::InjectionModelList
(
    const InjectionModelList<CloudType>& iml
)
:
    PtrList<InjectionModel<CloudType> >(iml)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::InjectionModelList<CloudType>::~InjectionModelList()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::InjectionModelList<CloudType>::timeStart() const
{
    scalar minTime = GREAT;
    forAll(*this, i)
    {
        minTime = min(minTime, this->operator[](i).timeStart());
    }

    return minTime;
}


template<class CloudType>
CML::scalar CML::InjectionModelList<CloudType>::timeEnd() const
{
    scalar maxTime = -GREAT;
    forAll(*this, i)
    {
        maxTime = max(maxTime, this->operator[](i).timeEnd());
    }

    return maxTime;
}


template<class CloudType>
CML::scalar CML::InjectionModelList<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    scalar vol = 0.0;
    forAll(*this, i)
    {
        vol += this->operator[](i).volumeToInject(time0, time1);
    }

    return vol;
}


template<class CloudType>
CML::scalar CML::InjectionModelList<CloudType>::averageParcelMass()
{
    scalar mass = 0.0;
    scalar massTotal = 0.0;
    forAll(*this, i)
    {
        scalar mt = this->operator[](i).massTotal();
        mass += mt*this->operator[](i).averageParcelMass();
        massTotal += mt;
    }

    return mass/massTotal;
}


template<class CloudType>
void CML::InjectionModelList<CloudType>::updateMesh()
{
    forAll(*this, i)
    {
        this->operator[](i).updateMesh();
    }
}


template<class CloudType>
template<class TrackCloudType>
void CML::InjectionModelList<CloudType>::inject
(
    TrackCloudType& cloud,
    typename CloudType::parcelType::trackingData& td
)
{
    forAll(*this, i)
    {
        this->operator[](i).inject(cloud, td);
    }
}


template<class CloudType>
template<class TrackCloudType>
void CML::InjectionModelList<CloudType>::injectSteadyState
(
    TrackCloudType& cloud,
    typename CloudType::parcelType::trackingData& td,
    const scalar trackTime
)
{
    forAll(*this, i)
    {
        this->operator[](i).injectSteadyState(cloud, td, trackTime);
    }
}


template<class CloudType>
void CML::InjectionModelList<CloudType>::info(Ostream& os)
{
    forAll(*this, i)
    {
        this->operator[](i).info(os);
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
