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
    CML::cuttingPlane

Description
    Constructs plane through mesh.

    No attempt at resolving degenerate cases. Since the cut faces are
    usually quite ugly, they will always be triangulated.

Note
    When the cutting plane coincides with a mesh face, the cell edge on the
    positive side of the plane is taken.

SourceFiles
    cuttingPlane.cpp

\*---------------------------------------------------------------------------*/

#ifndef cuttingPlane_H
#define cuttingPlane_H

#include "plane.hpp"
#include "pointField.hpp"
#include "faceList.hpp"
#include "MeshedSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class primitiveMesh;

/*---------------------------------------------------------------------------*\
                       Class cuttingPlane Declaration
\*---------------------------------------------------------------------------*/

class cuttingPlane
:
    public plane,
    public MeshedSurface<face>
{
    //- Private typedefs for convenience
        typedef MeshedSurface<face> MeshStorage;

    // Private data

        //- List of cells cut by the plane
        labelList cutCells_;

    // Private Member Functions

        //- Determine cut cells, possibly restricted to a list of cells
        void calcCutCells
        (
            const primitiveMesh&,
            const scalarField& dotProducts,
            const labelUList& cellIdLabels = labelUList::null()
        );

        //- Determine intersection points (cutPoints).
        void intersectEdges
        (
            const primitiveMesh&,
            const scalarField& dotProducts,
            List<label>& edgePoint
        );

        //- Walk circumference of cell, starting from startEdgeI crossing
        //  only cut edges. Record cutPoint labels in faceVerts.
        static bool walkCell
        (
            const primitiveMesh&,
            const labelUList& edgePoint,
            const label cellI,
            const label startEdgeI,
            DynamicList<label>& faceVerts
        );

        //- Determine cuts for all cut cells.
        void walkCellCuts
        (
            const primitiveMesh& mesh,
            const bool triangulate,
            const labelUList& edgePoint
        );


protected:

    // Constructors

        //- Construct plane description without cutting
        cuttingPlane(const plane&);

    // Protected Member Functions

        //- recut mesh with existing planeDesc, restricted to a list of cells
        void reCut
        (
            const primitiveMesh&,
            const bool triangulate,
            const labelUList& cellIdLabels = labelUList::null()
        );

        //- remap action on triangulation or cleanup
        virtual void remapFaces(const labelUList& faceMap);

public:

    // Constructors

        //- Construct from plane and mesh reference,
        //  possibly restricted to a list of cells
        cuttingPlane
        (
            const plane&,
            const primitiveMesh&,
            const bool triangulate,
            const labelUList& cellIdLabels = labelUList::null()
        );


    // Member Functions

        //- Return plane used
        const plane& planeDesc() const
        {
            return static_cast<const plane&>(*this);
        }

        //- Return List of cells cut by the plane
        const labelList& cutCells() const
        {
            return cutCells_;
        }

        //- Return true or false to question: have any cells been cut?
        bool cut() const
        {
            return cutCells_.size();
        }

        //- Sample the cell field
        template<class Type>
        tmp<Field<Type> > sample(const Field<Type>&) const;

        template<class Type>
        tmp<Field<Type> > sample(const tmp<Field<Type> >&) const;


    // Member Operators

        void operator=(const cuttingPlane&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> > CML::cuttingPlane::sample
(
    const Field<Type>& fld
) const
{
    return tmp<Field<Type> >(new Field<Type>(fld, cutCells()));
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::cuttingPlane::sample
(
    const tmp<Field<Type> >& tfld
) const
{
    tmp<Field<Type> > tsf = sample(tfld());
    tfld.clear();
    return tsf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
