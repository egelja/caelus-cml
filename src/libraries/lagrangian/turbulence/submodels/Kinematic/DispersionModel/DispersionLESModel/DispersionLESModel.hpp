/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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
    CML::DispersionLESModel

Description
    Base class for particle dispersion models based on LES turbulence.

\*---------------------------------------------------------------------------*/

#ifndef DispersionLESModel_H
#define DispersionLESModel_H

#include "DispersionModel.hpp"
#include "LESModel.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class DispersionLESModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class DispersionLESModel
:
    public DispersionModel<CloudType>
{
protected:

    // Protected data

        //- Reference to the incompressible turbulence model
        const incompressible::LESModel& turbulence_;

        // Locally cached turbulence fields

            //- Turbulence k
            const volScalarField* kPtr_;

            //- Take ownership of the k field
            mutable bool ownK_;

            //- Turbulence epsilon
            const volScalarField* epsilonPtr_;

            //- Take ownership of the epsilon field
            mutable bool ownEpsilon_;


public:

    //- Runtime type information
    TypeName("dispersionLESModel");


    // Constructors

        //- Construct from components
        DispersionLESModel(const dictionary& dict, CloudType& owner);

        //- Construct copy
        DispersionLESModel(const DispersionLESModel<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType> > clone() const = 0;


    //- Destructor
    virtual ~DispersionLESModel();


    // Member Functions

        //- Update (disperse particles)
        virtual vector update
        (
            const scalar dt,
            const label celli,
            const vector& U,
            const vector& Uc,
            vector& UTurb,
            scalar& tTurb
        ) = 0;

        //- Cache carrier fields
        virtual void cacheFields(const bool store);

        //- Return const access to the turbulence model
        const incompressible::LESModel& turbulence() const
        {
            return turbulence_;
        }


        // I-O

            //- Write
            virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::DispersionLESModel<CloudType>::DispersionLESModel
(
    const dictionary&,
    CloudType& owner
)
:
    DispersionModel<CloudType>(owner),
    turbulence_
    (
        owner.mesh().objectRegistry::template lookupObject
        <
            incompressible::LESModel
        >
        (
            "LESProperties"
        )
    ),
    kPtr_(nullptr),
    ownK_(false),
    epsilonPtr_(nullptr),
    ownEpsilon_(false)
{}


template<class CloudType>
CML::DispersionLESModel<CloudType>::DispersionLESModel
(
    const DispersionLESModel<CloudType>& dm
)
:
    DispersionModel<CloudType>(dm),
    turbulence_(dm.turbulence_),
    kPtr_(dm.kPtr_),
    ownK_(dm.ownK_),
    epsilonPtr_(dm.epsilonPtr_),
    ownEpsilon_(dm.ownEpsilon_)
{
    dm.ownK_ = false;
    dm.ownEpsilon_ = false;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::DispersionLESModel<CloudType>::~DispersionLESModel()
{
    cacheFields(false);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::DispersionLESModel<CloudType>::cacheFields(const bool store)
{
    if (store)
    {
        tmp<volScalarField> tk = this->turbulence().k();
        if (tk.isTmp())
        {
            kPtr_ = tk.ptr();
            ownK_ = true;
        }
        else
        {
            kPtr_ = tk.operator->();
            ownK_ = false;
        }

        tmp<volScalarField> tepsilon = this->turbulence().epsilon();
        if (tepsilon.isTmp())
        {
            epsilonPtr_ = tepsilon.ptr();
            ownEpsilon_ = true;
        }
        else
        {
            epsilonPtr_ = tepsilon.operator->();
            ownEpsilon_ = false;
        }
    }
    else
    {
        if (ownK_ && kPtr_)
        {
            deleteDemandDrivenData(kPtr_);
            ownK_ = false;
        }
        if (ownEpsilon_ && epsilonPtr_)
        {
            deleteDemandDrivenData(epsilonPtr_);
            ownEpsilon_ = false;
        }
    }
}


template<class CloudType>
void CML::DispersionLESModel<CloudType>::write(Ostream& os) const
{
    DispersionModel<CloudType>::write(os);

    os.writeKeyword("ownK") << ownK_ << token::END_STATEMENT << endl;
    os.writeKeyword("ownEpsilon") << ownEpsilon_ << token::END_STATEMENT
        << endl;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
