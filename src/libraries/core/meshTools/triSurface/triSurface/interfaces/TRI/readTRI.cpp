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

Description
    TRI (triangle) file reader. Comes out of e.g. AC3D.
    lines are 9 floats (3 points, each 3 floats) followed by hex colour.
    Is converted into regions: regions numbered from 0 up, each colour is
    region.
    Most of reading/stitching taken from STL reader.

\*---------------------------------------------------------------------------*/

#include "triSurface.hpp"
#include "STLpoint.hpp"
#include "SLList.hpp"
#include "IFstream.hpp"
#include "readHexLabel.hpp"
#include "stringList.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::triSurface::readTRI(const fileName& TRIfileName)
{
    IFstream TRIfile(TRIfileName);

    if (!TRIfile.good())
    {
        FatalErrorInFunction
            << "Cannot read file " << TRIfileName
            << exit(FatalError);
    }

    SLList<STLpoint> STLpoints;
    SLList<label> STLlabels;
    HashTable<label, string> STLsolidNames;

    // Max region number so far
    label maxRegion = 0;

    while (TRIfile)
    {
        string line = getLineNoComment(TRIfile);

        if (line.empty())
        {
            break;
        }

        IStringStream lineStream(line);

        STLpoint p
        (
            readScalar(lineStream),
            readScalar(lineStream),
            readScalar(lineStream)
        );

        if (!lineStream) break;

        STLpoints.append(p);
        STLpoints.append
        (
            STLpoint
            (
                readScalar(lineStream),
                readScalar(lineStream),
                readScalar(lineStream)
            )
        );
        STLpoints.append
        (
            STLpoint
            (
                readScalar(lineStream),
                readScalar(lineStream),
                readScalar(lineStream)
            )
        );

        // Region/colour in .tri file starts with 0x. Skip.

        char zero;
        lineStream >> zero;

        word rawSolidName(lineStream);

        word solidName("patch" + rawSolidName(1, rawSolidName.size()-1));

        label region  = -1;

        HashTable<label, string>::const_iterator fnd =
            STLsolidNames.find(solidName);

        if (fnd != STLsolidNames.end())
        {
            region = fnd();
        }
        else
        {
            Pout<< "Mapping triangle colour 0" << rawSolidName
                << " to region " << maxRegion << " name " << solidName
                << endl;

            region = maxRegion++;
            STLsolidNames.insert(solidName, region);
        }
        STLlabels.append(region);
    }


    pointField rawPoints(STLpoints.size());

    label pointI = 0;
    forAllConstIter(SLList<STLpoint>, STLpoints, iter)
    {
        rawPoints[pointI++] = *iter;
    }

    setSize(STLlabels.size());

    // Assign triangles
    pointI = 0;
    SLList<label>::const_iterator iter = STLlabels.begin();
    forAll(*this, i)
    {
        operator[](i)[0] = pointI++;
        operator[](i)[1] = pointI++;
        operator[](i)[2] = pointI++;
        operator[](i).region() = *iter;
        ++iter;
    }

    // Assign coordinates
    storedPoints().transfer(rawPoints);
    // Merge duplicate points
    stitchTriangles();

    // Convert solidNames into regionNames
    stringList names(STLsolidNames.toc());

    patches_.setSize(names.size());

    forAll(names, nameI)
    {
        patches_[nameI].name() = names[nameI];
        patches_[nameI].geometricType() = "empty";
    }

    return true;
}


// ************************************************************************* //
