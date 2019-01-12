/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2017 OpenFOAM Foundation
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
    CML::MultiInteraction

Description
    Runs multiple patch interaction models in turn. Takes dictionary
    where all the subdictionaries are the interaction models.

        // Exit upon first successful interaction or continue doing other
        // models. Returned nteraction status will be true if there has been any
        // interaction (so logical or)
        oneInteractionOnly true;

        model1
        {
            patchInteractionModel coincidentBaffleInteraction;
            coincidentBaffleInteractionCoeffs
            {
                coincidentPatches
                (
                    (pipetteWall_A pipetteCyclic_half0)
                    (pipetteWall_B pipetteCyclic_half1)
                );
            }
        }
        model2
        {
            patchInteractionModel localInteraction;
            localInteractionCoeffs
            {
                patches
                (
                    cWall
                    {
                        type rebound;
                    }
                    pipetteWall_A
                    {
                        type rebound;
                    }
                    pipetteWall_B
                    {
                        type rebound;
                    }
                );
            }
        }


\*---------------------------------------------------------------------------*/

#ifndef MultiInteraction_H
#define MultiInteraction_H

#include "PatchInteractionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                      Class MultiInteraction Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class MultiInteraction
:
    public PatchInteractionModel<CloudType>
{
    // Private data

        Switch oneInteractionOnly_;

        //- Submodels
        PtrList<PatchInteractionModel<CloudType> > models_;


    // Private Member Functions

        //- Read settings
        bool read(const dictionary&);

public:

    //- Runtime type information
    TypeName("multiInteraction");


    // Constructors

        //- Construct from dictionary
        MultiInteraction(const dictionary& dict, CloudType& cloud);

        //- Construct copy from owner cloud and patch interaction model
        MultiInteraction(const MultiInteraction<CloudType>& pim);

        //- Construct and return a clone using supplied owner cloud
        virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
        {
            return autoPtr<PatchInteractionModel<CloudType> >
            (
                new MultiInteraction<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~MultiInteraction();


    // Member Functions

        //- Flag to indicate whether model activates patch interaction model
        virtual bool active() const;

        //- Apply velocity correction
        //  Returns true if particle remains in same cell
        virtual bool correct
        (
            typename CloudType::parcelType& p,
            const polyPatch& pp,
            bool& keepParticle
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
bool CML::MultiInteraction<CloudType>::read(const dictionary& dict)
{
    // Count dictionaries

    Info<< "Patch interaction model " << typeName << nl
        << "Executing in turn " << endl;

    label nModels = 0;
    forAllConstIter(dictionary, dict, iter)
    {
        if (iter().isDict())
        {
            Info<< "    " << iter().name() << endl;

            nModels++;
        }
    }

    models_.setSize(nModels);
    nModels = 0;
    forAllConstIter(dictionary, dict, iter)
    {
        if (iter().isDict())
        {
            models_.set
            (
                nModels++,
                PatchInteractionModel<CloudType>::New
                (
                    iter().dict(),
                    this->owner()
                )
            );
        }
    }

    oneInteractionOnly_ = Switch(dict.lookup("oneInteractionOnly"));

    if (oneInteractionOnly_)
    {
        Info<< "Stopping upon first model that interacts with particle."
            << nl << endl;
    }
    else
    {
        Info<< "Allowing multiple models to interact."
            << nl << endl;
    }

    return true;
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::MultiInteraction<CloudType>::MultiInteraction
(
    const dictionary& dict,
    CloudType& cloud
)
:
    PatchInteractionModel<CloudType>(dict, cloud, typeName)
{
    read(this->coeffDict());
}


template<class CloudType>
CML::MultiInteraction<CloudType>::MultiInteraction
(
    const MultiInteraction<CloudType>& pim
)
:
    PatchInteractionModel<CloudType>(pim),
    oneInteractionOnly_(pim.oneInteractionOnly_),
    models_(pim.models_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::MultiInteraction<CloudType>::~MultiInteraction()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class CloudType>
bool CML::MultiInteraction<CloudType>::active() const
{
    forAll(models_, i)
    {
        if (models_[i].active())
        {
            return true;
        }
    }
    return false;
}


template<class CloudType>
bool CML::MultiInteraction<CloudType>::correct
(
    typename CloudType::parcelType& p,
    const polyPatch& pp,
    bool& keepParticle
)
{
    label origFacei = p.face();
    label patchi = pp.index();

    bool interacted = false;

    forAll(models_, i)
    {
        bool myInteracted = models_[i].correct
        (
            p,
            this->owner().pMesh().boundaryMesh()[patchi],
            keepParticle
        );

        if (myInteracted && oneInteractionOnly_)
        {
            break;
        }

        interacted = (interacted || myInteracted);


        // Check if perhaps the interaction model has changed patches
        // (CoincidentBaffleInteraction can do this)

        if (p.face() != origFacei)
        {
            origFacei = p.face();
            patchi = p.patch();

            // Interaction model has moved particle off wall?
            if (patchi == -1)
            {
                break;
            }
        }
    }

    return interacted;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
