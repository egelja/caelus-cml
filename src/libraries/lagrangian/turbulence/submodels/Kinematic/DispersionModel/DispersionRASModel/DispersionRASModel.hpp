/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::DispersionRASModel

Description
    Base class for particle dispersion models based on RAS turbulence.

\*---------------------------------------------------------------------------*/

#ifndef DispersionRASModel_H
#define DispersionRASModel_H

#include "DispersionModel.hpp"
#include "demandDrivenData.hpp"
#include "incompressible/turbulenceModel/turbulenceModel.hpp"
#include "compressible/turbulenceModel/compressibleTurbulenceModel.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class DispersionRASModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class DispersionRASModel
:
    public DispersionModel<CloudType>
{
protected:

    // Protected data

        // Locally cached turbulence fields

            //- Turbulence k
            const volScalarField* kPtr_;

            //- Take ownership of the k field
            mutable bool ownK_;

            //- Turbulence epsilon
            const volScalarField* epsilonPtr_;

            //- Take ownership of the epsilon field
            mutable bool ownEpsilon_;


    // Protected Functions

        //- Return the k field from the turbulence model
        tmp<volScalarField> kModel() const;

        //- Return the epsilon field from the turbulence model
        tmp<volScalarField> epsilonModel() const;


public:

    //- Runtime type information
    TypeName("dispersionRASModel");


    // Constructors

        //- Construct from components
        DispersionRASModel(const dictionary& dict, CloudType& owner);

        //- Construct copy
        DispersionRASModel(const DispersionRASModel<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType> > clone() const = 0;


    //- Destructor
    virtual ~DispersionRASModel();


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


        // I-O

            //- Write
            virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class CloudType>
CML::tmp<CML::volScalarField>
CML::DispersionRASModel<CloudType>::kModel() const
{
    const objectRegistry& obr = this->owner().mesh();
    const word turbName = "turbulenceModel";

    if (obr.foundObject<compressible::turbulenceModel>(turbName))
    {
        const compressible::turbulenceModel& model =
            obr.lookupObject<compressible::turbulenceModel>(turbName);
        return model.k();
    }
    else if (obr.foundObject<incompressible::turbulenceModel>(turbName))
    {
        const incompressible::turbulenceModel& model =
            obr.lookupObject<incompressible::turbulenceModel>(turbName);
        return model.k();
    }
    else
    {
        FatalErrorInFunction
            << "Turbulence model not found in mesh database" << nl
            << "Database objects include: " << obr.sortedToc()
            << abort(FatalError);

        return tmp<volScalarField>(nullptr);
    }
}


template<class CloudType>
CML::tmp<CML::volScalarField>
CML::DispersionRASModel<CloudType>::epsilonModel() const
{
    const objectRegistry& obr = this->owner().mesh();
    const word turbName = "turbulenceModel";

    if (obr.foundObject<compressible::turbulenceModel>(turbName))
    {
        const compressible::turbulenceModel& model =
            obr.lookupObject<compressible::turbulenceModel>(turbName);
        return model.epsilon();
    }
    else if (obr.foundObject<incompressible::turbulenceModel>(turbName))
    {
        const incompressible::turbulenceModel& model =
            obr.lookupObject<incompressible::turbulenceModel>(turbName);
        return model.epsilon();
    }
    else
    {
        FatalErrorInFunction
            << "Turbulence model not found in mesh database" << nl
            << "Database objects include: " << obr.sortedToc()
            << abort(FatalError);

        return tmp<volScalarField>(nullptr);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::DispersionRASModel<CloudType>::DispersionRASModel
(
    const dictionary&,
    CloudType& owner
)
:
    DispersionModel<CloudType>(owner),
    kPtr_(nullptr),
    ownK_(false),
    epsilonPtr_(nullptr),
    ownEpsilon_(false)
{}


template<class CloudType>
CML::DispersionRASModel<CloudType>::DispersionRASModel
(
    const DispersionRASModel<CloudType>& dm
)
:
    DispersionModel<CloudType>(dm),
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
CML::DispersionRASModel<CloudType>::~DispersionRASModel()
{
    cacheFields(false);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::DispersionRASModel<CloudType>::cacheFields(const bool store)
{
    if (store)
    {
        tmp<volScalarField> tk = this->kModel();
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

        tmp<volScalarField> tepsilon = this->epsilonModel();
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
void CML::DispersionRASModel<CloudType>::write(Ostream& os) const
{
    DispersionModel<CloudType>::write(os);

    os.writeKeyword("ownK") << ownK_ << token::END_STATEMENT << endl;
    os.writeKeyword("ownEpsilon") << ownEpsilon_ << token::END_STATEMENT
        << endl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
