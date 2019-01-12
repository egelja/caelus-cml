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
    CML::regionModel

Description
    Base class for region models

SourceFiles
    regionModelI.H
    regionModel.C

\*---------------------------------------------------------------------------*/

#ifndef regionModel_H
#define regionModel_H

#include "IOdictionary.hpp"
#include "Switch.hpp"
#include "labelList.hpp"
#include "volFields.hpp"
#include "mappedPatchBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace regionModels
{

/*---------------------------------------------------------------------------*\
                         Class regionModel Declaration
\*---------------------------------------------------------------------------*/

class regionModel
:
    public IOdictionary
{

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        regionModel(const regionModel&);

        //- Disallow default bitwise assignment
        void operator=(const regionModel&);

        //- Construct region mesh and fields
        void constructMeshObjects();

        //- Initialise the region
        void initialise();


protected:

    // Protected data

        //- Reference to the primary mesh database
        const fvMesh& primaryMesh_;

        //- Reference to the time database
        const Time& time_;

        //- Active flag
        Switch active_;

        //- Active information output
        Switch infoOutput_;

        //- Model name
        const word modelName_;

        //- Pointer to the region mesh database
        autoPtr<fvMesh> regionMeshPtr_;

        //- Model coefficients dictionary
        dictionary coeffs_;

        //- Dictionary of output properties
        autoPtr<IOdictionary> outputPropertiesPtr_;


        // Addressing

            //- List of patch IDs on the primary region coupled to this region
            labelList primaryPatchIDs_;

            //- List of patch IDs internally coupled with the primary region
            labelList intCoupledPatchIDs_;


        //- Region name
        word regionName_;


        // Inter-region AMI interpolation caching

            //- List of region names this region is coupled to
            mutable wordList interRegionAMINames_;

            //- List of AMI objects per coupled region
            mutable PtrList<PtrList<AMIPatchToPatchInterpolation> >
                interRegionAMI_;


    // Protected member functions

        //- Read control parameters from dictionary
        virtual bool read();

        //- Read control parameters from dictionary
        virtual bool read(const dictionary& dict);

        //- Create or return a new inter-region AMI object
        virtual const AMIPatchToPatchInterpolation& interRegionAMI
        (
            const regionModel& nbrRegion,
            const label regionPatchi,
            const label nbrPatchi,
            const bool flip
        ) const;


public:

    //- Runtime type information
    TypeName("regionModel");


    // Constructors

        //- Construct null
        regionModel(const fvMesh& mesh, const word& regionType);

        //- Construct from mesh, region type and name
        regionModel
        (
            const fvMesh& mesh,
            const word& regionType,
            const word& modelName,
            bool readFields = true
        );

        //- Construct from mesh and name and dict
        regionModel
        (
            const fvMesh& mesh,
            const word& regionType,
            const word& modelName,
            const dictionary& dict,
            bool readFields = true
        );


    //- Destructor
    virtual ~regionModel();


    // Member Functions

        // Access

            //- Return the reference to the primary mesh database
            inline const fvMesh& primaryMesh() const;

            //- Return the reference to the time database
            inline const Time& time() const;

            //- Return the active flag
            inline const Switch& active() const;

            //- Return the information flag
            inline const Switch& infoOutput() const;

            //- Return the model name
            inline const word& modelName() const;

            //- Return the region mesh database
            inline const fvMesh& regionMesh() const;

            //- Return the region mesh database for manipulation
            inline fvMesh& regionMesh();

            //- Return the model coefficients dictionary
            inline const dictionary& coeffs() const;

            //- Return the solution dictionary
            inline const dictionary& solution() const;

            //- Return const access to the output properties dictionary
            inline const IOdictionary& outputProperties() const;

            //- Return output properties dictionary
            inline IOdictionary& outputProperties();


            // Addressing

                //- Return true if patchi on the local region is a coupled
                //  patch to the primary region
                inline bool isCoupledPatch(const label regionPatchi) const;

                //- Return true if patchi on the primary region is a coupled
                //  patch to the local region
                inline bool isRegionPatch(const label primaryPatchi) const;

                //- Return the list of patch IDs on the primary region coupled
                //  to this region
                inline const labelList& primaryPatchIDs() const;

                //- Return the list of patch IDs internally coupled with the
                //  primary region
                inline const labelList& intCoupledPatchIDs() const;

                //- Return region ID corresponding to primaryPatchID
                inline label regionPatchID(const label primaryPatchID) const;


        // Helper

            //- Return the coupled patch ID paired with coupled patch
            //  regionPatchi
            label nbrCoupledPatchID
            (
                const regionModel& nbrRegion,
                const label regionPatchi
            ) const;

            //- Map patch field from another region model to local patch
            template<class Type>
            tmp<CML::Field<Type> > mapRegionPatchField
            (
                const regionModel& nbrRegion,
                const label regionPatchi,
                const label nbrPatchi,
                const Field<Type>& nbrField,
                const bool flip = false
            ) const;

            //- Map patch field from another region model to local patch
            template<class Type>
            tmp<Field<Type> > mapRegionPatchField
            (
                const regionModel& nbrRegion,
                const word& fieldName,
                const label regionPatchi,
                const bool flip = false
            ) const;

            //- Map patch internal field from another region model to local
            //  patch
            template<class Type>
            tmp<Field<Type> > mapRegionPatchInternalField
            (
                const regionModel& nbrRegion,
                const word& fieldName,
                const label regionPatchi,
                const bool flip = false
            ) const;

            //- Convert a local region field to the primary region
            template<class Type>
            void toPrimary
            (
                const label regionPatchi,
                List<Type>& regionField
            ) const;

            //- Convert a primary region field to the local region
            template<class Type>
            void toRegion
            (
                const label regionPatchi,
                List<Type>& primaryFieldField
            ) const;

            //- Convert a local region field to the primary region with op
            template<class Type, class CombineOp>
            void toPrimary
            (
                const label regionPatchi,
                List<Type>& regionField,
                const CombineOp& cop
            ) const;

            //- Convert a primary region field to the local region with op
            template<class Type, class CombineOp>
            void toRegion
            (
                const label regionPatchi,
                List<Type>& primaryFieldField,
                const CombineOp& cop
            ) const;


        // Evolution

            //- Main driver routing to evolve the region - calls other evolves
            virtual void evolve();

            //- Pre-evolve region
            virtual void preEvolveRegion();

            //- Evolve the region
            virtual void evolveRegion();

            //- Post-evolve region
            virtual void postEvolveRegion();


        // I-O

            //- Provide some feedback
            virtual void info();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace regionModels
} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
inline const CML::fvMesh&
CML::regionModels::regionModel::primaryMesh() const
{
    return primaryMesh_;
}


inline const CML::Time& CML::regionModels::regionModel::time() const
{
    return time_;
}


inline const CML::Switch& CML::regionModels::regionModel::active() const
{
    return active_;
}


inline const CML::Switch& CML::regionModels::regionModel::infoOutput() const
{
    return infoOutput_;
}


inline const CML::word& CML::regionModels::regionModel::modelName() const
{
    return modelName_;
}


inline const CML::fvMesh& CML::regionModels::regionModel::regionMesh() const
{
    if (time_.foundObject<fvMesh>(regionName_))
    {
        return time_.lookupObject<fvMesh>(regionName_);
    }
    else if (!regionMeshPtr_.valid())
    {
        FatalErrorInFunction
         << "Region mesh not available" << abort(FatalError);
    }

    return regionMeshPtr_();
}


inline CML::fvMesh& CML::regionModels::regionModel::regionMesh()
{
    if (time_.foundObject<fvMesh>(regionName_))
    {
        return const_cast<fvMesh&>
        (
            time_.lookupObject<fvMesh>(regionName_)
        );
    }
    else if (!regionMeshPtr_.valid())
    {
        FatalErrorInFunction
           << "Region mesh not available" << abort(FatalError);
    }

    return regionMeshPtr_();
}


inline const CML::dictionary& CML::regionModels::regionModel::coeffs() const
{
    return coeffs_;
}


inline const CML::dictionary&
CML::regionModels::regionModel::solution() const
{
    return regionMesh().solutionDict();
}


inline const CML::IOdictionary&
CML::regionModels::regionModel::outputProperties() const
{
    if (!outputPropertiesPtr_.valid())
    {
        FatalErrorInFunction
            << "outputProperties dictionary not available"
            << abort(FatalError);
    }

    return outputPropertiesPtr_();
}


inline CML::IOdictionary&
CML::regionModels::regionModel::outputProperties()
{
    if (!outputPropertiesPtr_.valid())
    {
        FatalErrorInFunction
            << "outputProperties dictionary not available"
            << abort(FatalError);
    }

    return outputPropertiesPtr_();
}


inline bool CML::regionModels::regionModel::isCoupledPatch
(
    const label regionPatchi
) const
{
    forAll(intCoupledPatchIDs_, i)
    {
        if (intCoupledPatchIDs_[i] == regionPatchi)
        {
            return true;
        }
    }

    return false;
}


inline bool CML::regionModels::regionModel::isRegionPatch
(
    const label primaryPatchi
) const
{
    forAll(primaryPatchIDs_, i)
    {
        if (primaryPatchIDs_[i] == primaryPatchi)
        {
            return true;
        }
    }

    return false;
}


inline const CML::labelList&
CML::regionModels::regionModel::primaryPatchIDs() const
{
    return primaryPatchIDs_;
}


inline const CML::labelList&
CML::regionModels::regionModel::intCoupledPatchIDs() const
{
    return intCoupledPatchIDs_;
}


inline CML::label CML::regionModels::regionModel::regionPatchID
(
    const label primaryPatchID
) const
{
    forAll(primaryPatchIDs_, i)
    {
        if (primaryPatchIDs_[i] == primaryPatchID)
        {
            return intCoupledPatchIDs_[i];
        }
    }

    return -1;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> >
CML::regionModels::regionModel::mapRegionPatchField
(
    const regionModel& nbrRegion,
    const label regionPatchi,
    const label nbrPatchi,
    const Field<Type>& nbrField,
    const bool flip
) const
{
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag + 1;

    const AMIPatchToPatchInterpolation& ami =
        interRegionAMI(nbrRegion, regionPatchi, nbrPatchi, flip);

    tmp<Field<Type> > tresult(ami.interpolateToSource(nbrField));

    UPstream::msgType() = oldTag;

    return tresult;
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::regionModels::regionModel::mapRegionPatchField
(
    const regionModel& nbrRegion,
    const word& fieldName,
    const label regionPatchi,
    const bool flip
) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    const fvMesh& nbrRegionMesh = nbrRegion.regionMesh();

    if (nbrRegionMesh.foundObject<fieldType>(fieldName))
    {
        const label nbrPatchi = nbrCoupledPatchID(nbrRegion, regionPatchi);

        int oldTag = UPstream::msgType();
        UPstream::msgType() = oldTag + 1;

        const AMIPatchToPatchInterpolation& ami =
            interRegionAMI(nbrRegion, regionPatchi, nbrPatchi, flip);

        const fieldType& nbrField =
            nbrRegionMesh.lookupObject<fieldType>(fieldName);

        const Field<Type>& nbrFieldp = nbrField.boundaryField()[nbrPatchi];

        tmp<Field<Type> > tresult(ami.interpolateToSource(nbrFieldp));

        UPstream::msgType() = oldTag;

        return tresult;
    }
    else
    {
        const polyPatch& p = regionMesh().boundaryMesh()[regionPatchi];

        return
            tmp<Field<Type> >
            (
                new Field<Type>
                (
                    p.size(),
                    Zero
                )
            );
    }
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::regionModels::regionModel::mapRegionPatchInternalField
(
    const regionModel& nbrRegion,
    const word& fieldName,
    const label regionPatchi,
    const bool flip
) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    const fvMesh& nbrRegionMesh = nbrRegion.regionMesh();

    if (nbrRegionMesh.foundObject<fieldType>(fieldName))
    {
        const label nbrPatchi = nbrCoupledPatchID(nbrRegion, regionPatchi);

        int oldTag = UPstream::msgType();
        UPstream::msgType() = oldTag + 1;

        const AMIPatchToPatchInterpolation& ami =
            interRegionAMI(nbrRegion, regionPatchi, nbrPatchi, flip);

        const fieldType& nbrField =
            nbrRegionMesh.lookupObject<fieldType>(fieldName);

        const fvPatchField<Type>& nbrFieldp =
            nbrField.boundaryField()[nbrPatchi];

        tmp<Field<Type> > tresult
        (
            ami.interpolateToSource(nbrFieldp.patchInternalField())
        );

        UPstream::msgType() = oldTag;

        return tresult;
    }
    else
    {
        const polyPatch& p = regionMesh().boundaryMesh()[regionPatchi];

        return
            tmp<Field<Type> >
            (
                new Field<Type>
                (
                    p.size(),
                    Zero
                )
            );
    }
}


template<class Type>
void CML::regionModels::regionModel::toPrimary
(
    const label regionPatchi,
    List<Type>& regionField
) const
{
    forAll(intCoupledPatchIDs_, i)
    {
        if (intCoupledPatchIDs_[i] == regionPatchi)
        {
            const mappedPatchBase& mpb =
                refCast<const mappedPatchBase>
                (
                    regionMesh().boundaryMesh()[regionPatchi]
                );
            mpb.reverseDistribute(regionField);
            return;
        }
    }

    FatalErrorInFunction
        << "Region patch ID " << regionPatchi << " not found in region mesh"
        << abort(FatalError);
}


template<class Type>
void CML::regionModels::regionModel::toRegion
(
    const label regionPatchi,
    List<Type>& primaryField
) const
{
    forAll(intCoupledPatchIDs_, i)
    {
        if (intCoupledPatchIDs_[i] == regionPatchi)
        {
            const mappedPatchBase& mpb =
                refCast<const mappedPatchBase>
                (
                    regionMesh().boundaryMesh()[regionPatchi]
                );
            mpb.distribute(primaryField);
            return;
        }
    }

    FatalErrorInFunction
        << "Region patch ID " << regionPatchi << " not found in region mesh"
        << abort(FatalError);
}


template<class Type, class CombineOp>
void CML::regionModels::regionModel::toPrimary
(
    const label regionPatchi,
    List<Type>& regionField,
    const CombineOp& cop
) const
{
    forAll(intCoupledPatchIDs_, i)
    {
        if (intCoupledPatchIDs_[i] == regionPatchi)
        {
            const mappedPatchBase& mpb =
                refCast<const mappedPatchBase>
                (
                    regionMesh().boundaryMesh()[regionPatchi]
                );
            mpb.reverseDistribute(regionField, cop);
            return;
        }
    }

    FatalErrorInFunction
        << "Region patch ID " << regionPatchi << " not found in region mesh"
        << abort(FatalError);
}


template<class Type, class CombineOp>
void CML::regionModels::regionModel::toRegion
(
    const label regionPatchi,
    List<Type>& primaryField,
    const CombineOp& cop
) const
{
    forAll(intCoupledPatchIDs_, i)
    {
        if (intCoupledPatchIDs_[i] == regionPatchi)
        {
            const mappedPatchBase& mpb =
                refCast<const mappedPatchBase>
                (
                    regionMesh().boundaryMesh()[regionPatchi]
                );
            mpb.distribute(primaryField, cop);
            return;
        }
    }

    FatalErrorInFunction
        << "Region patch ID " << regionPatchi << " not found in region mesh"
        << abort(FatalError);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
