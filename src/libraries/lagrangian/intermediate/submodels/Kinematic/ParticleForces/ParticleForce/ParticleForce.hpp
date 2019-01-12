/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::ParticleForce

Description
    Abstract base class for particle forces


\*---------------------------------------------------------------------------*/

#ifndef ParticleForce_H
#define ParticleForce_H

#include "dictionary.hpp"
#include "forceSuSp.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class ParticleForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ParticleForce
{
    // Private data

        //- Reference to the owner cloud
        CloudType& owner_;

        //- Reference to the mesh database
        const fvMesh& mesh_;

        //- Force coefficients dictaionary
        const dictionary coeffs_;


public:

    //- Runtime type information
    TypeName("particleForce");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        ParticleForce,
        dictionary,
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        ),
        (owner, mesh, dict)
    );


    //- Convenience typedef for return type
    typedef VectorSpace<Vector<vector>, vector, 2> returnType;


    // Constructors

        //- Construct from mesh
        ParticleForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& forceType,
            const bool readCoeffs
        );

        //- Construct copy
        ParticleForce(const ParticleForce& pf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new ParticleForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ParticleForce();


    //- Selector
    static autoPtr<ParticleForce<CloudType> > New
    (
        CloudType& owner,
        const fvMesh& mesh,
        const dictionary& dict,
        const word& forceType
    );


    // Member Functions

        // Access

            //- Return const access to the cloud owner
            inline const CloudType& owner() const;

            //- Return reference to the cloud owner
            inline CloudType& owner();

            //- Return the mesh database
            inline const fvMesh& mesh() const;

            //- Return the force coefficients dictionary
            inline const dictionary& coeffs() const;


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

            //- Return the added mass
            virtual scalar massAdd
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
inline const CloudType& CML::ParticleForce<CloudType>::owner() const
{
    return owner_;
}


template<class CloudType>
inline CloudType& CML::ParticleForce<CloudType>::owner()
{
    return owner_;
}


template<class CloudType>
inline const CML::fvMesh& CML::ParticleForce<CloudType>::mesh() const
{
    return mesh_;
}


template<class CloudType>
inline const CML::dictionary& CML::ParticleForce<CloudType>::coeffs() const
{
    return coeffs_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleForce<CloudType>::ParticleForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& forceType,
    const bool readCoeffs
)
:
    owner_(owner),
    mesh_(mesh),
    coeffs_
    (
        readCoeffs
      ? dict.optionalSubDict(forceType + "Coeffs")
      : dictionary::null
    )
{
    if (readCoeffs && coeffs_.isNull())
    {
        FatalIOErrorInFunction
        (
            dict
        )   << "Force " << forceType << " must be specified as a dictionary"
            << exit(FatalIOError);
    }
}


template<class CloudType>
CML::ParticleForce<CloudType>::ParticleForce(const ParticleForce& pf)
:
    owner_(pf.owner_),
    mesh_(pf.mesh_),
    coeffs_(pf.coeffs_)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleForce<CloudType>::~ParticleForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ParticleForce<CloudType>::cacheFields(const bool store)
{}


template<class CloudType>
CML::forceSuSp CML::ParticleForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType&,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value;
    value.Su() = Zero;
    value.Sp() = 0.0;

    return value;
}


template<class CloudType>
CML::forceSuSp CML::ParticleForce<CloudType>::calcNonCoupled
(
    const typename CloudType::parcelType&,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value;
    value.Su() = Zero;
    value.Sp() = 0.0;

    return value;
}


template<class CloudType>
CML::scalar CML::ParticleForce<CloudType>::massAdd
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar mass
) const
{
    return 0.0;
}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::ParticleForce<CloudType> >
CML::ParticleForce<CloudType>::New
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& name
)
{
    word forceType = name;
    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(forceType);

    if (cstrIter == dictionaryConstructorTablePtr_->end() && dict.found("type"))
    {
        forceType = dict.lookupType<word>("type");
        cstrIter = dictionaryConstructorTablePtr_->find(forceType);
    }

    Info<< "    Selecting particle force " << forceType << endl;

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown particle force type "
            << forceType
            << ", constructor not in hash table" << nl << nl
            << "    Valid particle force types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<ParticleForce<CloudType> >
    (
        cstrIter()
        (
            owner,
            mesh,
            dict
        )
    );

}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeParticleForceModel(CloudType)                                      \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug                                        \
        (ParticleForce<kinematicCloudType>, 0);                                \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        ParticleForce<kinematicCloudType>,                                     \
        dictionary                                                             \
    );


#define makeParticleForceModelType(SS, CloudType)                              \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug(SS<kinematicCloudType>, 0);            \
                                                                               \
    ParticleForce<kinematicCloudType>::                                        \
        adddictionaryConstructorToTable<SS<kinematicCloudType> >               \
        add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //
