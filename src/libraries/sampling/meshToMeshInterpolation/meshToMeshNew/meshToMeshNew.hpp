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

\*---------------------------------------------------------------------------*/

#ifndef meshToMeshNew_HPP
#define meshToMeshNew_HPP

#include "polyMesh.hpp"
#include "boundBox.hpp"
#include "mapDistribute.hpp"
#include "volFieldsFwd.hpp"
#include "NamedEnum.hpp"

namespace CML
{

class meshToMeshNew
{
public:

    // Public data types

        //- Enumeration specifying required accuracy
        enum interpolationMethod
        {
            imMap,
            imCellVolumeWeight
        };

        static const NamedEnum<interpolationMethod, 2>
            interpolationMethodNames_;

private:

    // Private data

        //- Name of source mesh region
        const word srcRegionName_;

        //- Name of target mesh region
        const word tgtRegionName_;

        //- Source to target cell addressing
        labelListList srcToTgtCellAddr_;

        //- Target to source cell addressing
        labelListList tgtToSrcCellAddr_;

        //- Source to target cell interplation weights
        scalarListList srcToTgtCellWght_;

        //- Target to source cell interpolation weights
        scalarListList tgtToSrcCellWght_;

        //- Interpolation method
        interpolationMethod method_;

        //- Cell total volume in overlap region [m3]
        scalar V_;

        //- Index of processor that holds all of both sides. -1 in all other
        //  cases
        label singleMeshProc_;

        //- Source map pointer - parallel running only
        autoPtr<mapDistribute> srcMapPtr_;

        //- Target map pointer - parallel running only
        autoPtr<mapDistribute> tgtMapPtr_;

        //- Tolerance used in volume overlap calculations
        static scalar tolerance_;


    // Private Member Functions

        //- Helper function to add a constant offset to a list
        template<class Type>
        void add(UList<Type>& fld, const label offset) const;

        //- Write the connectivity (debugging)
        void writeConnectivity
        (
            const polyMesh& src,
            const polyMesh& tgt,
            const labelListList& srcToTargetAddr
        ) const;

        //- Return src cell IDs for the overlap region
        labelList maskCells(const polyMesh& src, const polyMesh& tgt) const;

        //- Find indices of overlapping cells in src and tgt meshes - returns
        //  true if found a matching pair
        bool findInitialSeeds
        (
            const polyMesh& src,
            const polyMesh& tgt,
            const labelList& srcCellIDs,
            const boolList& mapFlag,
            const label startSeedI,
            label& srcSeedI,
            label& tgtSeedI
        ) const;


        // Direct (one-to-one) mapping

            //- Append to list of src mesh seed indices
            void appendToDirectSeeds
            (
                const polyMesh& src,
                const polyMesh& tgt,
                boolList& mapFlag,
                labelList& srcTgtSeed,
                DynamicList<label>& srcSeeds,
                label& srcSeedI,
                label& tgtSeedI
            ) const;

            //- Main driver routine for direct mapping
            void calcDirect
            (
                const polyMesh& src,
                const polyMesh& tgt,
                const label srcSeedI,
                const label tgtSeedI
            );


        // Indirect (non-conformal) mapping

            //- Normalise the interpolation weights
            void normaliseWeights
            (
                const word& descriptor,
                const scalarField& cellVolumes,
                const labelListList& addr,
                scalarListList& wght
            ) const;

            //- Append target cell neihgbour cells to cellIDs list
            void appendNbrTgtCells
            (
                const label tgtCellI,
                const polyMesh& tgt,
                const DynamicList<label>& visitedTgtCells,
                DynamicList<label>& nbrTgtCellIDs
            ) const;

            //- Set the next cells in the advancing front algorithm
            void setNextCells
            (
                label& startSeedI,
                label& srcCellI,
                label& tgtCellI,
                const polyMesh& src,
                const polyMesh& tgt,
                const labelList& srcCellIDs,
                const boolList& mapFlag,
                const DynamicList<label>& visitedCells,
                labelList& seedCells
            ) const;

            //- Return the true if cells intersect
            bool intersect
            (
                const polyMesh& src,
                const polyMesh& tgt,
                const label srcCellI,
                const label tgtCellI
            ) const;

            //- Return the intersection volume between two cells
            scalar interVol
            (
                const polyMesh& src,
                const polyMesh& tgt,
                const label srcCellI,
                const label tgtCellI
            ) const;

            //- Main driver routine for indirect mapping
            void calcIndirect
            (
                const polyMesh& src,
                const polyMesh& tgt,
                const label srcSeedI,
                const label tgtSeedI,
                const labelList& srcCellIDs,
                boolList& mapFlag,
                label& startSeedI
            );


        //- Calculate the addressing between overalping regions of src and tgt
        //  meshes - main driver function
        void calcAddressing(const polyMesh& src, const polyMesh& tgt);


        // Parallel operations

            //- Determine whether the meshes are split across multiple pocessors
            label calcDistribution
            (
                const polyMesh& src,
                const polyMesh& tgt
            ) const;

            //- Determine which processor bounding-boxes overlap
            label calcOverlappingProcs
            (
                const List<boundBox>& procBb,
                const boundBox& bb,
                boolList& overlaps
            ) const;

            //- Calculate the mapping between processors
            autoPtr<mapDistribute> calcProcMap
            (
                const polyMesh& src,
                const polyMesh& tgt
            ) const;

            //- Distribute mesh info from 'my' processor to others
            void distributeCells
            (
                const mapDistribute& map,
                const polyMesh& tgtMesh,
                const globalIndex& globalI,
                List<pointField>& points,
                List<label>& nInternalFaces,
                List<faceList>& faces,
                List<labelList>& faceOwner,
                List<labelList>& faceNeighbour,
                List<labelList>& cellIDs,
                List<labelList>& nbrProcIDs,
                List<labelList>& procLocalFaceIDs
            ) const;

            //- Collect pieces of tgt mesh from other procssors and restructure
            void distributeAndMergeCells
            (
                const mapDistribute& map,
                const polyMesh& tgt,
                const globalIndex& globalI,
                pointField& tgtPoints,
                faceList& tgtFaces,
                labelList& tgtFaceOwners,
                labelList& tgtFaceNeighbours,
                labelList& tgtCellIDs
            ) const;


        //- Disallow default bitwise copy construct
        meshToMeshNew(const meshToMeshNew&);

        //- Disallow default bitwise assignment
        void operator=(const meshToMeshNew&);


public:

    //- Run-time type information
    TypeName("meshToMeshNew");


    //- Construct from source and target meshes
    meshToMeshNew
    (
        const polyMesh& src,
        const polyMesh& tgt,
        const interpolationMethod& method
    );


    //- Destructor
    virtual ~meshToMeshNew();


    // Member Functions

        // Access

            //- Return const access to the source to target cell addressing
            inline const labelListList& srcToTgtCellAddr() const;

            //- Return const access to the target to source cell addressing
            inline const labelListList& tgtToSrcCellAddr() const;

            //- Return const access to the source to target cell weights
            inline const scalarListList& srcToTgtCellWght() const;

            //- Return const access to the target to source cell weights
            inline const scalarListList& tgtToSrcCellWght() const;

            //- Return const access to the overlap volume
            inline scalar V() const;


        // Evaluation

            // Source-to-target field mapping

                //- Map field from src to tgt mesh with defined operation
                //  Values passed in via 'result' are used to initialise the
                //  return value
                template<class Type, class CombineOp>
                void mapSrcToTgt
                (
                    const UList<Type>& srcFld,
                    const CombineOp& cop,
                    List<Type>& result
                ) const;

                //- Return the src field mapped to the tgt mesh with a defined
                //  operation.  Initial values of the result are set to zero
                template<class Type, class CombineOp>
                tmp<Field<Type> > mapSrcToTgt
                (
                    const Field<Type>& srcFld,
                    const CombineOp& cop
                ) const;

                //- Convenience function to map a tmp field to the tgt mesh
                //  with a defined operation
                template<class Type, class CombineOp>
                tmp<Field<Type> > mapSrcToTgt
                (
                    const tmp<Field<Type> >& tsrcFld,
                    const CombineOp& cop
                ) const;

                //- Convenience function to map a field to the tgt mesh with a
                //  default operation (plusEqOp)
                template<class Type>
                tmp<Field<Type> > mapSrcToTgt
                (
                    const Field<Type>& srcFld
                ) const;

                //- Convenience function to map a tmp field to the tgt mesh
                //  with a default operation (plusEqOp)
                template<class Type>
                tmp<Field<Type> > mapSrcToTgt
                (
                    const tmp<Field<Type> >& tsrcFld
                ) const;


            // Target-to-source field mapping

                //- Map field from tgt to src mesh with defined operation
                //  Values passed in via 'result' are used to initialise the
                //  return value
                template<class Type, class CombineOp>
                void mapTgtToSrc
                (
                    const UList<Type>& tgtFld,
                    const CombineOp& cop,
                    List<Type>& result
                ) const;

                //- Return the tgt field mapped to the src mesh with a defined
                //  operation.  Initial values of the result are set to zero
                template<class Type, class CombineOp>
                tmp<Field<Type> > mapTgtToSrc
                (
                    const Field<Type>& tgtFld,
                    const CombineOp& cop
                ) const;

                //- Convenience function to map a tmp field to the src mesh
                //  with a defined operation
                template<class Type, class CombineOp>
                tmp<Field<Type> > mapTgtToSrc
                (
                    const tmp<Field<Type> >& ttgtFld,
                    const CombineOp& cop
                ) const;

                //- Convenience function to map a field to the src mesh with a
                //  default operation (plusEqOp)
                template<class Type>
                tmp<Field<Type> > mapTgtToSrc
                (
                    const Field<Type>& tgtFld
                ) const;

                //- Convenience function to map a tmp field to the src mesh
                //  with a default operation (plusEqOp)
                template<class Type>
                tmp<Field<Type> > mapTgtToSrc
                (
                    const tmp<Field<Type> >& ttgtFld
                ) const;


            // Volume field mapping

                //- Interpolare a field with a defined operation.  Values
                //  passed in via 'result' are used to initialise the return
                //  value.  Optionally interpolate patch values
                template<class Type, class CombineOp>
                void interpolate
                (
                    const GeometricField<Type, fvPatchField, volMesh>& field,
                    const CombineOp& cop,
                    GeometricField<Type, fvPatchField, volMesh>& result,
                    const bool interpPatches = true
                ) const;

                //- Interpolare a field with a defined operation.  The initial
                //  values of the result are set to zero.  Optionally
                //  interpolate patch values
                template<class Type, class CombineOp>
                tmp<GeometricField<Type, fvPatchField, volMesh> > interpolate
                (
                    const GeometricField<Type, fvPatchField, volMesh>& field,
                    const CombineOp& cop,
                    const bool interpPatches = true
                ) const;

                //- Interpolare a tmp field with a defined operation.  The
                //  initial values of the result are set to zero.  Optionally
                //  interpolate patch values
                template<class Type, class CombineOp>
                tmp<GeometricField<Type, fvPatchField, volMesh> > interpolate
                (
                    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
                        tfield,
                    const CombineOp& cop,
                    const bool interpPatches = true
                ) const;

                //- Convenience function to map a field with a default
                //  operation (plusEqOp).  Optionally interpolate patch values
                template<class Type>
                tmp<GeometricField<Type, fvPatchField, volMesh> > interpolate
                (
                    const GeometricField<Type, fvPatchField, volMesh>& field,
                    const bool interpPatches = true
                ) const;

                //- Convenience function to map a tmp field with a default
                //  operation (plusEqOp).  Optionally interpolate patch values
                template<class Type>
                tmp<GeometricField<Type, fvPatchField, volMesh> > interpolate
                (
                    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
                        tfield,
                    const bool interpPatches = true
                ) const;
};


} // End namespace CML

#include "meshToMeshNewI.hpp"

#include "fvMesh.hpp"
#include "volFields.hpp"
//#include "ops.hpp"

namespace CML
{
    //- Helper class for list
    template<class Type>
    class ListPlusEqOp
    {
        public:
        void operator()(List<Type>& x, const List<Type> y) const
        {
            if (y.size())
            {
                if (x.size())
                {
                    label sz = x.size();
                    x.setSize(sz + y.size());
                    forAll(y, i)
                    {
                        x[sz++] = y[i];
                    }
                }
                else
                {
                    x = y;
                }
            }
        }
    };

    //- Combine operator for maps/interpolations
    template<class Type, class CombineOp>
    class combineBinaryOp
    {
        const CombineOp& cop_;

        public:

            combineBinaryOp(const CombineOp& cop)
            :
                cop_(cop)
            {}

            void operator()
            (
                Type& x,
                const label faceI,
                const Type& y,
                const scalar weight
            ) const
            {
                cop_(x, weight*y);
            }
    };
}


template<class Type>
void CML::meshToMeshNew::add
(
    UList<Type>& fld,
    const label offset
) const
{
    forAll(fld, i)
    {
        fld[i] += offset;
    }
}


template<class Type, class CombineOp>
void CML::meshToMeshNew::mapSrcToTgt
(
    const UList<Type>& srcField,
    const CombineOp& cop,
    List<Type>& result
) const
{
    if (result.size() != tgtToSrcCellAddr_.size())
    {
        FatalErrorIn
        (
            "void CML::meshToMeshNew::mapSrcToTgt"
            "("
                "const UList<Type>&, "
                "const CombineOp&, "
                "List<Type>&"
            ") const"
        )   << "Supplied field size is not equal to target mesh size" << nl
            << "    source mesh   = " << srcToTgtCellAddr_.size() << nl
            << "    target mesh   = " << tgtToSrcCellAddr_.size() << nl
            << "    supplied field = " << result.size()
            << abort(FatalError);
    }

    combineBinaryOp<Type, CombineOp> cbop(cop);

    if (singleMeshProc_ == -1)
    {
        const mapDistribute& map = srcMapPtr_();

        List<Type> work(srcField);
        map.distribute(work);

        forAll(result, cellI)
        {
            const labelList& srcAddress = tgtToSrcCellAddr_[cellI];
            const scalarList& srcWeight = tgtToSrcCellWght_[cellI];

            if (srcAddress.size())
            {
//                result[cellI] = pTraits<Type>::zero;
                result[cellI] *= (1.0 - sum(srcWeight));
                forAll(srcAddress, i)
                {
                    label srcI = srcAddress[i];
                    scalar w = srcWeight[i];
                    cbop(result[cellI], cellI, work[srcI], w);
                }
            }
        }
    }
    else
    {
        forAll(result, cellI)
        {
            const labelList& srcAddress = tgtToSrcCellAddr_[cellI];
            const scalarList& srcWeight = tgtToSrcCellWght_[cellI];

            if (srcAddress.size())
            {
//                result[cellI] = pTraits<Type>::zero;
                result[cellI] *= (1.0 - sum(srcWeight));
                forAll(srcAddress, i)
                {
                    label srcI = srcAddress[i];
                    scalar w = srcWeight[i];
                    cbop(result[cellI], cellI, srcField[srcI], w);
                }
            }
        }
    }
}


template<class Type, class CombineOp>
CML::tmp<CML::Field<Type> > CML::meshToMeshNew::mapSrcToTgt
(
    const Field<Type>& srcField,
    const CombineOp& cop
) const
{
    tmp<Field<Type> > tresult
    (
        new Field<Type>
        (
            tgtToSrcCellAddr_.size(),
            pTraits<Type>::zero
        )
    );

    mapSrcToTgt(srcField, cop, tresult());

    return tresult;
}


template<class Type, class CombineOp>
CML::tmp<CML::Field<Type> > CML::meshToMeshNew::mapSrcToTgt
(
    const tmp<Field<Type> >& tsrcField,
    const CombineOp& cop
) const
{
    return mapSrcToTgt(tsrcField(), cop);
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::meshToMeshNew::mapSrcToTgt
(
    const Field<Type>& srcField
) const
{
    return mapSrcToTgt(srcField, plusEqOp<Type>());
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::meshToMeshNew::mapSrcToTgt
(
    const tmp<Field<Type> >& tsrcField
) const
{
    return mapSrcToTgt(tsrcField());
}


template<class Type, class CombineOp>
void CML::meshToMeshNew::mapTgtToSrc
(
    const UList<Type>& tgtField,
    const CombineOp& cop,
    List<Type>& result
) const
{
    if (result.size() != srcToTgtCellAddr_.size())
    {
        FatalErrorIn
        (
            "void CML::meshToMeshNew::mapTgtToSrc"
            "("
                "const UList<Type>&, "
                "const CombineOp&, "
                "List<Type>&"
            ") const"
        )   << "Supplied field size is not equal to source mesh size" << nl
            << "    source mesh   = " << srcToTgtCellAddr_.size() << nl
            << "    target mesh   = " << tgtToSrcCellAddr_.size() << nl
            << "    supplied field = " << result.size()
            << abort(FatalError);
    }

    combineBinaryOp<Type, CombineOp> cbop(cop);

    if (singleMeshProc_ == -1)
    {
        const mapDistribute& map = tgtMapPtr_();

        List<Type> work(tgtField);
        map.distribute(work);

        forAll(result, cellI)
        {
            const labelList& tgtAddress = srcToTgtCellAddr_[cellI];
            const scalarList& tgtWeight = srcToTgtCellWght_[cellI];

            if (tgtAddress.size())
            {
//                result[cellI] = pTraits<Type>::zero;
                result[cellI] *= (1.0 - sum(tgtWeight));
                forAll(tgtAddress, i)
                {
                    label tgtI = tgtAddress[i];
                    scalar w = tgtWeight[i];
                    cbop(result[cellI], cellI, work[tgtI], w);
                }
            }
        }
    }
    else
    {
        forAll(result, cellI)
        {
            const labelList& tgtAddress = srcToTgtCellAddr_[cellI];
            const scalarList& tgtWeight = srcToTgtCellWght_[cellI];

            if (tgtAddress.size())
            {
//                result[cellI] = pTraits<Type>::zero;
                result[cellI] *= (1.0 - sum(tgtWeight));
                forAll(tgtAddress, i)
                {
                    label tgtI = tgtAddress[i];
                    scalar w = tgtWeight[i];
                    cbop(result[cellI], cellI, tgtField[tgtI], w);
                }
            }
        }
    }
}


template<class Type, class CombineOp>
CML::tmp<CML::Field<Type> > CML::meshToMeshNew::mapTgtToSrc
(
    const Field<Type>& tgtField,
    const CombineOp& cop
) const
{
    tmp<Field<Type> > tresult
    (
        new Field<Type>
        (
            srcToTgtCellAddr_.size(),
            pTraits<Type>::zero
        )
    );

    mapTgtToSrc(tgtField, cop, tresult());

    return tresult;
}


template<class Type, class CombineOp>
CML::tmp<CML::Field<Type> > CML::meshToMeshNew::mapTgtToSrc
(
    const tmp<Field<Type> >& ttgtField,
    const CombineOp& cop
) const
{
    return mapTgtToSrc(ttgtField(), cop);
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::meshToMeshNew::mapTgtToSrc
(
    const Field<Type>& tgtField
) const
{
    return mapTgtToSrc(tgtField, plusEqOp<Type>());
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::meshToMeshNew::mapTgtToSrc
(
    const tmp<Field<Type> >& ttgtField
) const
{
    return mapTgtToSrc(ttgtField(), plusEqOp<Type>());
}


template<class Type, class CombineOp>
void CML::meshToMeshNew::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& field,
    const CombineOp& cop,
    GeometricField<Type, fvPatchField, volMesh>& result,
    const bool interpPatches
) const
{
    const fvMesh& mesh = field.mesh();

    if (mesh.name() == srcRegionName_)
    {
        mapSrcToTgt(field, cop, result.internalField());
    }
    else if (mesh.name() == tgtRegionName_)
    {
        mapTgtToSrc(field, cop, result.internalField());
    }
    else
    {
        FatalErrorIn
        (
            "void CML::meshToMeshNew::interpolate"
            "("
                "const GeometricField<Type, fvPatchField, volMesh>&, "
                "const CombineOp&, "
                "GeometricField<Type, fvPatchField, volMesh>&, "
                "const bool"
            ") const"
        )
            << "Supplied field " << field.name() << " did not originate from "
            << "either the source or target meshes used to create this "
            << "interpolation object"
            << abort(FatalError);
    }

    if (interpPatches)
    {
        switch (method_)
        {
            case imMap:
            {
                result.boundaryField() == field.boundaryField();
                break;
            }
            default:
            {
                notImplemented
                (
                    "void CML::meshToMeshNew::interpolate"
                    "("
                        "const GeometricField<Type, fvPatchField, volMesh>&, "
                        "const CombineOp&, "
                        "GeometricField<Type, fvPatchField, volMesh>&, "
                        "const bool"
                    ") const - non-conformal patches"
                )

                // do something...
            }
        }
    }
}


template<class Type, class CombineOp>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::meshToMeshNew::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& field,
    const CombineOp& cop,
    const bool interpPatches
) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    const fvMesh& mesh = field.mesh();

    tmp<fieldType> tresult;

    if (mesh.name() == srcRegionName_)
    {
        const fvMesh& tgtMesh =
            mesh.time().lookupObject<fvMesh>(tgtRegionName_);

        tresult =
            new fieldType
            (
                IOobject
                (
                    type() + "::interpolate(" + field.name() + ")",
                    tgtMesh.time().timeName(),
                    tgtMesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                tgtMesh,
                dimensioned<Type>
                (
                    "zero",
                    field.dimensions(),
                    pTraits<Type>::zero
                )
            );

         interpolate(field, cop, tresult(), interpPatches);
    }
    else if (mesh.name() == tgtRegionName_)
    {
        const fvMesh& srcMesh =
            mesh.time().lookupObject<fvMesh>(srcRegionName_);

        tresult =
            new fieldType
            (
                IOobject
                (
                    type() + "::interpolate(" + field.name() + ")",
                    srcMesh.time().timeName(),
                    srcMesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                srcMesh,
                dimensioned<Type>
                (
                    "zero",
                    field.dimensions(),
                    pTraits<Type>::zero
                )
            );

         interpolate(field, cop, tresult(), interpPatches);
    }

    return tresult;
}


template<class Type, class CombineOp>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::meshToMeshNew::interpolate
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfield,
    const CombineOp& cop,
    const bool interpPatches
) const
{
    return
        interpolate
        (
            tfield(),
            combineBinaryOp<Type, CombineOp>(cop),
            interpPatches
        );
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::meshToMeshNew::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& field,
    const bool interpPatches
) const
{
    return interpolate(field, plusEqOp<Type>(), interpPatches);
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::meshToMeshNew::interpolate
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfield,
    const bool interpPatches
) const
{
    return interpolate(tfield(), plusEqOp<Type>(), interpPatches);
}


#endif


