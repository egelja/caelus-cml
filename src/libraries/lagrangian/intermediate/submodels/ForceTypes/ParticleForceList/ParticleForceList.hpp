/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::ParticleForceList

Description
    List of particle forces


\*---------------------------------------------------------------------------*/

#ifndef ParticleForceList_H
#define ParticleForceList_H

#include "ParticleForce.hpp"
#include "forceSuSp.hpp"
#include "entry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class ParticleForceList Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ParticleForceList
:
    public PtrList<ParticleForce<CloudType> >
{
    // Private data

        //- Reference to the owner cloud
        CloudType& owner_;

        //- Reference to the mesh database
        const fvMesh& mesh_;

        //- Forces dictionary
        const dictionary dict_;

        //- Calculate coupled forces flag
        bool calcCoupled_;

        //- Calculate non-coupled forces flag
        bool calcNonCoupled_;


public:

    // Constructors

        //- Null constructor
        ParticleForceList(CloudType& owner, const fvMesh& mesh);

        //- Construct from mesh
        ParticleForceList
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const bool readFields
        );

        //- Construct copy
        ParticleForceList(const ParticleForceList& pfl);


    //- Destructor
    virtual ~ParticleForceList();


    // Member Functions

        // Access

            //- Return const access to the cloud owner
            inline const CloudType& owner() const;

            //- Return references to the cloud owner
            inline CloudType& owner();

            //- Return the mesh database
            inline const fvMesh& mesh() const;

            //- Return the forces dictionary
            inline const dictionary& dict() const;

            //- Set the calcCoupled flag
            inline void setCalcCoupled(bool flag);

            //- Set the calcNonCoupled flag
            inline void setCalcNonCoupled(bool flag);


        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);

            //- Calculate the coupled force
            virtual forceSuSp calcCoupled
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar dt,
                const scalar mass,
                const scalar Re,
                const scalar muc
            ) const;

            //- Calculate the non-coupled force
            virtual forceSuSp calcNonCoupled
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar dt,
                const scalar mass,
                const scalar Re,
                const scalar muc
            ) const;

            //- Return the effective mass
            virtual scalar massEff
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar mass
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
inline const CloudType& CML::ParticleForceList<CloudType>::owner() const
{
    return owner_;
}


template<class CloudType>
inline CloudType& CML::ParticleForceList<CloudType>::owner()
{
    return owner_;
}


template<class CloudType>
inline const CML::fvMesh& CML::ParticleForceList<CloudType>::mesh() const
{
    return mesh_;
}


template<class CloudType>
inline const CML::dictionary& CML::ParticleForceList<CloudType>::dict() const
{
    return dict_;
}


template<class CloudType>
inline void CML::ParticleForceList<CloudType>::setCalcCoupled(bool flag)
{
    calcCoupled_ = flag;
}


template<class CloudType>
inline void CML::ParticleForceList<CloudType>::setCalcNonCoupled(bool flag)
{
    calcNonCoupled_ = flag;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleForceList<CloudType>::ParticleForceList
(
    CloudType& owner,
    const fvMesh& mesh
)
:
    PtrList<ParticleForce<CloudType> >(),
    owner_(owner),
    mesh_(mesh),
    dict_(dictionary::null),
    calcCoupled_(true),
    calcNonCoupled_(true)
{}


template<class CloudType>
CML::ParticleForceList<CloudType>::ParticleForceList
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const bool readFields
)
:
    PtrList<ParticleForce<CloudType> >(),
    owner_(owner),
    mesh_(mesh),
    dict_(dict),
    calcCoupled_(true),
    calcNonCoupled_(true)
{
    if (readFields)
    {
        wordList modelNames(dict.toc());

        Info<< "Constructing particle forces" << endl;

        if (modelNames.size() > 0)
        {
            this->setSize(modelNames.size());

            label i = 0;
            forAllConstIter(IDLList<entry>, dict, iter)
            {
                const word& model = iter().keyword();
                if (iter().isDict())
                {
                    this->set
                    (
                        i++,
                        ParticleForce<CloudType>::New
                        (
                            owner,
                            mesh,
                            iter().dict(),
                            model
                        )
                    );
                }
                else
                {
                    this->set
                    (
                        i++,
                        ParticleForce<CloudType>::New
                        (
                            owner,
                            mesh,
                            dictionary::null,
                            model
                        )
                    );
                }
            }
        }
        else
        {
            Info<< "    none" << endl;
        }
    }
}


template<class CloudType>
CML::ParticleForceList<CloudType>::ParticleForceList
(
    const ParticleForceList& pf
)
:
    PtrList<ParticleForce<CloudType> >(pf),
    owner_(pf.owner_),
    mesh_(pf.mesh_),
    dict_(pf.dict_)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleForceList<CloudType>::~ParticleForceList()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ParticleForceList<CloudType>::cacheFields(const bool store)
{
    forAll(*this, i)
    {
        this->operator[](i).cacheFields(store);
    }
}


template<class CloudType>
CML::forceSuSp CML::ParticleForceList<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value(Zero, 0.0);

    if (calcCoupled_)
    {
        forAll(*this, i)
        {
            value += this->operator[](i).calcCoupled(p, td, dt, mass, Re, muc);
        }
    }

    return value;
}


template<class CloudType>
CML::forceSuSp CML::ParticleForceList<CloudType>::calcNonCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value(Zero, 0.0);

    if (calcNonCoupled_)
    {
        forAll(*this, i)
        {
            value +=
                this->operator[](i).calcNonCoupled(p, td, dt, mass, Re, muc);
        }
    }

    return value;
}


template<class CloudType>
CML::scalar CML::ParticleForceList<CloudType>::massEff
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar mass
) const
{
    scalar massEff = mass;
    forAll(*this, i)
    {
        massEff += this->operator[](i).massAdd(p, td, mass);
    }

    return massEff;
}

#endif

// ************************************************************************* //
