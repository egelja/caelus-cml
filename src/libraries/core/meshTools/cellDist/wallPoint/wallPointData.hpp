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
    CML::wallPointData

Description
    Holds information (coordinate and normal) regarding nearest wall point.

    Is like wallPoint but transfer extra (passive) data.
    Used e.g. in wall distance calculation with wall reflection vectors.

SourceFiles
    wallPointDataI.hpp
    wallPointData.cpp

\*---------------------------------------------------------------------------*/

#ifndef wallPointData_H
#define wallPointData_H

#include "wallPoint.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type> class wallPointData;

// Forward declaration of friend functions and operators

template<class Type> Istream& operator>>(Istream&, wallPointData<Type>&);
template<class Type> Ostream& operator<<(Ostream&, const wallPointData<Type>&);


/*---------------------------------------------------------------------------*\
                           Class wallPointData Declaration
\*---------------------------------------------------------------------------*/

template <class Type>
class wallPointData
:
    public wallPoint
{
    // Private data

        //- data at nearest wall center
        Type data_;


    // Private Member Functions

        //- Evaluate distance to point. Update distSqr, origin from whomever
        //  is nearer pt. Return true if w2 is closer to point,
        //  false otherwise.
        template<class TrackingData>
        inline bool update
        (
            const point&,
            const wallPointData<Type>& w2,
            const scalar tol,
            TrackingData& td
        );


public:

    typedef Type dataType;


    // Constructors

        //- Construct null
        inline wallPointData();

        //- Construct from origin, normal, distance
        inline wallPointData
        (
            const point& origin,
            const Type& data,
            const scalar distSqr
        );


    // Member Functions

        // Access

            inline const Type& data() const;
            inline Type& data();

        // Needed by meshWave

            //- Influence of neighbouring face.
            //  Calls update(...) with cellCentre of cellI
            template<class TrackingData>
            inline bool updateCell
            (
                const polyMesh& mesh,
                const label thisCellI,
                const label neighbourFaceI,
                const wallPointData<Type>& neighbourWallInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of neighbouring cell.
            //  Calls update(...) with faceCentre of faceI
            template<class TrackingData>
            inline bool updateFace
            (
                const polyMesh& mesh,
                const label thisFaceI,
                const label neighbourCellI,
                const wallPointData<Type>& neighbourWallInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of different value on same face.
            //  Merge new and old info.
            //  Calls update(...) with faceCentre of faceI
            template<class TrackingData>
            inline bool updateFace
            (
                const polyMesh& mesh,
                const label thisFaceI,
                const wallPointData<Type>& neighbourWallInfo,
                const scalar tol,
                TrackingData& td
            );

    // Member Operators

    // IOstream Operators

        friend Ostream& operator<< <Type>(Ostream&, const wallPointData<Type>&);
        friend Istream& operator>> <Type>(Istream&, wallPointData<Type>&);
};


//- Data associated with wallPointData type are contiguous. List the usual
//  ones.

template <>
inline bool contiguous<wallPointData<bool> >()
{
    return contiguous<wallPoint>();
}
template <>
inline bool contiguous<wallPointData<label> >()
{
    return contiguous<wallPoint>();
}
template <>
inline bool contiguous<wallPointData<scalar> >()
{
    return contiguous<wallPoint>();
}
template <>
inline bool contiguous<wallPointData<vector> >()
{
    return contiguous<wallPoint>();
}
template <>
inline bool contiguous<wallPointData<sphericalTensor> >()
{
    return contiguous<wallPoint>();
}
template <>
inline bool contiguous<wallPointData<symmTensor> >()
{
    return contiguous<wallPoint>();
}
template <>
inline bool contiguous<wallPointData<tensor> >()
{
    return contiguous<wallPoint>();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template <class Type>
Ostream& operator<<
(
    Ostream& os,
    const wallPointData<Type>& wDist
)
{
    operator<<(os, static_cast<const wallPoint&>(wDist));

    return os << wDist.data();
}


template <class Type>
Istream& operator>>
(
    Istream& is,
    wallPointData<Type>& wDist
)
{
    operator>>(is, static_cast<wallPoint&>(wDist));

    return is >> wDist.data_;
}


// ************************************************************************* //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Update this with w2 if w2 nearer to pt.
template <class Type>
template <class TrackingData>
inline bool wallPointData<Type>::update
(
    const point& pt,
    const wallPointData<Type>& w2,
    const scalar tol,
    TrackingData& td
)
{
    scalar dist2 = magSqr(pt - w2.origin());

    if (valid(td))
    {
        scalar diff = distSqr() - dist2;

        if (diff < 0)
        {
            // already nearer to pt
            return false;
        }

        if ((diff < SMALL) || ((distSqr() > SMALL) && (diff/distSqr() < tol)))
        {
            // don't propagate small changes
            return false;
        }
    }

    // Either *this is not yet valid or w2 is closer
    {
        // current not yet set so use any value
        distSqr() = dist2;
        origin() = w2.origin();
        data_ = w2.data();

        return true;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Null constructor
template <class Type>
inline wallPointData<Type>::wallPointData()
:
    wallPoint(),
    data_()
{}


// Construct from components
template <class Type>
inline wallPointData<Type>::wallPointData
(
    const point& origin,
    const Type& data,
    const scalar distSqr
)
:
    wallPoint(origin, distSqr),
    data_(data)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class Type>
inline const Type& wallPointData<Type>::data() const
{
    return data_;
}


template <class Type>
inline Type& wallPointData<Type>::data()
{
    return data_;
}


// Update this with w2 if w2 nearer to pt.
template <class Type>
template <class TrackingData>
inline bool wallPointData<Type>::updateCell
(
    const polyMesh& mesh,
    const label thisCellI,
    const label,
    const wallPointData<Type>& neighbourWallInfo,
    const scalar tol,
    TrackingData& td
)
{
    const vectorField& cellCentres = mesh.primitiveMesh::cellCentres();

    return update
    (
        cellCentres[thisCellI],
        neighbourWallInfo,
        tol,
        td
    );
}


// Update this with w2 if w2 nearer to pt.
template <class Type>
template <class TrackingData>
inline bool wallPointData<Type>::updateFace
(
    const polyMesh& mesh,
    const label thisFaceI,
    const label,
    const wallPointData<Type>& neighbourWallInfo,
    const scalar tol,
    TrackingData& td
)
{
    const vectorField& faceCentres = mesh.faceCentres();

    return update
    (
        faceCentres[thisFaceI],
        neighbourWallInfo,
        tol,
        td
    );
}


// Update this with w2 if w2 nearer to pt.
template <class Type>
template <class TrackingData>
inline bool wallPointData<Type>::updateFace
(
    const polyMesh& mesh,
    const label thisFaceI,
    const wallPointData<Type>& neighbourWallInfo,
    const scalar tol,
    TrackingData& td
)
{
    const vectorField& faceCentres = mesh.faceCentres();

    return update
    (
        faceCentres[thisFaceI],
        neighbourWallInfo,
        tol,
        td
    );
}


// ************************************************************************* //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
