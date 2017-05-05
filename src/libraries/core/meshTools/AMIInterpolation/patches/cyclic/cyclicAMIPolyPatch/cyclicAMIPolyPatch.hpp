/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::cyclicAMIPolyPatch

Description
    Cyclic patch for Arbitrary Mesh Interface (AMI)

SourceFiles
    cyclicAMIPolyPatch.cpp

\*---------------------------------------------------------------------------*/

#ifndef cyclicAMIPolyPatch_H
#define cyclicAMIPolyPatch_H

#include "coupledPolyPatch.hpp"
#include "AMIPatchToPatchInterpolation.hpp"
#include "polyBoundaryMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class cyclicAMIPolyPatch Declaration
\*---------------------------------------------------------------------------*/

class cyclicAMIPolyPatch
:
    public coupledPolyPatch
{

private:

    // Private data

        //- Name of other half
        const word nbrPatchName_;

        //- Index of other half
        mutable label nbrPatchID_;


        // Transformations

            //- Type of transformation - rotational or translational
            transformType transform_;


            // For rotation

                //- Axis of rotation for rotational cyclics
                vector rotationAxis_;

                //- point on axis of rotation for rotational cyclics
                point rotationCentre_;


            // For translation

                //- Translation vector
                vector separationVector_;


        //- AMI interpolation class
        mutable autoPtr<AMIPatchToPatchInterpolation> AMIPtr_;

        //- Flag to indicate that slave patch should be reversed for AMI
        const bool AMIReverse_;

        //- Projection surface
        mutable autoPtr<searchableSurface> surfPtr_;

        //- Dictionary used during projection surface construction
        const dictionary surfDict_;


    // Private Member Functions

        //- Return normal of face at max distance from rotation axis
        vector findFaceMaxRadius(const pointField& faceCentres) const;

        void calcTransforms
        (
            const primitivePatch& half0,
            const pointField& half0Ctrs,
            const vectorField& half0Areas,
            const pointField& half1Ctrs,
            const vectorField& half1Areas
        );

        //- Reset the AMI interpolator
        void resetAMI() const;


protected:

    // Protected Member Functions

        //- Recalculate the transformation tensors
        virtual void calcTransforms();

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


public:

    //- Runtime type information
    TypeName("cyclicAMI");


    // Constructors

        //- Construct from (base couped patch) components
        cyclicAMIPolyPatch
        (
            const word& name,
            const label size,
            const label start,
            const label index,
            const polyBoundaryMesh& bm
        );

        //- Construct from dictionary
        cyclicAMIPolyPatch
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const polyBoundaryMesh& bm
        );

        //- Construct as copy, resetting the boundary mesh
        cyclicAMIPolyPatch(const cyclicAMIPolyPatch&, const polyBoundaryMesh&);

        //- Construct given the original patch and resetting the
        //  face list and boundary mesh information
        cyclicAMIPolyPatch
        (
            const cyclicAMIPolyPatch& pp,
            const polyBoundaryMesh& bm,
            const label index,
            const label newSize,
            const label newStart,
            const word& nbrPatchName
        );

        //- Construct given the original patch and a map
        cyclicAMIPolyPatch
        (
            const cyclicAMIPolyPatch& pp,
            const polyBoundaryMesh& bm,
            const label index,
            const labelUList& mapAddressing,
            const label newStart
        );


        //- Construct and return a clone, resetting the boundary mesh
        virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
        {
            return autoPtr<polyPatch>(new cyclicAMIPolyPatch(*this, bm));
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
                new cyclicAMIPolyPatch
                (
                    *this,
                    bm,
                    index,
                    newSize,
                    newStart,
                    nbrPatchName_
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
                new cyclicAMIPolyPatch
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
    virtual ~cyclicAMIPolyPatch();


    // Member Functions

        // Access

            //- Is patch 'coupled'. Note that on AMI the geometry is not
            //  coupled but the fields are!
            virtual bool coupled() const
            {
                return false;
            }

            //- Neighbour patch name
            inline const word& neighbPatchName() const;

            //- Neighbour patch ID
            virtual label neighbPatchID() const;

            //- Does this side own the patch?
            virtual bool owner() const;

            //- Return a reference to the neighbour patch
            inline const cyclicAMIPolyPatch& neighbPatch() const;

            //- Return a reference to the projection surface
            const autoPtr<searchableSurface>& surfPtr() const;

            //- Return a reference to the AMI interpolator
            const AMIPatchToPatchInterpolation& AMI() const;


            // Transformations

                //- Type of transform
                inline transformType transform() const;

                //- Axis of rotation for rotational cyclic AMI
                inline const vector& rotationAxis() const;

                //- point on axis of rotation for rotational cyclic AMI
                inline const point& rotationCentre() const;

                //- Translation vector for translational cyclic AMI
                inline const vector& separationVector() const;

                //- Transform patch-based positions from nbr side to this side
                virtual void transformPosition(pointField&) const;

                //- Transform a patch-based position from nbr side to this side
                virtual void transformPosition
                (
                    point& l,
                    const label faceI
                ) const;


            // Interpolations

                //- Interpolate field
                template<class Type>
                tmp<Field<Type> > interpolate(const Field<Type>& fld) const;

                //- Interpolate tmp field
                template<class Type>
                tmp<Field<Type> > interpolate
                (
                    const tmp<Field<Type> >& tFld
                ) const;

                //- Low-level interpolate List
                template<class Type, class BinaryOp>
                void interpolate
                (
                    const UList<Type>& fld,
                    const BinaryOp& bop,
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

inline const CML::word& CML::cyclicAMIPolyPatch::neighbPatchName() const
{
    return nbrPatchName_;
}


inline const CML::cyclicAMIPolyPatch&
CML::cyclicAMIPolyPatch::neighbPatch() const
{
    const polyPatch& pp = this->boundaryMesh()[neighbPatchID()];
    return refCast<const cyclicAMIPolyPatch>(pp);
}


inline CML::coupledPolyPatch::transformType
CML::cyclicAMIPolyPatch::transform() const
{
    return transform_;
}


inline const CML::vector& CML::cyclicAMIPolyPatch::rotationAxis() const
{
    return rotationAxis_;
}


inline const CML::point& CML::cyclicAMIPolyPatch::rotationCentre() const
{
    return rotationCentre_;
}


inline const CML::vector& CML::cyclicAMIPolyPatch::separationVector() const
{
    return separationVector_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> > CML::cyclicAMIPolyPatch::interpolate
(
    const Field<Type>& fld
) const
{
    if (owner())
    {
        return AMI().interpolateToSource(fld);
    }
    else
    {
        return neighbPatch().AMI().interpolateToTarget(fld);
    }
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::cyclicAMIPolyPatch::interpolate
(
    const tmp<Field<Type> >& tFld
) const
{
    if (owner())
    {
        return AMI().interpolateToSource(tFld);
    }
    else
    {
        return neighbPatch().AMI().interpolateToTarget(tFld);
    }
}


template<class Type, class BinaryOp>
void CML::cyclicAMIPolyPatch::interpolate
(
    const UList<Type>& fld,
    const BinaryOp& bop,
    List<Type>& result
) const
{
    if (owner())
    {
        AMI().interpolateToSource(fld, bop, result);
    }
    else
    {
        neighbPatch().AMI().interpolateToTarget(fld, bop, result);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
