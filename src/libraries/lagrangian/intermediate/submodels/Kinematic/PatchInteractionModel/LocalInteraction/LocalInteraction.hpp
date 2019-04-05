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
    CML::LocalInteraction

Description
    Patch interaction specified on a patch-by-patch basis

\*---------------------------------------------------------------------------*/

#ifndef LocalInteraction_H
#define LocalInteraction_H

#include "PatchInteractionModel.hpp"
#include "patchInteractionDataList.hpp"
#include "Switch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                      Class LocalInteraction Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class LocalInteraction
:
    public PatchInteractionModel<CloudType>
{
    // Private data

        //- List of participating patches
        const patchInteractionDataList patchData_;


        // Counters for particle fates

            //- Number of parcels escaped
            List<label> nEscape_;

            //- Mass of parcels escaped
            List<scalar> massEscape_;

            //- Number of parcels stuck to patches
            List<label> nStick_;

            //- Mass of parcels stuck to patches
            List<scalar> massStick_;


        //- Flag to output data as fields
        Switch writeFields_;

        //- Mass escape field
        autoPtr<volScalarField> massEscapePtr_;

        //- Mass stick field
        autoPtr<volScalarField> massStickPtr_;


public:

    //- Runtime type information
    TypeName("localInteraction");


    // Constructors

        //- Construct from dictionary
        LocalInteraction(const dictionary& dict, CloudType& owner);

        //- Construct copy from owner cloud and patch interaction model
        LocalInteraction(const LocalInteraction<CloudType>& pim);

        //- Construct and return a clone using supplied owner cloud
        virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
        {
            return autoPtr<PatchInteractionModel<CloudType> >
            (
                new LocalInteraction<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~LocalInteraction();


    // Member Functions

        //- Return access to the massEscape field
        volScalarField& massEscape();

        //- Return access to the massStick field
        volScalarField& massStick();

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

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::LocalInteraction<CloudType>::LocalInteraction
(
    const dictionary& dict,
    CloudType& cloud
)
:
    PatchInteractionModel<CloudType>(dict, cloud, typeName),
    patchData_(cloud.mesh(), this->coeffDict()),
    nEscape_(patchData_.size(), 0),
    massEscape_(patchData_.size(), 0.0),
    nStick_(patchData_.size(), 0),
    massStick_(patchData_.size(), 0.0),
    writeFields_(this->coeffDict().lookupOrDefault("writeFields", false)),
    massEscapePtr_(nullptr),
    massStickPtr_(nullptr)
{
    if (writeFields_)
    {
        word massEscapeName(this->owner().name() + ":massEscape");
        word massStickName(this->owner().name() + ":massStick");
        Info<< "    Interaction fields will be written to " << massEscapeName
            << " and " << massStickName << endl;

        (void)massEscape();
        (void)massStick();
    }
    else
    {
        Info<< "    Interaction fields will not be written" << endl;
    }

    // check that interactions are valid/specified
    forAll(patchData_, patchi)
    {
        const word& interactionTypeName =
            patchData_[patchi].interactionTypeName();
        const typename PatchInteractionModel<CloudType>::interactionType& it =
            this->wordToInteractionType(interactionTypeName);

        if (it == PatchInteractionModel<CloudType>::itOther)
        {
            const word& patchName = patchData_[patchi].patchName();
            FatalErrorInFunction
                << "Unknown patch interaction type "
                << interactionTypeName << " for patch " << patchName
                << ". Valid selections are:"
                << this->PatchInteractionModel<CloudType>::interactionTypeNames_
                << nl << exit(FatalError);
        }
    }
}


template<class CloudType>
CML::LocalInteraction<CloudType>::LocalInteraction
(
    const LocalInteraction<CloudType>& pim
)
:
    PatchInteractionModel<CloudType>(pim),
    patchData_(pim.patchData_),
    nEscape_(pim.nEscape_),
    massEscape_(pim.massEscape_),
    nStick_(pim.nStick_),
    massStick_(pim.massStick_),
    writeFields_(pim.writeFields_),
    massEscapePtr_(nullptr),
    massStickPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::LocalInteraction<CloudType>::~LocalInteraction()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class CloudType>
CML::volScalarField& CML::LocalInteraction<CloudType>::massEscape()
{
    if (!massEscapePtr_.valid())
    {
        const fvMesh& mesh = this->owner().mesh();

        massEscapePtr_.reset
        (
            new volScalarField
            (
                IOobject
                (
                    this->owner().name() + ":massEscape",
                    mesh.time().timeName(),
                    mesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimMass, 0.0)
            )
        );
    }

    return massEscapePtr_();
}


template<class CloudType>
CML::volScalarField& CML::LocalInteraction<CloudType>::massStick()
{
    if (!massStickPtr_.valid())
    {
        const fvMesh& mesh = this->owner().mesh();

        massStickPtr_.reset
        (
            new volScalarField
            (
                IOobject
                (
                    this->owner().name() + ":massStick",
                    mesh.time().timeName(),
                    mesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimMass, 0.0)
            )
        );
    }

    return massStickPtr_();
}


template<class CloudType>
bool CML::LocalInteraction<CloudType>::correct
(
    typename CloudType::parcelType& p,
    const polyPatch& pp,
    bool& keepParticle
)
{
    label patchi = patchData_.applyToPatch(pp.index());

    if (patchi >= 0)
    {
        vector& U = p.U();
        bool& active = p.active();

        typename PatchInteractionModel<CloudType>::interactionType it =
            this->wordToInteractionType
            (
                patchData_[patchi].interactionTypeName()
            );

        switch (it)
        {
            case PatchInteractionModel<CloudType>::itNone:
            {
                return false;
            }
            case PatchInteractionModel<CloudType>::itEscape:
            {
                scalar dm = p.mass()*p.nParticle();

                keepParticle = false;
                active = false;
                U = Zero;
                nEscape_[patchi]++;
                massEscape_[patchi] += dm;
                if (writeFields_)
                {
                    label pI = pp.index();
                    label fI = pp.whichFace(p.face());
                    massEscape().boundaryField()[pI][fI] += dm;
                }
                break;
            }
            case PatchInteractionModel<CloudType>::itStick:
            {
                scalar dm = p.mass()*p.nParticle();

                keepParticle = true;
                active = false;
                U = Zero;
                nStick_[patchi]++;
                massStick_[patchi] += dm;
                if (writeFields_)
                {
                    label pI = pp.index();
                    label fI = pp.whichFace(p.face());
                    massStick().boundaryField()[pI][fI] += dm;
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
                    U -= (1.0 + patchData_[patchi].e())*Un*nw;
                }

                U -= patchData_[patchi].mu()*Ut;

                // Return velocity to global space
                U += Up;

                break;
            }
            default:
            {
                FatalErrorInFunction
                    << "Unknown interaction type "
                    << patchData_[patchi].interactionTypeName()
                    << "(" << it << ") for patch "
                    << patchData_[patchi].patchName()
                    << ". Valid selections are:" << this->interactionTypeNames_
                    << endl << abort(FatalError);
            }
        }

        return true;
    }

    return false;
}


template<class CloudType>
void CML::LocalInteraction<CloudType>::info(Ostream& os)
{
    // retrieve any stored data
    labelList npe0(patchData_.size(), 0);
    this->getModelProperty("nEscape", npe0);

    scalarList mpe0(patchData_.size(), 0.0);
    this->getModelProperty("massEscape", mpe0);

    labelList nps0(patchData_.size(), 0);
    this->getModelProperty("nStick", nps0);

    scalarList mps0(patchData_.size(), 0.0);
    this->getModelProperty("massStick", mps0);

    // accumulate current data
    labelList npe(nEscape_);
    Pstream::listCombineGather(npe, plusEqOp<label>());
    npe = npe + npe0;

    scalarList mpe(massEscape_);
    Pstream::listCombineGather(mpe, plusEqOp<scalar>());
    mpe = mpe + mpe0;

    labelList nps(nStick_);
    Pstream::listCombineGather(nps, plusEqOp<label>());
    nps = nps + nps0;

    scalarList mps(massStick_);
    Pstream::listCombineGather(mps, plusEqOp<scalar>());
    mps = mps + mps0;


    forAll(patchData_, i)
    {
        os  << "    Parcel fate (number, mass)      : patch "
            <<  patchData_[i].patchName() << nl
            << "      - escape                      = " << npe[i]
            << ", " << mpe[i] << nl
            << "      - stick                       = " << nps[i]
            << ", " << mps[i] << nl;
    }

    if (this->writeTime())
    {
        this->setModelProperty("nEscape", npe);
        nEscape_ = 0;

        this->setModelProperty("massEscape", mpe);
        massEscape_ = 0.0;

        this->setModelProperty("nStick", nps);
        nStick_ = 0;

        this->setModelProperty("massStick", mps);
        massStick_ = 0.0;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
