/*---------------------------------------------------------------------------*\
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
    CML::SurfaceReactionModel

Description
    Templated surface reaction model class

SourceFiles
    SurfaceReactionModel.cpp
    SurfaceReactionModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef SurfaceReactionModel_H
#define SurfaceReactionModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "CloudSubModelBase.hpp"
#include "scalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class SurfaceReactionModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class SurfaceReactionModel
:
    public CloudSubModelBase<CloudType>
{
protected:

    // Protected data

        //- Mass of lagrangian phase converted
        scalar dMass_;


public:

    //-Runtime type information
    TypeName("surfaceReactionModel");


    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        SurfaceReactionModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& cloud
        ),
        (dict, cloud)
    );


    // Constructors

        //- Construct null from owner
        SurfaceReactionModel(CloudType& owner);

        //- Construct from dictionary
        SurfaceReactionModel
        (
            const dictionary& dict,
            CloudType& cloud,
            const word& type
        );

        //- Construct copy
        SurfaceReactionModel(const SurfaceReactionModel<CloudType>& srm);

        //- Construct and return a clone
        virtual autoPtr<SurfaceReactionModel<CloudType> > clone() const = 0;


    //- Destructor
    virtual ~SurfaceReactionModel();


    //- Selector
    static autoPtr<SurfaceReactionModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& cloud
    );


    // Member Functions

        //- Update surface reactions
        //  Returns the heat of reaction
        virtual scalar calculate
        (
            const scalar dt,
            const label celli,
            const scalar d,
            const scalar T,
            const scalar Tc,
            const scalar pc,
            const scalar rhoc,
            const scalar mass,
            const scalarField& YGas,
            const scalarField& YLiquid,
            const scalarField& YSolid,
            const scalarField& YMixture,
            const scalar N,
            scalarField& dMassGas,
            scalarField& dMassLiquid,
            scalarField& dMassSolid,
            scalarField& dMassSRCarrier
        ) const = 0;

        //- Add to devolatilisation mass
        void addToSurfaceReactionMass(const scalar dMass);


        // I-O

            //- Write injection info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeSurfaceReactionModel(CloudType)                                    \
                                                                               \
    typedef CloudType::reactingMultiphaseCloudType                             \
        reactingMultiphaseCloudType;                                           \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        SurfaceReactionModel<reactingMultiphaseCloudType>,                     \
        0                                                                      \
    );                                                                         \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        SurfaceReactionModel<reactingMultiphaseCloudType>,                     \
        dictionary                                                             \
    );


#define makeSurfaceReactionModelType(SS, CloudType)                            \
                                                                               \
    typedef CloudType::reactingMultiphaseCloudType                             \
        reactingMultiphaseCloudType;                                           \
    defineNamedTemplateTypeNameAndDebug                                        \
        (SS<reactingMultiphaseCloudType>, 0);                                  \
                                                                               \
    SurfaceReactionModel<reactingMultiphaseCloudType>::                        \
        adddictionaryConstructorToTable                                        \
        <SS<reactingMultiphaseCloudType> >                                     \
        add##SS##CloudType##reactingMultiphaseCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::SurfaceReactionModel<CloudType>::SurfaceReactionModel
(
    CloudType& owner
)
:
    CloudSubModelBase<CloudType>(owner),
    dMass_(0.0)
{}


template<class CloudType>
CML::SurfaceReactionModel<CloudType>::SurfaceReactionModel
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
CML::SurfaceReactionModel<CloudType>::SurfaceReactionModel
(
    const SurfaceReactionModel<CloudType>& srm
)
:
    CloudSubModelBase<CloudType>(srm),
    dMass_(srm.dMass_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::SurfaceReactionModel<CloudType>::~SurfaceReactionModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::SurfaceReactionModel<CloudType>::addToSurfaceReactionMass
(
    const scalar dMass
)
{
    dMass_ += dMass;
}


template<class CloudType>
void CML::SurfaceReactionModel<CloudType>::info(Ostream& os)
{
    const scalar mass0 = this->template getBaseProperty<scalar>("mass");
    const scalar massTotal = mass0 + returnReduce(dMass_, sumOp<scalar>());

    Info<< "    Mass transfer surface reaction  = " << massTotal << nl;

    if (this->writeTime())
    {
        this->setBaseProperty("mass", massTotal);
        dMass_ = 0.0;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::SurfaceReactionModel<CloudType> >
CML::SurfaceReactionModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    const word modelType(dict.lookup("surfaceReactionModel"));

    Info<< "Selecting surface reaction model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown surface reaction model type "
            << modelType << nl << nl
            << "Valid surface reaction model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<SurfaceReactionModel<CloudType> >(cstrIter()(dict, owner));
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
