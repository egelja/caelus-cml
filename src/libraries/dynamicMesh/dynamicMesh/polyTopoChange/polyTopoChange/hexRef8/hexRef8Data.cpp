/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "IOobject.hpp"
#include "UList.hpp"

#include "hexRef8Data.hpp"
#include "mapPolyMesh.hpp"
#include "mapDistributePolyMesh.hpp"
#include "polyMesh.hpp"
#include "syncTools.hpp"
#include "refinementHistory.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::hexRef8Data::hexRef8Data(const IOobject& io)
{
    {
        IOobject rio(io);
        rio.rename("cellLevel");
        bool haveFile = returnReduce
        (
            rio.headerOk(),
            orOp<bool>()
        );
        if (haveFile)
        {
            Info<< "Reading hexRef8 data : " << rio.name() << endl;
            cellLevelPtr_.reset(new labelIOList(rio));
        }
    }
    {
        IOobject rio(io);
        rio.rename("pointLevel");
        bool haveFile = returnReduce
        (
            rio.headerOk(),
            orOp<bool>()
        );
        if (haveFile)
        {
            Info<< "Reading hexRef8 data : " << rio.name() << endl;
            pointLevelPtr_.reset(new labelIOList(rio));
        }
    }
    {
        IOobject rio(io);
        rio.rename("level0Edge");
        bool haveFile = returnReduce
        (
            rio.headerOk(),
            orOp<bool>()
        );
        if (haveFile)
        {
            Info<< "Reading hexRef8 data : " << rio.name() << endl;
            level0EdgePtr_.reset(new uniformDimensionedScalarField(rio));
        }
    }
    {
        IOobject rio(io);
        rio.rename("refinementHistory");
        bool haveFile = returnReduce
        (
            rio.headerOk(),
            orOp<bool>()
        );
        if (haveFile)
        {
            Info<< "Reading hexRef8 data : " << rio.name() << endl;
            refHistoryPtr_.reset(new refinementHistory(rio));
        }
    }
}


CML::hexRef8Data::hexRef8Data
(
    const IOobject& io,
    const hexRef8Data& data,
    const labelList& cellMap,
    const labelList& pointMap
)
{
    if (data.cellLevelPtr_.valid())
    {
        IOobject rio(io);
        rio.rename(data.cellLevelPtr_().name());

        cellLevelPtr_.reset
        (
            new labelIOList
            (
                rio,
                UIndirectList<label>(data.cellLevelPtr_(), cellMap)()
            )
        );
    }
    if (data.pointLevelPtr_.valid())
    {
        IOobject rio(io);
        rio.rename(data.pointLevelPtr_().name());

        pointLevelPtr_.reset
        (
            new labelIOList
            (
                rio,
                UIndirectList<label>(data.pointLevelPtr_(), pointMap)()
            )
        );
    }
    if (data.level0EdgePtr_.valid())
    {
        IOobject rio(io);
        rio.rename(data.level0EdgePtr_().name());

        level0EdgePtr_.reset
        (
            new uniformDimensionedScalarField(rio, data.level0EdgePtr_())
        );
    }
    if (data.refHistoryPtr_.valid())
    {
        IOobject rio(io);
        rio.rename(data.refHistoryPtr_().name());

        refHistoryPtr_ = data.refHistoryPtr_().clone(rio, cellMap);
    }
}


CML::hexRef8Data::hexRef8Data
(
    const IOobject& io,
    const UPtrList<const labelList>& cellMaps,
    const UPtrList<const labelList>& pointMaps,
    const UPtrList<const hexRef8Data>& procDatas
)
{
    const polyMesh& mesh = dynamic_cast<const polyMesh&>(io.db());

    // cellLevel

    if (procDatas[0].cellLevelPtr_.valid())
    {
        IOobject rio(io);
        rio.rename(procDatas[0].cellLevelPtr_().name());

        cellLevelPtr_.reset(new labelIOList(rio, mesh.nCells()));
        labelList& cellLevel = cellLevelPtr_();

        forAll(procDatas, procI)
        {
            const labelList& procCellLevel = procDatas[procI].cellLevelPtr_();
            UIndirectList<label>(cellLevel, cellMaps[procI]) = procCellLevel;
        }
    }


    // pointLevel

    if (procDatas[0].pointLevelPtr_.valid())
    {
        IOobject rio(io);
        rio.rename(procDatas[0].pointLevelPtr_().name());

        pointLevelPtr_.reset(new labelIOList(rio, mesh.nPoints()));
        labelList& pointLevel = pointLevelPtr_();

        forAll(procDatas, procI)
        {
            const labelList& procPointLevel = procDatas[procI].pointLevelPtr_();
            UIndirectList<label>(pointLevel, pointMaps[procI]) = procPointLevel;
        }
    }


    // level0Edge

    if (procDatas[0].level0EdgePtr_.valid())
    {
        IOobject rio(io);
        rio.rename(procDatas[0].level0EdgePtr_().name());

        level0EdgePtr_.reset
        (
            new uniformDimensionedScalarField
            (
                rio,
                procDatas[0].level0EdgePtr_()
            )
        );
    }


    // refinementHistory

    if (procDatas[0].refHistoryPtr_.valid())
    {
        IOobject rio(io);
        rio.rename(procDatas[0].refHistoryPtr_().name());

        UPtrList<const refinementHistory> procRefs(procDatas.size());
        forAll(procDatas, i)
        {
            procRefs.set(i, &procDatas[i].refHistoryPtr_());
        }

        refHistoryPtr_.reset
        (
            new refinementHistory
            (
                rio,
                cellMaps,
                procRefs
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::hexRef8Data::~hexRef8Data()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::hexRef8Data::sync(const IOobject& io)
{
    const polyMesh& mesh = dynamic_cast<const polyMesh&>(io.db());

    bool hasCellLevel = returnReduce(cellLevelPtr_.valid(), orOp<bool>());
    if (hasCellLevel && !cellLevelPtr_.valid())
    {
        IOobject rio(io);
        rio.rename("cellLevel");
        rio.readOpt() = IOobject::NO_READ;
        cellLevelPtr_.reset(new labelIOList(rio, labelList(mesh.nCells(), 0)));
    }

    bool hasPointLevel = returnReduce(pointLevelPtr_.valid(), orOp<bool>());
    if (hasPointLevel && !pointLevelPtr_.valid())
    {
        IOobject rio(io);
        rio.rename("pointLevel");
        rio.readOpt() = IOobject::NO_READ;
        pointLevelPtr_.reset
        (
            new labelIOList(rio, labelList(mesh.nPoints(), 0))
        );
    }

    bool hasLevel0Edge = returnReduce(level0EdgePtr_.valid(), orOp<bool>());
    if (hasLevel0Edge)
    {
        // Get master length
        scalar masterLen = (Pstream::master() ? level0EdgePtr_().value() : 0);
        Pstream::scatter(masterLen);
        if (!level0EdgePtr_.valid())
        {
            IOobject rio(io);
            rio.rename("level0Edge");
            rio.readOpt() = IOobject::NO_READ;
            level0EdgePtr_.reset
            (
                new uniformDimensionedScalarField
                (
                    rio,
                    dimensionedScalar("zero", dimLength, masterLen)
                )
            );
        }
    }

    bool hasHistory = returnReduce(refHistoryPtr_.valid(), orOp<bool>());
    if (hasHistory && !refHistoryPtr_.valid())
    {
        IOobject rio(io);
        rio.rename("refinementHistory");
        rio.readOpt() = IOobject::NO_READ;
        refHistoryPtr_.reset(new refinementHistory(rio, mesh.nCells(), true));
    }
}


void CML::hexRef8Data::distribute(const mapDistributePolyMesh& map)
{
    if (cellLevelPtr_.valid())
    {
        map.cellMap().distribute(cellLevelPtr_());
    }
    if (pointLevelPtr_.valid())
    {
        map.pointMap().distribute(pointLevelPtr_());
    }

    // No need to distribute the level0Edge

    if (refHistoryPtr_.valid() && refHistoryPtr_().active())
    {
        refHistoryPtr_().distribute(map);
    }
}


bool CML::hexRef8Data::write() const
{
    bool ok = true;
    if (cellLevelPtr_.valid())
    {
        ok = ok && cellLevelPtr_().write();
    }
    if (pointLevelPtr_.valid())
    {
        ok = ok && pointLevelPtr_().write();
    }
    if (level0EdgePtr_.valid())
    {
        ok = ok && level0EdgePtr_().write();
    }
    if (refHistoryPtr_.valid())
    {
        ok = ok && refHistoryPtr_().write();
    }
    return ok;
}


// ************************************************************************* //
