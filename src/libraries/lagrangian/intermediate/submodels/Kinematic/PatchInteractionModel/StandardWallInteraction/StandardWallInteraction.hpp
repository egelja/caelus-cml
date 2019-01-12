/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2017 OpenFOAM Foundation
Copyright (C) 2015-2018 OpenCFD Ltd.
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
    CML::StandardWallInteraction

Description
    Wall interaction model.

    Three choices:
      - rebound - optionally specify elasticity and restitution coefficients
      - stick   - particles assigned zero velocity
      - escape  - remove particle from the domain

    Example usage:
    \verbatim
    StandardWallInteractionCoeffs
    {
        type        rebound; // stick, escape
        e           1;       // optional - elasticity coeff
        mu          0;       // optional - restitution coeff
    }
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef StandardWallInteraction_H
#define StandardWallInteraction_H

#include "PatchInteractionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                   Class StandardWallInteraction Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class StandardWallInteraction
:
    public PatchInteractionModel<CloudType>
{
protected:

    // Protected data

        // Reference to mesh
        const fvMesh& mesh_;

        //- Interaction type
        typename PatchInteractionModel<CloudType>::interactionType
            interactionType_;

        //- Elasticity coefficient
        scalar e_;

        //- Restitution coefficient
        scalar mu_;


        // Counters for particle fates

            //- Number of parcels escaped
            List<List<label> > nEscape_;

            //- Mass of parcels escaped
            List<List<scalar> > massEscape_;

            //- Number of parcels stuck to patches
            List<List<label> > nStick_;

            //- Mass of parcels stuck to patches
            List<List<scalar> > massStick_;

            //- Flag to output escaped/mass particles sorted by injectorID
            bool outputByInjectorId_;

            //- InjectorId to index map
            Map<label> injIdToIndex_;


public:

    //- Runtime type information
    TypeName("standardWallInteraction");


    // Constructors

        //- Construct from dictionary
        StandardWallInteraction(const dictionary& dict, CloudType& cloud);

        //- Construct copy from owner cloud and patch interaction model
        StandardWallInteraction(const StandardWallInteraction<CloudType>& pim);

        //- Construct and return a clone using supplied owner cloud
        virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
        {
            return autoPtr<PatchInteractionModel<CloudType> >
            (
                new StandardWallInteraction<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~StandardWallInteraction();


    // Member Functions

        //- Apply velocity correction
        //  Returns true if particle remains in same cell
        virtual bool correct
        (
            typename CloudType::parcelType& p,
            const polyPatch& pp,
            bool& keepParticle
        );


        // I-O

            //- Write patch interaction info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::StandardWallInteraction<CloudType>::StandardWallInteraction
(
    const dictionary& dict,
    CloudType& cloud
)
:
    PatchInteractionModel<CloudType>(dict, cloud, typeName),
    mesh_(cloud.mesh()),
    interactionType_
    (
        this->wordToInteractionType(this->coeffDict().lookup("type"))
    ),
    e_(0.0),
    mu_(0.0),
    nEscape_(mesh_.boundaryMesh().nNonProcessor()),
    massEscape_(nEscape_.size()),
    nStick_(nEscape_.size()),
    massStick_(nEscape_.size()),
    outputByInjectorId_
    (
        this->coeffDict().lookupOrDefault("outputByInjectorId", false)
    ),
    injIdToIndex_(cloud.injectors().size())
{
    switch (interactionType_)
    {
        case PatchInteractionModel<CloudType>::itOther:
        {
            const word interactionTypeName(this->coeffDict().lookup("type"));

            FatalErrorInFunction
                << "Unknown interaction result type "
                << interactionTypeName
                << ". Valid selections are:" << this->interactionTypeNames_
                << endl << exit(FatalError);

            break;
        }
        case PatchInteractionModel<CloudType>::itRebound:
        {
            e_ = this->coeffDict().lookupOrDefault("e", 1.0);
            mu_ = this->coeffDict().lookupOrDefault("mu", 0.0);
            break;
        }
        default:
        {}
    }

    forAll(nEscape_, patchi)
    {
        label nInjectors(1);
        if (outputByInjectorId_)
        {
            nInjectors = cloud.injectors().size();
            for (label i=0; i<nInjectors; i++)
            {
                injIdToIndex_.insert(cloud.injectors()[i].injectorID(), i);
            }
        }

        nEscape_[patchi].setSize(nInjectors, 0);
        massEscape_[patchi].setSize(nInjectors, 0.0);
        nStick_[patchi].setSize(nInjectors, 0);
        massStick_[patchi].setSize(nInjectors, 0.0);
    }
}


template<class CloudType>
CML::StandardWallInteraction<CloudType>::StandardWallInteraction
(
    const StandardWallInteraction<CloudType>& pim
)
:
    PatchInteractionModel<CloudType>(pim),
    mesh_(pim.mesh_),
    interactionType_(pim.interactionType_),
    e_(pim.e_),
    mu_(pim.mu_),
    nEscape_(pim.nEscape_),
    massEscape_(pim.massEscape_),
    nStick_(pim.nStick_),
    massStick_(pim.massStick_),
    outputByInjectorId_(pim.outputByInjectorId_),
    injIdToIndex_(pim.injIdToIndex_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::StandardWallInteraction<CloudType>::~StandardWallInteraction()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::StandardWallInteraction<CloudType>::correct
(
    typename CloudType::parcelType& p,
    const polyPatch& pp,
    bool& keepParticle
)
{
    vector& U = p.U();

    bool& active = p.active();

    if (isA<wallPolyPatch>(pp))
    {
        switch (interactionType_)
        {
            case PatchInteractionModel<CloudType>::itNone:
            {
                return false;
            }
            case PatchInteractionModel<CloudType>::itEscape:
            {
                keepParticle = false;
                active = false;
                U = Zero;
                const scalar dm = p.nParticle()*p.mass();
                if (outputByInjectorId_)
                {
                    nEscape_[pp.index()][injIdToIndex_[p.typeId()]]++;
                    massEscape_[pp.index()][injIdToIndex_[p.typeId()]] += dm;
                }
                else
                {
                    nEscape_[pp.index()][0]++;
                    massEscape_[pp.index()][0] += dm;
                }
                break;
            }
            case PatchInteractionModel<CloudType>::itStick:
            {
                keepParticle = true;
                active = false;
                U = Zero;
                const scalar dm = p.nParticle()*p.mass();
                if (outputByInjectorId_)
                {
                    nStick_[pp.index()][injIdToIndex_[p.typeId()]]++;
                    massStick_[pp.index()][injIdToIndex_[p.typeId()]] += dm;
                }
                else
                {
                    nStick_[pp.index()][0]++;
                    massStick_[pp.index()][0] += dm;
                }
                break;
            }
            case PatchInteractionModel<CloudType>::itRebound:
            {
                keepParticle = true;
                active = true;

                vector nw;
                vector Up;

                this->owner().patchData(p, pp, nw, Up);

                // Calculate motion relative to patch velocity
                U -= Up;

                scalar Un = U & nw;
                vector Ut = U - Un*nw;

                if (Un > 0)
                {
                    U -= (1.0 + e_)*Un*nw;
                }

                U -= mu_*Ut;

                // Return velocity to global space
                U += Up;

                break;
            }
            default:
            {
                FatalErrorInFunction
                    << "Unknown interaction type "
                    << this->interactionTypeToWord(interactionType_)
                    << "(" << interactionType_ << ")" << endl
                    << abort(FatalError);
            }
        }

        return true;
    }

    return false;
}


template<class CloudType>
void CML::StandardWallInteraction<CloudType>::info(Ostream& os)
{
    PatchInteractionModel<CloudType>::info(os);

    labelListList npe0(nEscape_);
    this->getModelProperty("nEscape", npe0);

    scalarListList mpe0(massEscape_);
    this->getModelProperty("massEscape", mpe0);

    labelListList nps0(nStick_);
    this->getModelProperty("nStick", nps0);

    scalarListList mps0(massStick_);
    this->getModelProperty("massStick", mps0);

    // accumulate current data
    labelListList npe(nEscape_);

    forAll(npe, i)
    {
        Pstream::listCombineGather(npe[i], plusEqOp<label>());
        npe[i] = npe[i] + npe0[i];
    }

    scalarListList mpe(massEscape_);
    forAll(mpe, i)
    {
        Pstream::listCombineGather(mpe[i], plusEqOp<scalar>());
        mpe[i] = mpe[i] + mpe0[i];
    }

    labelListList nps(nStick_);
    forAll(nps, i)
    {
        Pstream::listCombineGather(nps[i], plusEqOp<label>());
        nps[i] = nps[i] + nps0[i];
    }

    scalarListList mps(massStick_);
    forAll(nps, i)
    {
        Pstream::listCombineGather(mps[i], plusEqOp<scalar>());
        mps[i] = mps[i] + mps0[i];
    }

    if (outputByInjectorId_)
    {
        forAll(npe, i)
        {
            forAll (mpe[i], injId)
            {
                os  << "    Parcel fate: patch " <<  mesh_.boundary()[i].name()
                    << " (number, mass)" << nl
                    << "      - escape  (injector " << injIdToIndex_.toc()[injId]
                    << " )  = " << npe[i][injId]
                    << ", " << mpe[i][injId] << nl
                    << "      - stick   (injector " << injIdToIndex_.toc()[injId]
                    << " )  = " << nps[i][injId]
                    << ", " << mps[i][injId] << nl;
            }
        }
    }
    else
    {
        forAll(npe, i)
        {

            os  << "    Parcel fate: patch (number, mass) "
                << mesh_.boundary()[i].name() << nl
                << "      - escape                      = "
                << npe[i][0] << ", " << mpe[i][0] << nl
                << "      - stick                       = "
                << nps[i][0] << ", " << mps[i][0] << nl;

        }
    }

    if (this->outputTime())
    {
        this->setModelProperty("nEscape", npe);
        nEscape_ = Zero;
        this->setModelProperty("massEscape", mpe);
        massEscape_ = Zero;
        this->setModelProperty("nStick", nps);
        nStick_ = Zero;
        this->setModelProperty("massStick", mps);
        massStick_ = Zero;
    }

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
