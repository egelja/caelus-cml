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
    CML::DevolatilisationModel

Description
    Templated devolatilisation model class

SourceFiles
    DevolatilisationModel.cpp
    DevolatilisationModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef DevolatilisationModel_H
#define DevolatilisationModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "CloudSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class DevolatilisationModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class DevolatilisationModel
:
    public CloudSubModelBase<CloudType>
{
protected:

    // Protected data

        //- Mass of lagrangian phase converted
        scalar dMass_;


public:

    //- Runtime type information
    TypeName("devolatilisationModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        DevolatilisationModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        DevolatilisationModel(CloudType& owner);

        //- Construct from dictionary
        DevolatilisationModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        DevolatilisationModel(const DevolatilisationModel<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DevolatilisationModel<CloudType> > clone() const = 0;


    //- Destructor
    virtual ~DevolatilisationModel();


    //- Selector
    static autoPtr<DevolatilisationModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        //- Update model
        virtual void calculate
        (
            const scalar dt,
            const scalar age,
            const scalar mass0,
            const scalar mass,
            const scalar T,
            const scalarField& YGasEff,
            const scalarField& YLiquidEff,
            const scalarField& YSolidEff,
            label& canCombust,
            scalarField& dMassDV
        ) const = 0;

        //- Add to devolatilisation mass
        void addToDevolatilisationMass(const scalar dMass);


        // I-O

            //- Write injection info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeDevolatilisationModel(CloudType)                                   \
                                                                               \
    typedef CloudType::reactingMultiphaseCloudType                             \
        reactingMultiphaseCloudType;                                           \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        DevolatilisationModel<reactingMultiphaseCloudType>,                    \
        0                                                                      \
    );                                                                         \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        DevolatilisationModel<reactingMultiphaseCloudType>,                    \
        dictionary                                                             \
    );


#define makeDevolatilisationModelType(SS, CloudType)                           \
                                                                               \
    typedef CloudType::reactingMultiphaseCloudType                             \
        reactingMultiphaseCloudType;                                           \
    defineNamedTemplateTypeNameAndDebug                                        \
        (SS<reactingMultiphaseCloudType>, 0);                                  \
                                                                               \
    DevolatilisationModel<reactingMultiphaseCloudType>::                       \
        adddictionaryConstructorToTable                                        \
        <SS<reactingMultiphaseCloudType> >      \
        add##SS##CloudType##reactingMultiphaseCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::DevolatilisationModel<CloudType>::DevolatilisationModel
(
    CloudType& owner
)
:
    CloudSubModelBase<CloudType>(owner),
    dMass_(0.0)
{}


template<class CloudType>
CML::DevolatilisationModel<CloudType>::DevolatilisationModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type),
    dMass_(0.0)
{}


template<class CloudType>
CML::DevolatilisationModel<CloudType>::DevolatilisationModel
(
    const DevolatilisationModel<CloudType>& dm
)
:
    CloudSubModelBase<CloudType>(dm),
    dMass_(dm.dMass_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::DevolatilisationModel<CloudType>::~DevolatilisationModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::DevolatilisationModel<CloudType>::addToDevolatilisationMass
(
    const scalar dMass
)
{
    dMass_ += dMass;
}


template<class CloudType>
void CML::DevolatilisationModel<CloudType>::info(Ostream& os)
{
    const scalar mass0 = this->template getBaseProperty<scalar>("mass");
    const scalar massTotal = mass0 + returnReduce(dMass_, sumOp<scalar>());

    Info<< "    Mass transfer devolatilisation  = " << massTotal << nl;

    if (this->writeTime())
    {
        this->setBaseProperty("mass", massTotal);
        dMass_ = 0.0;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class CloudType>
CML::autoPtr<CML::DevolatilisationModel<CloudType> >
CML::DevolatilisationModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    const word modelType(dict.lookup("devolatilisationModel"));

    Info<< "Selecting devolatilisation model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown devolatilisation model type "
            << modelType << nl << nl
            << "Valid devolatilisation model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<DevolatilisationModel<CloudType> >(cstrIter()(dict, owner));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
