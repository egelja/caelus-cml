/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::SubModelBase

Description
    Base class for cloud sub-models


\*---------------------------------------------------------------------------*/

#ifndef SubModelBase_H
#define SubModelBase_H

#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
template<class CloudType>
class SubModelBase;

/*---------------------------------------------------------------------------*\
                       Class SubModelBase Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class SubModelBase
{
protected:

    // Protected Data

        //- Reference to the cloud
        CloudType& owner_;

        //- Reference to the cloud dictionary
        const dictionary& dict_;

        //- Name of the sub-model bas class
        const word baseName_;

        //- Name of the sub-model
        const word name_;

        //- Coefficients dictionary
        const dictionary& coeffDict_;


public:

    // Constructors

        //- Construct null from owner cloud
        SubModelBase(CloudType& owner);

        //- Construct from owner cloud, dictionary, and model type name
        SubModelBase
        (
            CloudType& owner,
            const dictionary& dict,
            const word& baseName,
            const word& name,
            const word& dictExt = "Coeffs"
        );

        //- Construct as copy
        SubModelBase(const SubModelBase<CloudType>& smb);


    //- Destructor
    virtual ~SubModelBase();

    //- Type of cloud this model was instantiated for
    typedef CloudType cloudType;


    // Member Functions

        // Access

            //- Return const access to the owner cloud
            const CloudType& owner() const;

            //- Return const access to the cloud dictionary
            const dictionary& dict() const;

            //- Return const access to the base name of the sub-model
            const word& baseName() const;

            //- Return const access to the name of the sub-model
            const word& name() const;

            //- Return const access to the coefficients dictionary
            const dictionary& coeffDict() const;

            //- Return const access to the properties dictionary
            const IOdictionary& properties() const;

            //- Returns true if defaultCoeffs is true and outputs on printMsg
            bool defaultCoeffs(const bool printMsg) const;

            //- Return the model 'active' status - default active = true
            virtual bool active() const;

            //- Cache dependant sub-model fields
            virtual void cacheFields(const bool store);


        // Edit

            //- Return non-const access to the owner cloud for manipulation
            CloudType& owner();

            //- Retrieve generic property from sub-model
            template<class Type>
            Type getModelProperty(const word& entryName) const;

            //- Retrieve generic property from sub-model
            template<class Type>
            void getModelProperty(const word& entryName, Type& value) const;

            //- Add generic property from sub-model
            template<class Type>
            void setModelProperty(const word& entryName, const Type& value);

            //- Retrieve generic property from base model
            template<class Type>
            Type getBaseProperty(const word& entryName) const;

            //- Add generic property from base model
            template<class Type>
            void setBaseProperty(const word& entryName, const Type& value);


        // I-O

            //- Write
            virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::SubModelBase<CloudType>::SubModelBase(CloudType& owner)
:
    owner_(owner),
    dict_(dictionary::null),
    baseName_("none"),
    name_("none"),
    coeffDict_(dictionary::null)
{}


template<class CloudType>
CML::SubModelBase<CloudType>::SubModelBase
(
    CloudType& owner,
    const dictionary& dict,
    const word& baseName,
    const word& name,
    const word& dictExt
)
:
    owner_(owner),
    dict_(dict),
    baseName_(baseName),
    name_(name),
    coeffDict_(dict.subDict(name + dictExt))
{}


template<class CloudType>
CML::SubModelBase<CloudType>::SubModelBase(const SubModelBase<CloudType>& smb)
:
    owner_(smb.owner_),
    dict_(smb.dict_),
    baseName_(smb.baseName_),
    name_(smb.name_),
    coeffDict_(smb.coeffDict_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::SubModelBase<CloudType>::~SubModelBase()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CloudType& CML::SubModelBase<CloudType>::owner() const
{
    return owner_;
}


template<class CloudType>
const CML::dictionary& CML::SubModelBase<CloudType>::dict() const
{
    return dict_;
}


template<class CloudType>
const CML::word& CML::SubModelBase<CloudType>::baseName() const
{
    return baseName_;
}


template<class CloudType>
const CML::word& CML::SubModelBase<CloudType>::name() const
{
    return name_;
}


template<class CloudType>
const CML::dictionary& CML::SubModelBase<CloudType>::coeffDict() const
{
    return coeffDict_;
}


template<class CloudType>
bool CML::SubModelBase<CloudType>::defaultCoeffs(const bool printMsg) const
{
    bool def = coeffDict_.lookupOrDefault<bool>("defaultCoeffs", false);
    if (printMsg && def)
    {
        Info<< incrIndent;
        Info<< indent << "Employing default coefficients" << endl;
        Info<< decrIndent;
    }

    return def;
}


template<class CloudType>
CloudType& CML::SubModelBase<CloudType>::owner()
{
    return owner_;
}


template<class CloudType>
bool CML::SubModelBase<CloudType>::active() const
{
    return true;
}


template<class CloudType>
void CML::SubModelBase<CloudType>::cacheFields(const bool)
{
    // do nothing
}


template<class CloudType>
template<class Type>
Type CML::SubModelBase<CloudType>::getBaseProperty
(
    const word& entryName
) const
{
    Type result = pTraits<Type>::zero;

    const dictionary& properties = this->owner().outputProperties();

    if (properties.found(baseName_))
    {
        const dictionary& baseDict = properties.subDict(baseName_);
        baseDict.readIfPresent(entryName, result);
    }

    return result;
}


template<class CloudType>
template<class Type>
void CML::SubModelBase<CloudType>::setBaseProperty
(
    const word& entryName,
    const Type& value
)
{
    dictionary& properties = this->owner().outputProperties();

    if (properties.found(baseName_))
    {
        dictionary& baseDict = properties.subDict(baseName_);
        baseDict.add(entryName, value, true);
    }
    else
    {
        properties.add(baseName_, dictionary());
        properties.subDict(baseName_).add(entryName, value);
    }
}


template<class CloudType>
template<class Type>
Type CML::SubModelBase<CloudType>::getModelProperty
(
    const word& entryName
) const
{
    Type result = pTraits<Type>::zero;

    const dictionary& properties = this->owner().outputProperties();

    if (properties.found(baseName_))
    {
        const dictionary& baseDict = properties.subDict(baseName_);

        if (baseDict.found(name_))
        {
            baseDict.subDict(name_).readIfPresent(entryName, result);
        }
    }

    return result;
}


template<class CloudType>
template<class Type>
void CML::SubModelBase<CloudType>::getModelProperty
(
    const word& entryName,
    Type& value
) const
{
    const dictionary& properties = this->owner().outputProperties();

    if (properties.found(baseName_))
    {
        const dictionary& baseDict = properties.subDict(baseName_);

        if (baseDict.found(name_))
        {
            baseDict.subDict(name_).readIfPresent(entryName, value);
        }
    }
}


template<class CloudType>
template<class Type>
void CML::SubModelBase<CloudType>::setModelProperty
(
    const word& entryName,
    const Type& value
)
{
    dictionary& properties = this->owner().outputProperties();

    if (properties.found(baseName_))
    {
        dictionary& baseDict = properties.subDict(baseName_);
        if (baseDict.found(name_))
        {
            baseDict.subDict(name_).add(entryName, value, true);
        }
        else
        {
            baseDict.add(name_, dictionary());
            baseDict.subDict(name_).add(entryName, value, true);
        }
    }
    else
    {
        properties.add(baseName_, dictionary());
        properties.subDict(baseName_).add(name_, dictionary());
        properties.subDict(baseName_).subDict(name_).add(entryName, value);
    }
}


template<class CloudType>
void CML::SubModelBase<CloudType>::write(Ostream& os) const
{
    os.writeKeyword("owner") << owner_.name() << token::END_STATEMENT << nl;

    // not writing complete cloud dictionary, only coeffs
//    os  << dict_;
    os  << coeffDict_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
