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
    CML::DispersionModel

Description

\*---------------------------------------------------------------------------*/

#ifndef DispersionModel_H
#define DispersionModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "CloudSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class DispersionModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class DispersionModel
:
    public CloudSubModelBase<CloudType>
{
public:

    //- Runtime type information
    TypeName("dispersionModel");


    // Declare runtime constructor selection table

    declareRunTimeSelectionTable
    (
        autoPtr,
        DispersionModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        DispersionModel(CloudType& owner);

        //- Construct from components
        DispersionModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        DispersionModel(const DispersionModel<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType>> clone() const = 0;


    //- Destructor
    virtual ~DispersionModel();


    //- Selector
    static autoPtr<DispersionModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


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
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeDispersionModel(CloudType)                                         \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineTemplateTypeNameAndDebug                                             \
        (DispersionModel<kinematicCloudType>, 0);                              \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        DispersionModel<kinematicCloudType>,                                   \
        dictionary                                                             \
    );

#define makeDispersionModelType(SS, CloudType)                                 \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug(SS<kinematicCloudType>, 0);            \
                                                                               \
    DispersionModel<kinematicCloudType>::                                      \
        adddictionaryConstructorToTable<SS<kinematicCloudType> >               \
            add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::DispersionModel<CloudType>::DispersionModel(CloudType& owner)
:
    CloudSubModelBase<CloudType>(owner)
{}


template<class CloudType>
CML::DispersionModel<CloudType>::DispersionModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type)
{}


template<class CloudType>
CML::DispersionModel<CloudType>::DispersionModel
(
    const DispersionModel<CloudType>& dm
)
:
    CloudSubModelBase<CloudType>(dm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::DispersionModel<CloudType>::~DispersionModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::vector CML::DispersionModel<CloudType>::update
(
    const scalar,
    const label,
    const vector&,
    const vector& Uc,
    vector&,
    scalar&
)
{
    NotImplemented;

    return Uc;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::DispersionModel<CloudType> >
CML::DispersionModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    const word modelType(dict.lookup("dispersionModel"));

    Info<< "Selecting dispersion model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown dispersion model type "
            << modelType << nl << nl
            << "Valid dispersion model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<DispersionModel<CloudType> >(cstrIter()(dict, owner));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
