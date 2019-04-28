/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    CML::topoDistanceData

Description
    For use with FaceCellWave. Determines topological distance to starting faces

SourceFiles
    topoDistanceDataI.hpp
    topoDistanceData.cpp

\*---------------------------------------------------------------------------*/

#ifndef topoDistanceData_H
#define topoDistanceData_H

#include "point.hpp"
#include "tensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class polyPatch;
class polyMesh;

// Forward declaration of friend functions and operators

class topoDistanceData;

Istream& operator>>(Istream&, topoDistanceData&);
Ostream& operator<<(Ostream&, const topoDistanceData&);


/*---------------------------------------------------------------------------*\
                           Class topoDistanceData Declaration
\*---------------------------------------------------------------------------*/

class topoDistanceData
{
    // Private data

        //- Starting data
        label data_;

        //- Distance
        label distance_;


public:

    // Constructors

        //- Construct null
        inline topoDistanceData();

        //- Construct from count
        inline topoDistanceData
        (
            const label data,
            const label distance
        );


    // Member Functions

        // Access


            inline label data() const
            {
                return data_;
            }
            inline label distance() const
            {
                return distance_;
            }


        // Needed by FaceCellWave

            //- Check whether origin has been changed at all or
            //  still contains original (invalid) value.
            template<class TrackingData>
            inline bool valid(TrackingData& td) const;

            //- Check for identical geometrical data. Used for cyclics checking.
            template<class TrackingData>
            inline bool sameGeometry
            (
                const polyMesh&,
                const topoDistanceData&,
                const scalar,
                TrackingData& td
            ) const;

            //- Convert any absolute coordinates into relative to (patch)face
            //  centre
            template<class TrackingData>
            inline void leaveDomain
            (
                const polyMesh&,
                const polyPatch&,
                const label patchFaceI,
                const point& faceCentre,
                TrackingData& td
            );

            //- Reverse of leaveDomain
            template<class TrackingData>
            inline void enterDomain
            (
                const polyMesh&,
                const polyPatch&,
                const label patchFaceI,
                const point& faceCentre,
                TrackingData& td
            );

            //- Apply rotation matrix to any coordinates
            template<class TrackingData>
            inline void transform
            (
                const polyMesh&,
                const tensor&,
                TrackingData& td
            );

            //- Influence of neighbouring face.
            template<class TrackingData>
            inline bool updateCell
            (
                const polyMesh&,
                const label thisCellI,
                const label neighbourFaceI,
                const topoDistanceData& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of neighbouring cell.
            template<class TrackingData>
            inline bool updateFace
            (
                const polyMesh&,
                const label thisFaceI,
                const label neighbourCellI,
                const topoDistanceData& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of different value on same face.
            template<class TrackingData>
            inline bool updateFace
            (
                const polyMesh&,
                const label thisFaceI,
                const topoDistanceData& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Same (like operator==)
            template<class TrackingData>
            inline bool equal(const topoDistanceData&, TrackingData& td) const;

    // Member Operators

        // Needed for List IO
        inline bool operator==(const topoDistanceData&) const;

        inline bool operator!=(const topoDistanceData&) const;


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const topoDistanceData&);
        friend Istream& operator>>(Istream&, topoDistanceData&);
};


//- Data associated with topoDistanceData type are contiguous
template<>
inline bool contiguous<topoDistanceData>()
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "topoDistanceDataI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
