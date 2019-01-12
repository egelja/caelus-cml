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
    CML::BreakupModel

Description
    Templated break-up model class


\*---------------------------------------------------------------------------*/

#ifndef BreakupModel_H
#define BreakupModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class BreakupModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class BreakupModel
:
    public CloudSubModelBase<CloudType>
{

protected:

    // Protected data

        Switch solveOscillationEq_;

        scalar y0_;
        scalar yDot0_;

        scalar TABComega_;
        scalar TABCmu_;
        scalar TABtwoWeCrit_;


public:

    //- Runtime type information
    TypeName("breakupModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        BreakupModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        BreakupModel(CloudType& owner);

        //- Construct from dictionary
        BreakupModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type,
            bool solveOscillationEq = false
        );

        //- Construct copy
        BreakupModel(const BreakupModel<CloudType>& bum);

        //- Construct and return a clone
        virtual autoPtr<BreakupModel<CloudType>> clone() const = 0;


    //- Destructor
    virtual ~BreakupModel();


    //- Selector
    static autoPtr<BreakupModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Access

        inline const Switch& solveOscillationEq() const
        {
            return solveOscillationEq_;
        }

        inline const scalar& y0() const
        {
            return y0_;
        }

        inline const scalar& yDot0() const
        {
            return yDot0_;
        }

        inline const scalar& TABComega() const
        {
            return TABComega_;
        }

        inline const scalar& TABCmu() const
        {
            return TABCmu_;
        }

        inline const scalar& TABtwoWeCrit() const
        {
            return TABtwoWeCrit_;
        }


    // Member Functions

        //- Update the parcel properties and return true if a child parcel
        //  should be added
        virtual bool update
        (
            const scalar dt,
            const vector& g,
            scalar& d,
            scalar& tc,
            scalar& ms,
            scalar& nParticle,
            scalar& KHindex,
            scalar& y,
            scalar& yDot,
            const scalar d0,
            const scalar rho,
            const scalar mu,
            const scalar sigma,
            const vector& U,
            const scalar rhoc,
            const scalar muc,
            const vector& Urel,
            const scalar Urmag,
            const scalar tMom,
            scalar& dChild,
            scalar& massChild
        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeBreakupModel(CloudType)                                            \
                                                                               \
    typedef CloudType::sprayCloudType sprayCloudType;                          \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        BreakupModel<sprayCloudType>,                                          \
        0                                                                      \
    );                                                                         \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        BreakupModel<sprayCloudType>,                                          \
        dictionary                                                             \
    );


#define makeBreakupModelType(SS, CloudType)                                    \
                                                                               \
    typedef CloudType::sprayCloudType sprayCloudType;                          \
    defineNamedTemplateTypeNameAndDebug(SS<sprayCloudType>, 0);                \
                                                                               \
    BreakupModel<sprayCloudType>::                                             \
        adddictionaryConstructorToTable<SS<sprayCloudType> >                   \
            add##SS##CloudType##sprayCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::BreakupModel<CloudType>::BreakupModel
(
    CloudType& owner
)
:
    CloudSubModelBase<CloudType>(owner),
    solveOscillationEq_(false),
    y0_(0.0),
    yDot0_(0.0),
    TABComega_(0.0),
    TABCmu_(0.0),
    TABtwoWeCrit_(0.0)
{}


template<class CloudType>
CML::BreakupModel<CloudType>::BreakupModel
(
    const BreakupModel<CloudType>& bum
)
:
    CloudSubModelBase<CloudType>(bum),
    solveOscillationEq_(bum.solveOscillationEq_),
    y0_(bum.y0_),
    yDot0_(bum.yDot0_),
    TABComega_(bum.TABComega_),
    TABCmu_(bum.TABCmu_),
    TABtwoWeCrit_(bum.TABtwoWeCrit_)
{}


template<class CloudType>
CML::BreakupModel<CloudType>::BreakupModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type,
    bool solveOscillationEq
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type),
    solveOscillationEq_(solveOscillationEq),
    y0_(this->coeffDict().template lookupOrDefault<scalar>("y0", 0.0)),
    yDot0_(this->coeffDict().template lookupOrDefault<scalar>("yDot0", 0.0)),
    TABComega_(8),
    TABCmu_(5),
    TABtwoWeCrit_(12)
{
    if (solveOscillationEq_ && dict.found("TABCoeffs"))
    {
        const dictionary coeffs(dict.subDict("TABCoeffs"));
        coeffs.lookup("Comega") >> TABComega_;
        coeffs.lookup("Cmu") >> TABCmu_;
        scalar WeCrit(readScalar(coeffs.lookup("WeCrit")));
        TABtwoWeCrit_ = 2*WeCrit;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::BreakupModel<CloudType>::~BreakupModel()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::BreakupModel<CloudType> >
CML::BreakupModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    word BreakupModelType(dict.lookup("breakupModel"));

    Info<< "Selecting BreakupModel " << BreakupModelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(BreakupModelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown BreakupModelType type "
            << BreakupModelType
            << ", constructor not in hash table" << nl << nl
            << "    Valid BreakupModel types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<BreakupModel<CloudType> >(cstrIter()(dict, owner));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
