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
    CML::NoSurfaceFilm

Description
    Place holder for 'none' option


\*---------------------------------------------------------------------------*/

#ifndef NoSurfaceFilm_H
#define NoSurfaceFilm_H

#include "SurfaceFilmModel_.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class NoSurfaceFilm Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoSurfaceFilm
:
    public SurfaceFilmModel<CloudType>
{
protected:

    // Protected data

        //- Convenience typedef for parcel type
        typedef typename CloudType::parcelType parcelType;


public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoSurfaceFilm(const dictionary&, CloudType&);

        //- Construct copy
        NoSurfaceFilm(const NoSurfaceFilm<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<SurfaceFilmModel<CloudType> > clone() const
        {
            return autoPtr<SurfaceFilmModel<CloudType> >
            (
                new NoSurfaceFilm<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoSurfaceFilm();


    // Member Functions

        // Evaluation

            //- Flag to indicate whether model activates the surface film model
            virtual bool active() const;

            //- Transfer parcel from cloud to surface film
            //  Returns true if parcel is to be transferred
            virtual bool transferParcel
            (
                parcelType& p,
                const polyPatch& pp,
                bool& keepParticle
            );

            //- Set parcel properties
            virtual void setParcelProperties
            (
                parcelType& p,
                const label filmCelli
            ) const;


        // I-O

            //- Write surface film info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoSurfaceFilm<CloudType>::NoSurfaceFilm
(
    const dictionary&,
    CloudType& owner
)
:
    SurfaceFilmModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoSurfaceFilm<CloudType>::NoSurfaceFilm
(
    const NoSurfaceFilm<CloudType>& sfm
)
:
    SurfaceFilmModel<CloudType>(sfm.owner_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoSurfaceFilm<CloudType>::~NoSurfaceFilm()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoSurfaceFilm<CloudType>::active() const
{
    return false;
}


template<class CloudType>
bool CML::NoSurfaceFilm<CloudType>::transferParcel
(
    parcelType&,
    const polyPatch&,
    bool&
)
{
    return false;
}


template<class CloudType>
void CML::NoSurfaceFilm<CloudType>::setParcelProperties
(
    parcelType&,
    const label
) const
{}


template<class CloudType>
void CML::NoSurfaceFilm<CloudType>::info(Ostream&)
{}


// ************************************************************************* //


#endif

// ************************************************************************* //
