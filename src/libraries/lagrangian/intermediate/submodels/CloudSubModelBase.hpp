/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2019 OpenFOAM Foundation
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
    CML::CloudSubModelBase

Description
    Base class for cloud sub-models


\*---------------------------------------------------------------------------*/

#ifndef CloudSubModelBase_H
#define CloudSubModelBase_H

#include "subModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class CloudSubModelBase Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CloudSubModelBase
:
    public subModelBase
{
protected:

    // Protected Data

        //- Reference to the cloud
        CloudType& owner_;


public:

    // Constructors

        //- Construct null from owner cloud
        CloudSubModelBase(CloudType& owner);

        //- Construct from owner cloud without name
        CloudSubModelBase
        (
            CloudType& owner,
            const dictionary& dict,
            const word& baseName,
            const word& modelType,
            const word& dictExt = "Coeffs"
        );

        //- Construct from owner cloud with name
        CloudSubModelBase
        (
            const word& modelName,
            CloudType& owner,
            const dictionary& dict,
            const word& baseName,
            const word& modelType
        );

        //- Construct as copy
        CloudSubModelBase(const CloudSubModelBase<CloudType>& smb);


    //- Destructor
    virtual ~CloudSubModelBase();

    //- Type of cloud this model was instantiated for
    typedef CloudType cloudType;


    // Member Functions

        // Access

            //- Return const access to the owner cloud
            const CloudType& owner() const;

            //- Flag to indicate when to write a property
            virtual bool writeTime() const;


        // Edit

            //- Return non-const access to the owner cloud for manipulation
            CloudType& owner();


        // I-O

            //- Write
            virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::CloudSubModelBase<CloudType>::CloudSubModelBase(CloudType& owner)
:
    subModelBase(owner.outputProperties()),
    owner_(owner)
{}


template<class CloudType>
CML::CloudSubModelBase<CloudType>::CloudSubModelBase
(
    CloudType& owner,
    const dictionary& dict,
    const word& baseName,
    const word& modelType,
    const word& dictExt
)
:
    subModelBase
    (
        owner.outputProperties(),
        dict,
        baseName,
        modelType,
        dictExt
    ),
    owner_(owner)
{}


template<class CloudType>
CML::CloudSubModelBase<CloudType>::CloudSubModelBase
(
    const word& modelName,
    CloudType& owner,
    const dictionary& dict,
    const word& baseName,
    const word& modelType
)
:
    subModelBase
    (
        modelName,
        owner.outputProperties(),
        dict,
        baseName,
        modelType
    ),
    owner_(owner)
{}


template<class CloudType>
CML::CloudSubModelBase<CloudType>::CloudSubModelBase
(
    const CloudSubModelBase<CloudType>& smb
)
:
    subModelBase(smb),
    owner_(smb.owner_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::CloudSubModelBase<CloudType>::~CloudSubModelBase()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CloudType& CML::CloudSubModelBase<CloudType>::owner() const
{
    return owner_;
}


template<class CloudType>
CloudType& CML::CloudSubModelBase<CloudType>::owner()
{
    return owner_;
}


template<class CloudType>
bool CML::CloudSubModelBase<CloudType>::writeTime() const
{
    return
        active()
     && owner_.solution().transient()
     && owner_.db().time().outputTime();
}


template<class CloudType>
void CML::CloudSubModelBase<CloudType>::write(Ostream& os) const
{
    os.writeKeyword("owner") << owner_.name() << token::END_STATEMENT
        << nl;

    subModelBase::write(os);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
