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
    CML::ptscotchDecomp

Description
    PTScotch domain decomposition

SourceFiles
    ptscotchDecomp.cpp

\*---------------------------------------------------------------------------*/

#ifndef ptscotchDecomp_H
#define ptscotchDecomp_H

#include "decompositionMethod.hpp"

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class ptscotchDecomp Declaration
\*---------------------------------------------------------------------------*/

class ptscotchDecomp
:
    public decompositionMethod
{
    // Private Member Functions

        //- Insert list in front of list.
        template<class Type>
        static void prepend(const UList<Type>&, List<Type>&);
        //- Insert list at end of list.
        template<class Type>
        static void append(const UList<Type>&, List<Type>&);

        //- Check and print error message
        static void check(const int, const char*);

        //- Decompose. Handles size 0 arrays
        label decompose
        (
            const fileName& meshPath,
            const List<label>& adjncy,
            const List<label>& xadj,
            const scalarField& cWeights,
            List<label>& finalDecomp
        ) const;

        //- Low level decompose
        label decompose
        (
            const fileName& meshPath,
            const label adjncySize,
            const label adjncy[],
            const label xadjSize,
            const label xadj[],
            const scalarField& cWeights,
            List<label>& finalDecomp
        ) const;

        //- Disallow default bitwise copy construct and assignment
        void operator=(const ptscotchDecomp&);
        ptscotchDecomp(const ptscotchDecomp&);


public:

    //- Runtime type information
    TypeName("ptscotch");


    // Constructors

        //- Construct given the decomposition dictionary and mesh
        ptscotchDecomp(const dictionary& decompositionDict);


    //- Destructor
    virtual ~ptscotchDecomp()
    {}


    // Member Functions

        virtual bool parallelAware() const
        {
            // ptscotch does not know about proc boundaries
            return true;
        }
    
        //- Inherit decompose from decompositionMethod
        using decompositionMethod::decompose;
    
        //- Return for every coordinate the wanted processor number. Use the
        //  mesh connectivity (if needed). See note on weights in scotchDecomp.H
        virtual labelList decompose
        (
            const polyMesh& mesh,
            const pointField& points,
            const scalarField& pointWeights
        );

        //- Return for every coordinate the wanted processor number. Gets
        //  passed agglomeration map (from fine to coarse cells) and coarse cell
        //  location. Can be overridden by decomposers that provide this
        //  functionality natively. See note on weights in scotchDecomp.H
        virtual labelList decompose
        (
            const polyMesh& mesh,
            const labelList& agglom,
            const pointField& regionPoints,
            const scalarField& regionWeights
        );

        //- Return for every coordinate the wanted processor number. Explicitly
        //  provided mesh connectivity.
        //  The connectivity is equal to mesh.cellCells() except for
        //  - in parallel the cell numbers are global cell numbers (starting
        //    from 0 at processor0 and then incrementing all through the
        //    processors)
        //  - the connections are across coupled patches
        //  See note on weights in scotchDecomp.H
        virtual labelList decompose
        (
            const labelListList& globalCellCells,
            const pointField& cc,
            const scalarField& cWeights
        );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Insert at front of list
template<class Type>
void CML::ptscotchDecomp::prepend
(
    const UList<Type>& extraLst,
    List<Type>& lst
)
{
    label nExtra = extraLst.size();

    // Make space for initial elements
    lst.setSize(lst.size() + nExtra);
    for (label i = lst.size()-1; i >= nExtra; i--)
    {
        lst[i] = lst[i-nExtra];
    }

    // Insert at front
    forAll(extraLst, i)
    {
        lst[i] = extraLst[i];
    }
}


// Insert at back of list
template<class Type>
void CML::ptscotchDecomp::append
(
    const UList<Type>& extraLst,
    List<Type>& lst
)
{
    label sz = lst.size();

    // Make space for initial elements
    lst.setSize(sz + extraLst.size());

    // Insert at back
    forAll(extraLst, i)
    {
        lst[sz++] = extraLst[i];
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
