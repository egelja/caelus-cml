/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::CloudFunctionObjectList

Description
    List of cloud function objects


\*---------------------------------------------------------------------------*/

#ifndef CloudFunctionObjectList_H
#define CloudFunctionObjectList_H

#include "PtrList.hpp"
#include "CloudFunctionObject.hpp"
#include "entry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class CloudFunctionObjectList Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CloudFunctionObjectList
:
    public PtrList<CloudFunctionObject<CloudType> >
{
protected:

    // Protected Data

        //- Reference to the owner cloud
        const CloudType& owner_;

        //- Dictionary
        const dictionary dict_;


public:

    // Constructors

        //- Null constructor
        CloudFunctionObjectList(CloudType& owner);

        //- Construct from mesh
        CloudFunctionObjectList
        (
            CloudType& owner,
            const dictionary& dict,
            const bool readFields
        );

        //- Construct copy
        CloudFunctionObjectList(const CloudFunctionObjectList& ppml);


    //- Destructor
    virtual ~CloudFunctionObjectList();


    // Member Functions

        // Access

            //- Return const access to the cloud owner
            inline const CloudType& owner() const;

            //- Return references to the cloud owner
            inline CloudType& owner();

            //- Return the forces dictionary
            inline const dictionary& dict() const;


        // Evaluation

            //- Pre-evolve hook
            virtual void preEvolve();

            //- Post-evolve hook
            virtual void postEvolve();

            //- Post-move hook
            virtual void postMove
            (
                typename CloudType::parcelType& p,
                const scalar dt,
                const point& position0,
                bool& keepParticle
            );

            //- Post-patch hook
            virtual void postPatch
            (
                const typename CloudType::parcelType& p,
                const polyPatch& pp,
                bool& keepParticle
            );

            //- Post-face hook
            virtual void postFace
            (
                const typename CloudType::parcelType& p,
                bool& keepParticle
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
inline const CloudType& CML::CloudFunctionObjectList<CloudType>::owner() const
{
    return owner_;
}


template<class CloudType>
inline CloudType& CML::CloudFunctionObjectList<CloudType>::owner()
{
    return owner_;
}


template<class CloudType>
inline const CML::dictionary&
CML::CloudFunctionObjectList<CloudType>::dict() const
{
    return dict_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::CloudFunctionObjectList<CloudType>::CloudFunctionObjectList
(
    CloudType& owner
)
:
    PtrList<CloudFunctionObject<CloudType> >(),
    owner_(owner),
    dict_(dictionary::null)
{}


template<class CloudType>
CML::CloudFunctionObjectList<CloudType>::CloudFunctionObjectList
(
    CloudType& owner,
    const dictionary& dict,
    const bool readFields
)
:
    PtrList<CloudFunctionObject<CloudType> >(),
    owner_(owner),
    dict_(dict)
{
    if (readFields)
    {
        wordList modelNames(dict.toc());

        Info<< "Constructing cloud functions" << endl;

        if (modelNames.size() > 0)
        {
            this->setSize(modelNames.size());

            forAll(modelNames, i)
            {
                const word& modelName = modelNames[i];

                const dictionary& modelDict(dict.subDict(modelName));

                // read the type of the function object
                const word objectType(modelDict.lookup("type"));

                this->set
                (
                    i,
                    CloudFunctionObject<CloudType>::New
                    (
                        modelDict,
                        owner,
                        objectType,
                        modelName
                    )
                );
            }
        }
        else
        {
            Info<< "    none" << endl;
        }
    }
}


template<class CloudType>
CML::CloudFunctionObjectList<CloudType>::CloudFunctionObjectList
(
    const CloudFunctionObjectList& cfol
)
:
    PtrList<CloudFunctionObject<CloudType> >(cfol),
    owner_(cfol.owner_),
    dict_(cfol.dict_)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::CloudFunctionObjectList<CloudType>::~CloudFunctionObjectList()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::CloudFunctionObjectList<CloudType>::preEvolve()
{
    forAll(*this, i)
    {
        this->operator[](i).preEvolve();
    }
}


template<class CloudType>
void CML::CloudFunctionObjectList<CloudType>::postEvolve()
{
    forAll(*this, i)
    {
        this->operator[](i).postEvolve();
    }
}


template<class CloudType>
void CML::CloudFunctionObjectList<CloudType>::postMove
(
    typename CloudType::parcelType& p,
    const scalar dt,
    const point& position0,
    bool& keepParticle
)
{
    forAll(*this, i)
    {
        if (!keepParticle)
        {
            return;
        }

        this->operator[](i).postMove(p, dt, position0, keepParticle);
    }
}


template<class CloudType>
void CML::CloudFunctionObjectList<CloudType>::postPatch
(
    const typename CloudType::parcelType& p,
    const polyPatch& pp,
    bool& keepParticle
)
{
    forAll(*this, i)
    {
        if (!keepParticle)
        {
            return;
        }

        this->operator[](i).postPatch(p, pp, keepParticle);
    }
}


template<class CloudType>
void CML::CloudFunctionObjectList<CloudType>::postFace
(
    const typename CloudType::parcelType& p,
    bool& keepParticle
)
{
    forAll(*this, i)
    {
        if (!keepParticle)
        {
            return;
        }

        this->operator[](i).postFace(p, keepParticle);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
