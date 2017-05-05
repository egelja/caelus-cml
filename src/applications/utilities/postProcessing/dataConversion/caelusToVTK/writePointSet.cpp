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

\*---------------------------------------------------------------------------*/

#include "writePointSet.hpp"
#include "OFstream.hpp"
#include "writeFuns.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

void writePointSet
(
    const bool binary,
    const vtkMesh& vMesh,
    const pointSet& set,
    const fileName& fileName
)
{
    std::ofstream ostr(fileName.c_str());

    writeFuns::writeHeader
    (
        ostr,
        binary,
        set.name()
    );

    ostr<< "DATASET POLYDATA" << std::endl;

    //------------------------------------------------------------------
    //
    // Write topology
    //
    //------------------------------------------------------------------


    // Write points

    ostr<< "POINTS " << set.size() << " float" << std::endl;

    DynamicList<floatScalar> ptField(3*set.size());

    writeFuns::insert
    (
        UIndirectList<point>(vMesh.mesh().points(), set.toc())(),
        ptField
    );

    writeFuns::write(ostr, binary, ptField);


    //-----------------------------------------------------------------
    //
    // Write data
    //
    //-----------------------------------------------------------------

    // Write faceID

    ostr
        << "POINT_DATA " << set.size() << std::endl
        << "FIELD attributes 1" << std::endl;

    // Cell ids first
    ostr<< "pointID 1 " << set.size() << " int" << std::endl;

    labelList pointIDs(set.toc());

    writeFuns::write(ostr, binary, pointIDs);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
