/*---------------------------------------------------------------------------*\
Copyright (C) 2013 OpenFOAM Foundation
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
    CML::cyclicACMIPolyPatch

Description
    Cyclic patch for Arbitrarily Coupled Mesh Interface (ACMI)


\*---------------------------------------------------------------------------*/

#ifndef cyclicACMIPolyPatch_H
#define cyclicACMIPolyPatch_H

#include "cyclicAMIPolyPatch.hpp"
#include "AMIPatchToPatchInterpolation.hpp"
#include "polyBoundaryMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class cyclicACMIPolyPatch Declaration
\*---------------------------------------------------------------------------*/

class cyclicACMIPolyPatch
:
    public cyclicAMIPolyPatch
{

private:

    // Private data

        //- Copy of the original patch face areas
        mutable vectorField faceAreas0_;

        //- Fraction of face area below which face is considered disconnected
        static const scalar tolerance_;

        //- Name of non-overlapping patch
        const word nonOverlapPatchName_;

        //- Index of non-overlapping patch
        mutable label nonOverlapPatchID_;

        //- Mask/weighting for source patch
        mutable scalarField srcMask_;

        //- Mask/weighting for target patch
        mutable scalarField tgtMask_;

        //- Flag to indicate that AMI has been updated
        mutable bool updated_;


protected:

    // Protected Member Functions

        //- Initialise patch face areas
        virtual void initPatchFaceAreas() const;

        //- Reset the AMI interpolator
        virtual void resetAMI
        (
            const AMIPatchToPatchInterpolation::interpolationMethod& AMIMethod =
                AMIPatchToPatchInterpolation::imFaceAreaWeight
        ) const;

        //- Set neighbour ACMI patch areas
        virtual void setNeighbourFaceAreas() const;

        //- Initialise the calculation of the patch geometry
        virtual void initGeometry(PstreamBuffers&);

        //- Calculate the patch geometry
        virtual void calcGeometry(PstreamBuffers&);

        //- Initialise the patches for moving points
        virtual void initMovePoints(PstreamBuffers& pBufs, const pointField&);

        //- Correct patches after moving points
        virtual void movePoints(PstreamBuffers& pBufs, const pointField&);

        //- Initialise the update of the patch topology
        virtual void initUpdateMesh(PstreamBuffers&);

        //- Update of the patch topology
        virtual void updateMesh(PstreamBuffers&);

        //- Clear geometry
        virtual void clearGeom();

        //- Return the mask/weighting for the source patch
        virtual const scalarField& srcMask() const;

        //- Return the mask/weighting for the target patch
        virtual const scalarField& tgtMask() const;


public:

    //- Runtime type information
    TypeName("cyclicACMI");


    // Constructors

        //- Construct from (base couped patch) components
        cyclicACMIPolyPatch
        (
            const word& name,
            const label size,
            const label start,
            const label index,
            const polyBoundaryMesh& bm,
            const word& patchType,
            const transformType transform = UNKNOWN
        );

        //- Construct from dictionary
        cyclicACMIPolyPatch
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const polyBoundaryMesh& bm,
            const word& patchType
        );

        //- Construct as copy, resetting the boundary mesh
        cyclicACMIPolyPatch
        (
            const cyclicACMIPolyPatch&,
            const polyBoundaryMesh&
        );

        //- Construct given the original patch and resetting the
        //  face list and boundary mesh information
        cyclicACMIPolyPatch
        (
            const cyclicACMIPolyPatch& pp,
            const polyBoundaryMesh& bm,
            const label index,
            const label newSize,
            const label newStart,
            const word& nbrPatchName,
            const word& nonOverlapPatchName
        );

        //- Construct given the original patch and a map
        cyclicACMIPolyPatch
        (
            const cyclicACMIPolyPatch& pp,
            const polyBoundaryMesh& bm,
            const label index,
            const labelUList& mapAddressing,
            const label newStart
        );


        //- Construct and return a clone, resetting the boundary mesh
        virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
        {
            return autoPtr<polyPatch>(new cyclicACMIPolyPatch(*this, bm));
        }

        //- Construct and return a clone, resetting the face list
        //  and boundary mesh
        virtual autoPtr<polyPatch> clone
        (
            const polyBoundaryMesh& bm,
            const label index,
            const label newSize,
            const label newStart
        ) const
        {
            return autoPtr<polyPatch>
            (
                new cyclicACMIPolyPatch
                (
                    *this,
                    bm,
                    index,
                    newSize,
                    newStart,
                    neighbPatchName(),
                    nonOverlapPatchName_
                )
            );
        }

        //- Construct and return a clone, resetting the face list
        //  and boundary mesh
        virtual autoPtr<polyPatch> clone
        (
            const polyBoundaryMesh& bm,
            const label index,
            const labelUList& mapAddressing,
            const label newStart
        ) const
        {
            return autoPtr<polyPatch>
            (
                new cyclicACMIPolyPatch
                (
                    *this,
                    bm,
                    index,
                    mapAddressing,
                    newStart
                )
            );
        }


    //- Destructor
    virtual ~cyclicACMIPolyPatch();


    // Member Functions

        // Access

            //- Reset the updated flag
            inline void setUpdated(bool flag) const;

            //- Return access to the updated flag
            inline bool updated() const;

            //- Return access to the original patch face areas
            inline const vectorField& faceAreas0() const;

            //- Return a reference to the neighbour patch
            virtual const cyclicACMIPolyPatch& neighbPatch() const;

            //- Non-overlapping patch name
            inline const word& nonOverlapPatchName() const;

            //- Non-overlapping patch ID
            virtual label nonOverlapPatchID() const;

            //- Return a const reference to the non-overlapping patch
            inline const polyPatch& nonOverlapPatch() const;

            //- Return a reference to the non-overlapping patch
            inline polyPatch& nonOverlapPatch();

            //- Mask field where 1 = overlap, 0 = no-overlap
            inline const scalarField& mask() const;

            //- Overlap tolerance
            inline static scalar tolerance();

            // Interpolations

                //- Interpolate field
                template<class Type>
                tmp<Field<Type> > interpolate
                (
                    const Field<Type>& fldCouple,
                    const Field<Type>& fldNonOverlap
                ) const;

                //- Interpolate tmp field
                template<class Type>
                tmp<Field<Type> > interpolate
                (
                    const tmp<Field<Type> >& tFldCouple,
                    const tmp<Field<Type> >& tFldNonOverlap
                ) const;

                //- Low-level interpolate List
                template<class Type, class CombineOp>
                void interpolate
                (
                    const UList<Type>& fldCouple,
                    const UList<Type>& fldNonOverlap,
                    const CombineOp& cop,
                    List<Type>& result
                ) const;


        //- Calculate the patch geometry
        virtual void calcGeometry
        (
            const primitivePatch& referPatch,
            const pointField& thisCtrs,
            const vectorField& thisAreas,
            const pointField& thisCc,
            const pointField& nbrCtrs,
            const vectorField& nbrAreas,
            const pointField& nbrCc
        );

        //- Initialize ordering for primitivePatch. Does not
        //  refer to *this (except for name() and type() etc.)
        virtual void initOrder
        (
            PstreamBuffers&,
            const primitivePatch&
        ) const;

        //- Return new ordering for primitivePatch.
        //  Ordering is -faceMap: for every face
        //  index of the new face -rotation:for every new face the clockwise
        //  shift of the original face. Return false if nothing changes
        //  (faceMap is identity, rotation is 0), true otherwise.
        virtual bool order
        (
            PstreamBuffers&,
            const primitivePatch&,
            labelList& faceMap,
            labelList& rotation
        ) const;

        //- Write the polyPatch data as a dictionary
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


inline void CML::cyclicACMIPolyPatch::setUpdated(const bool flag) const
{
    updated_ = flag;
}


inline bool CML::cyclicACMIPolyPatch::updated() const
{
    return updated_;
}


inline const CML::vectorField& CML::cyclicACMIPolyPatch::faceAreas0() const
{
    return faceAreas0_;
}


inline const CML::word& CML::cyclicACMIPolyPatch::nonOverlapPatchName() const
{
    return nonOverlapPatchName_;
}


inline const CML::polyPatch& CML::cyclicACMIPolyPatch::nonOverlapPatch() const
{
    // note: use nonOverlapPatchID() as opposed to patch name to initialise
    // demand-driven data

    return this->boundaryMesh()[nonOverlapPatchID()];
}


inline CML::polyPatch& CML::cyclicACMIPolyPatch::nonOverlapPatch()
{
    // note: use nonOverlapPatchID() as opposed to patch name to initialise
    // demand-driven data

    return const_cast<polyPatch&>(this->boundaryMesh()[nonOverlapPatchID()]);
}


inline const CML::scalarField& CML::cyclicACMIPolyPatch::mask() const
{
    if (owner())
    {
        return srcMask_;
    }
    else
    {
        return neighbPatch().tgtMask();
    }
}


inline CML::scalar CML::cyclicACMIPolyPatch::tolerance()
{
    return tolerance_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class Type>
CML::tmp<CML::Field<Type> > CML::cyclicACMIPolyPatch::interpolate
(
    const Field<Type>& fldCouple,
    const Field<Type>& fldNonOverlap
) const
{
    // note: do not scale AMI field as face areas have already been taken
    // into account

    if (owner())
    {
        const scalarField& w = srcMask_;

        tmp<Field<Type> > interpField(AMI().interpolateToSource(fldCouple));

        return interpField + (1.0 - w)*fldNonOverlap;
    }
    else
    {
        const scalarField& w = neighbPatch().tgtMask();

        tmp<Field<Type> > interpField
        (
            neighbPatch().AMI().interpolateToTarget(fldCouple)
        );

        return interpField + (1.0 - w)*fldNonOverlap;
    }
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::cyclicACMIPolyPatch::interpolate
(
    const tmp<Field<Type> >& tFldCouple,
    const tmp<Field<Type> >& tFldNonOverlap
) const
{
    return interpolate(tFldCouple(), tFldNonOverlap());
}


template<class Type, class CombineOp>
void CML::cyclicACMIPolyPatch::interpolate
(
    const UList<Type>& fldCouple,
    const UList<Type>& fldNonOverlap,
    const CombineOp& cop,
    List<Type>& result
) const
{
    // note: do not scale AMI field as face areas have already been taken
    // into account

    if (owner())
    {
        const scalarField& w = srcMask_;

        AMI().interpolateToSource(fldCouple, cop, result);
        result = result + (1.0 - w)*fldNonOverlap;
    }
    else
    {
        const scalarField& w = neighbPatch().tgtMask();

        neighbPatch().AMI().interpolateToTarget(fldCouple, cop, result);
        result = result + (1.0 - w)*fldNonOverlap;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
