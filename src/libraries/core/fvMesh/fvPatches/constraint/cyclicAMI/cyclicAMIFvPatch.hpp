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
    CML::cyclicAMIFvPatch

Description
    Cyclic patch for Arbitrary Mesh Interface (AMI)

SourceFiles
    cyclicAMIFvPatch.cpp

\*---------------------------------------------------------------------------*/

#ifndef cyclicAMIFvPatch_H
#define cyclicAMIFvPatch_H

#include "coupledFvPatch.hpp"
#include "cyclicAMILduInterface.hpp"
#include "cyclicAMIPolyPatch.hpp"
#include "fvBoundaryMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class cyclicAMIFvPatch Declaration
\*---------------------------------------------------------------------------*/

class cyclicAMIFvPatch
:
    public coupledFvPatch,
    public cyclicAMILduInterface
{
    // Private data

        const cyclicAMIPolyPatch& cyclicAMIPolyPatch_;


protected:

    // Protected Member functions

        //- Make patch weighting factors
        void makeWeights(scalarField&) const;


public:

    //- Runtime type information
    TypeName(cyclicAMIPolyPatch::typeName_());


    // Constructors

        //- Construct from polyPatch
        cyclicAMIFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
        :
            coupledFvPatch(patch, bm),
            cyclicAMILduInterface(),
            cyclicAMIPolyPatch_(refCast<const cyclicAMIPolyPatch>(patch))
        {}


    // Member functions

        // Access

            //- Return local reference cast into the cyclic patch
            const cyclicAMIPolyPatch& cyclicAMIPatch() const
            {
                return cyclicAMIPolyPatch_;
            }

            //- Return neighbour
            virtual label neighbPatchID() const
            {
                return cyclicAMIPolyPatch_.neighbPatchID();
            }

            virtual bool owner() const
            {
                return cyclicAMIPolyPatch_.owner();
            }

            //- Return processor number
            virtual const cyclicAMIFvPatch& neighbPatch() const
            {
                return refCast<const cyclicAMIFvPatch>
                (
                    this->boundaryMesh()[cyclicAMIPolyPatch_.neighbPatchID()]
                );
            }

            //- Return a reference to the AMI interpolator
            virtual const AMIPatchToPatchInterpolation& AMI() const
            {
                return cyclicAMIPolyPatch_.AMI();
            }

            //- Are the cyclic planes parallel
            virtual bool parallel() const
            {
                return cyclicAMIPolyPatch_.parallel();
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return cyclicAMIPolyPatch_.forwardT();
            }

            //- Return neighbour-cell transformation tensor
            virtual const tensorField& reverseT() const
            {
                return cyclicAMIPolyPatch_.reverseT();
            }

            const cyclicAMIFvPatch& neighbFvPatch() const
            {
                return refCast<const cyclicAMIFvPatch>
                (
                    this->boundaryMesh()[cyclicAMIPolyPatch_.neighbPatchID()]
                );
            }

            //- Return delta (P to N) vectors across coupled patch
            virtual tmp<vectorField> delta() const;

            template<class Type>
            tmp<Field<Type> > interpolate(const Field<Type>& fld) const
            {
                return cyclicAMIPolyPatch_.interpolate(fld);
            }

            template<class Type>
            tmp<Field<Type> > interpolate(const tmp<Field<Type> >& tFld) const
            {
                return cyclicAMIPolyPatch_.interpolate(tFld);
            }


        // Interface transfer functions

            //- Return the values of the given internal data adjacent to
            //  the interface as a field
            virtual tmp<labelField> interfaceInternalField
            (
                const labelUList& internalData
            ) const;

            //- Return neighbour field
            virtual tmp<labelField> internalFieldTransfer
            (
                const Pstream::commsTypes commsType,
                const labelUList& internalData
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
