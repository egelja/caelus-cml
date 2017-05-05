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
    CML::channelIndex

Description
    Does averaging of fields over layers of cells. Assumes layered mesh.

SourceFiles
    channelIndex.cpp

\*---------------------------------------------------------------------------*/

#ifndef channelIndex_H
#define channelIndex_H

#include "regionSplit.hpp"
#include "direction.hpp"
#include "scalarField.hpp"
#include "polyMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


/*---------------------------------------------------------------------------*\
                           Class channelIndex Declaration
\*---------------------------------------------------------------------------*/

class channelIndex
{

    // Private data

        static const NamedEnum<vector::components, 3> vectorComponentsNames_;

        //- Is mesh symmetric
        const bool symmetric_;

        //- direction to sort
        const direction dir_;

        //- Per cell the global region
        autoPtr<regionSplit> cellRegion_;

        //- Per global region the number of cells (scalarField so we can use
        //  field algebra)
        scalarField regionCount_;

        //- From sorted region back to unsorted global region
        labelList sortMap_;

        //- Sorted component of cell centres
        scalarField y_;



    // Private Member Functions

        void walkOppositeFaces
        (
            const polyMesh& mesh,
            const labelList& startFaces,
            boolList& blockedFace
        );

        void calcLayeredRegions
        (
            const polyMesh& mesh,
            const labelList& startFaces
        );

        //- Disallow default bitwise copy construct and assignment
        channelIndex(const channelIndex&);
        void operator=(const channelIndex&);


public:

    // Constructors

        //- Construct from dictionary
        channelIndex(const polyMesh&, const dictionary&);

        //- Construct from supplied starting faces
        channelIndex
        (
            const polyMesh& mesh,
            const labelList& startFaces,
            const bool symmetric,
            const direction dir
        );


    // Member Functions

        // Access

            //- Sum field per region
            template<class T>
            Field<T> regionSum(const Field<T>& cellField) const;

            //- collapse a field to a line
            template<class T>
            Field<T> collapse
            (
                const Field<T>& vsf,
                const bool asymmetric=false
            ) const;

            //- return the field of Y locations from the cell centres
            const scalarField& y() const
            {
                return y_;
            }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
CML::Field<T> CML::channelIndex::regionSum(const Field<T>& cellField) const
{
    Field<T> regionField(cellRegion_().nRegions(), pTraits<T>::zero);

    forAll(cellRegion_(), cellI)
    {
        regionField[cellRegion_()[cellI]] += cellField[cellI];
    }

    // Global sum
    Pstream::listCombineGather(regionField, plusEqOp<T>());
    Pstream::listCombineScatter(regionField);

    return regionField;
}


template<class T>
CML::Field<T> CML::channelIndex::collapse
(
    const Field<T>& cellField,
    const bool asymmetric
) const
{
    // Average and order
    const Field<T> summedField(regionSum(cellField));

    Field<T> regionField
    (
        summedField
      / regionCount_,
        sortMap_
    );

    // Symmetry?
    if (symmetric_)
    {
        label nlb2 = cellRegion_().nRegions()/2;

        if (asymmetric)
        {
            for (label j=0; j<nlb2; j++)
            {
                regionField[j] =
                    0.5
                  * (
                        regionField[j]
                      - regionField[cellRegion_().nRegions() - j - 1]
                    );
            }
        }
        else
        {
            for (label j=0; j<nlb2; j++)
            {
                regionField[j] =
                    0.5
                  * (
                        regionField[j]
                      + regionField[cellRegion_().nRegions() - j - 1]
                    );
            }
        }

        regionField.setSize(nlb2);
    }

    return regionField;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
