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
    CML::PatchInteractionModel

Description
    Templated patch interaction model class


\*---------------------------------------------------------------------------*/

#ifndef PatchInteractionModel_H
#define PatchInteractionModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "polyPatch.hpp"
#include "wallPolyPatch.hpp"
#include "tetIndices.hpp"
#include "CloudSubModelBase.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class PatchInteractionModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PatchInteractionModel
:
    public CloudSubModelBase<CloudType>
{
public:

    // Public enumerations

        // Interaction types
        enum interactionType
        {
            itNone,
            itRebound,
            itStick,
            itEscape,
            itOther
        };

        static wordList interactionTypeNames_;


private:

    // Private data

        //- Name of velocity field - default = "U"
        const word UName_;


public:

    //- Runtime type information
    TypeName("patchInteractionModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        PatchInteractionModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        PatchInteractionModel(CloudType& owner);

        //- Construct from components
        PatchInteractionModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        PatchInteractionModel(const PatchInteractionModel<CloudType>& pim);

        //- Construct and return a clone
        virtual autoPtr<PatchInteractionModel<CloudType> > clone() const = 0;


    //- Destructor
    virtual ~PatchInteractionModel();


    //- Selector
    static autoPtr<PatchInteractionModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Access

        //- Return name of velocity field
        const word& UName() const;


    // Member Functions

        //- Convert interaction result to word
        static word interactionTypeToWord(const interactionType& itEnum);

        //- Convert word to interaction result
        static interactionType wordToInteractionType(const word& itWord);

        //- Apply velocity correction
        //  Returns true if particle remains in same cell
        virtual bool correct
        (
            typename CloudType::parcelType& p,
            const polyPatch& pp,
            bool& keepParticle
        ) = 0;


        // I-O

            //- Write patch interaction info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makePatchInteractionModel(CloudType)                                   \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        PatchInteractionModel<kinematicCloudType>,                             \
        0                                                                      \
    );                                                                         \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        PatchInteractionModel<kinematicCloudType>,                             \
        dictionary                                                             \
    );


#define makePatchInteractionModelType(SS, CloudType)                           \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug(SS<kinematicCloudType>, 0);            \
                                                                               \
    PatchInteractionModel<kinematicCloudType>::                                \
        adddictionaryConstructorToTable<SS<kinematicCloudType> >               \
            add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class CloudType>
CML::wordList CML::PatchInteractionModel<CloudType>::interactionTypeNames_
(
    IStringStream
    (
        "(none rebound stick escape)"
    )()
);

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::word CML::PatchInteractionModel<CloudType>::interactionTypeToWord
(
    const interactionType& itEnum
)
{
    word it = "other";

    switch (itEnum)
    {
        case itNone:
        {
            it = "none";
            break;
        }
        case itRebound:
        {
            it = "rebound";
            break;
        }
        case itStick:
        {
            it = "stick";
            break;
        }
        case itEscape:
        {
            it = "escape";
            break;
        }
        default:
        {
        }
    }

    return it;
}


template<class CloudType>
typename CML::PatchInteractionModel<CloudType>::interactionType
CML::PatchInteractionModel<CloudType>::wordToInteractionType
(
    const word& itWord
)
{
    if (itWord == "none")
    {
        return itNone;
    }
    if (itWord == "rebound")
    {
        return itRebound;
    }
    else if (itWord == "stick")
    {
        return itStick;
    }
    else if (itWord == "escape")
    {
        return itEscape;
    }
    else
    {
        return itOther;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchInteractionModel<CloudType>::PatchInteractionModel
(
    CloudType& owner
)
:
    CloudSubModelBase<CloudType>(owner),
    UName_("unknown_U")
{}


template<class CloudType>
CML::PatchInteractionModel<CloudType>::PatchInteractionModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type),
    UName_(this->coeffDict().lookupOrDefault("U", word("U")))
{}


template<class CloudType>
CML::PatchInteractionModel<CloudType>::PatchInteractionModel
(
    const PatchInteractionModel<CloudType>& pim
)
:
    CloudSubModelBase<CloudType>(pim),
    UName_(pim.UName_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchInteractionModel<CloudType>::~PatchInteractionModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CML::word& CML::PatchInteractionModel<CloudType>::UName() const
{
    return UName_;
}


template<class CloudType>
void CML::PatchInteractionModel<CloudType>::info(Ostream& os)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::PatchInteractionModel<CloudType> >
CML::PatchInteractionModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    const word modelType(dict.lookup("patchInteractionModel"));

    Info<< "Selecting patch interaction model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown patch interaction model type "
            << modelType << nl << nl
            << "Valid patch interaction model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<PatchInteractionModel<CloudType> >(cstrIter()(dict, owner));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
