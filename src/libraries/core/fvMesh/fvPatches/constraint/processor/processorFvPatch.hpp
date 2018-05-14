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
    CML::processorFvPatch

Description
    Processor patch.

SourceFiles
    processorFvPatch.cpp

\*---------------------------------------------------------------------------*/

#ifndef processorFvPatch_H
#define processorFvPatch_H

#include "coupledFvPatch.hpp"
#include "processorLduInterface.hpp"
#include "processorPolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class processorFvPatch Declaration
\*---------------------------------------------------------------------------*/

class processorFvPatch
:
    public coupledFvPatch,
    public processorLduInterface
{
    // Private Data

        const processorPolyPatch& procPolyPatch_;


protected:

    // Protected Member functions

        //- Make patch weighting factors
        void makeWeights(scalarField&) const;


public:

    //- Runtime type information
    TypeName(processorPolyPatch::typeName_());


    // Constructors

        //- Construct from components
        processorFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
        :
            coupledFvPatch(patch, bm),
            procPolyPatch_(refCast<const processorPolyPatch>(patch))
        {}


    // Member functions

        //- Return processor number
        int myProcNo() const
        {
            return procPolyPatch_.myProcNo();
        }

        //- Return neighbour processor number
        int neighbProcNo() const
        {
            return procPolyPatch_.neighbProcNo();
        }

        //- Return message tag used for sending
        virtual int tag() const
        {
            return UPstream::msgType();
        }

        //- Return true if running parallel
        virtual bool coupled() const
        {
            if (Pstream::parRun())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        const processorPolyPatch& procPolyPatch() const
        {
            return procPolyPatch_;
        }

        //- Are the cyclic planes parallel
        virtual bool parallel() const
        {
            return procPolyPatch_.parallel();
        }

        //- Return face transformation tensor
        virtual const tensorField& forwardT() const
        {
            return procPolyPatch_.forwardT();
        }

        //- Return neighbour-cell transformation tensor.
        virtual const tensorField& reverseT() const
        {
            return procPolyPatch_.reverseT();
        }

        //- Return delta (P to N) vectors across coupled patch
        virtual tmp<vectorField> delta() const;

        //- Return delta (P to N) vectors across coupled patch
        virtual tmp<vectorField> deltaFull() const;


        // Interface transfer functions

            //- Return the values of the given internal data adjacent to
            //  the interface as a field
            virtual tmp<labelField> interfaceInternalField
            (
                const labelUList& internalData
            ) const;
    
            //- Inherit initInternalFieldTransfer from coupledFvPatch
            using coupledFvPatch::initInternalFieldTransfer;
    
            //- Initialise neighbour field transfer
            virtual void initInternalFieldTransfer
            (
                const Pstream::commsTypes commsType,
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
