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
    CML::extendedCentredCellToFaceStencil

Description

SourceFiles
    extendedCentredCellToFaceStencil.cpp

\*---------------------------------------------------------------------------*/

#ifndef extendedCentredCellToFaceStencil_H
#define extendedCentredCellToFaceStencil_H

#include "extendedCellToFaceStencil.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class cellToFaceStencil;

/*---------------------------------------------------------------------------*\
              Class extendedCentredCellToFaceStencil Declaration
\*---------------------------------------------------------------------------*/

class extendedCentredCellToFaceStencil
:
    public extendedCellToFaceStencil
{
    // Private data

        //- Swap map for getting neigbouring data
        autoPtr<mapDistribute> mapPtr_;

        //- Per face the stencil.
        labelListList stencil_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        extendedCentredCellToFaceStencil
        (
            const extendedCentredCellToFaceStencil&
        );

        //- Disallow default bitwise assignment
        void operator=(const extendedCentredCellToFaceStencil&);


public:

    // Constructors

        //- Construct from uncompacted face stencil
        explicit extendedCentredCellToFaceStencil(const cellToFaceStencil&);


    // Member Functions

        //- Return reference to the parallel distribution map
        const mapDistribute& map() const
        {
            return mapPtr_();
        }

        //- Return reference to the stencil
        const labelListList& stencil() const
        {
            return stencil_;
        }

        //- After removing elements from the stencil adapt the schedule (map).
        void compact();

        //- Use map to get the data into stencil order
        template<class T>
        void collectData
        (
            const GeometricField<T, fvPatchField, volMesh>& fld,
            List<List<T> >& stencilFld
        ) const
        {
            extendedCellToFaceStencil::collectData
            (
                map(),
                stencil(),
                fld,
                stencilFld
            );
        }

        //- Sum vol field contributions to create face values
        template<class Type>
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > weightedSum
        (
            const GeometricField<Type, fvPatchField, volMesh>& fld,
            const List<List<scalar> >& stencilWeights
        ) const
        {
            return extendedCellToFaceStencil::weightedSum
            (
                map(),
                stencil(),
                fld,
                stencilWeights
            );
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
