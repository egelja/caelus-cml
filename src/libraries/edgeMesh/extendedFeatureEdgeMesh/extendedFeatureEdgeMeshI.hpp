/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::extendedFeatureEdgeMesh::convexStart() const
{
    return convexStart_;
}


inline CML::label CML::extendedFeatureEdgeMesh::concaveStart() const
{
    return concaveStart_;
}


inline CML::label CML::extendedFeatureEdgeMesh::mixedStart() const
{
    return mixedStart_;
}


inline CML::label CML::extendedFeatureEdgeMesh::nonFeatureStart() const
{
    return nonFeatureStart_;
}


inline CML::label CML::extendedFeatureEdgeMesh::externalStart() const
{
    return externalStart_;
}


inline CML::label CML::extendedFeatureEdgeMesh::internalStart() const
{
    return internalStart_;
}


inline CML::label CML::extendedFeatureEdgeMesh::flatStart() const
{
    return flatStart_;
}


inline CML::label CML::extendedFeatureEdgeMesh::openStart() const
{
    return openStart_;
}


inline CML::label CML::extendedFeatureEdgeMesh::multipleStart() const
{
    return multipleStart_;
}


inline bool CML::extendedFeatureEdgeMesh::featurePoint(label ptI) const
{
    return ptI < nonFeatureStart_;
}


inline const CML::vectorField& CML::extendedFeatureEdgeMesh::normals() const
{
    return normals_;
}

inline const CML::vectorField& CML::extendedFeatureEdgeMesh::edgeDirections()
const
{
    return edgeDirections_;
}


inline CML::vector CML::extendedFeatureEdgeMesh::edgeDirection
(
    label edgeI,
    label ptI
) const
{
    const edge& e = edges()[edgeI];

    if (ptI == e.start())
    {
        return edgeDirections()[edgeI];
    }
    else if (ptI == e.end())
    {
        return -edgeDirections()[edgeI];
    }
    else
    {
        FatalErrorInFunction
            << "Requested ptI " << ptI << " is not a point on the requested "
            << "edgeI " << edgeI << ". edgeI start and end: "
            << e.start() << " " << e.end()
            << exit(FatalError);

        return vector::zero;
    }
}


inline const CML::labelListList& CML::extendedFeatureEdgeMesh::edgeNormals()
const
{
    return edgeNormals_;
}


inline CML::vectorField CML::extendedFeatureEdgeMesh::edgeNormals
(
    const labelList& edgeNormIs
) const
{
    vectorField norms(edgeNormIs.size());

    forAll(edgeNormIs, i)
    {
        norms[i] = normals_[edgeNormIs[i]];
    }

    return norms;
}


inline CML::vectorField CML::extendedFeatureEdgeMesh::edgeNormals(label edgeI)
const
{
    return edgeNormals(edgeNormals_[edgeI]);
}


inline const CML::labelListList&
CML::extendedFeatureEdgeMesh::featurePointNormals() const
{
    return featurePointNormals_;
}


inline CML::vectorField CML::extendedFeatureEdgeMesh::featurePointNormals
(
    label ptI
) const
{
    if (!featurePoint(ptI))
    {
        WarningInFunction
            << "Requesting the normals of a non-feature point. "
            << "Returned zero length vectorField."
            << endl;

        return vectorField(0);
    }

    labelList featPtNormIs(featurePointNormals_[ptI]);

    vectorField norms(featPtNormIs.size());

    forAll(featPtNormIs, i)
    {
        norms[i] = normals_[featPtNormIs[i]];
    }

    return norms;
}


inline const CML::labelList& CML::extendedFeatureEdgeMesh::regionEdges() const
{
    return regionEdges_;
}


inline CML::extendedFeatureEdgeMesh::pointStatus
CML::extendedFeatureEdgeMesh::getPointStatus
(
    label ptI
) const
{
    if (ptI < concaveStart_)
    {
        return CONVEX;
    }
    else if (ptI < mixedStart_)
    {
        return CONCAVE;
    }
    else if (ptI < nonFeatureStart_)
    {
        return MIXED;
    }
    else
    {
        return NONFEATURE;
    }
}


inline CML::extendedFeatureEdgeMesh::edgeStatus
CML::extendedFeatureEdgeMesh::getEdgeStatus
(
    label edgeI
) const
{
    if (edgeI < internalStart_)
    {
        return EXTERNAL;
    }
    else if (edgeI < flatStart_)
    {
        return INTERNAL;
    }
    else if (edgeI < openStart_)
    {
        return FLAT;
    }
    else if (edgeI < multipleStart_)
    {
        return OPEN;
    }
    else
    {
        return MULTIPLE;
    }
}


// ************************************************************************* //
