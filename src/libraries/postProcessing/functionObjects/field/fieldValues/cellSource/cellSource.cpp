/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "cellSource.hpp"
#include "fvMesh.hpp"
#include "volFields.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    template<>
    const char* NamedEnum<fieldValues::cellSource::sourceType, 2>::names[] =
    {
        "cellZone",
        "all"
    };


    template<>
    const char* NamedEnum<fieldValues::cellSource::operationType, 10>::names[] =
    {
        "none",
        "sum",
        "sumMag",
        "average",
        "weightedAverage",
        "volAverage",
        "volIntegrate",
        "min",
        "max",
        "CoV"
    };

    namespace fieldValues
    {
        defineTypeNameAndDebug(cellSource, 0);
        addToRunTimeSelectionTable(fieldValue, cellSource, dictionary);
    }
}


const CML::NamedEnum<CML::fieldValues::cellSource::sourceType, 2>
    CML::fieldValues::cellSource::sourceTypeNames_;

const CML::NamedEnum<CML::fieldValues::cellSource::operationType, 10>
    CML::fieldValues::cellSource::operationTypeNames_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::fieldValues::cellSource::setCellZoneCells()
{
    switch (source_)
    {
        case stCellZone:
        {
            dict().lookup("sourceName") >> sourceName_;

            label zoneId = mesh().cellZones().findZoneID(sourceName_);

            if (zoneId < 0)
            {
                FatalErrorInFunction
                    << "Unknown cell zone name: " << sourceName_
                    << ". Valid cell zones are: " << mesh().cellZones().names()
                    << nl << exit(FatalError);
            }

            cellId_ = mesh().cellZones()[zoneId];
            nCells_ = returnReduce(cellId_.size(), sumOp<label>());
            break;
        }

        case stAll:
        {
            cellId_ = identity(mesh().nCells());
            nCells_ = returnReduce(cellId_.size(), sumOp<label>());
            break;
        }

        default:
        {
            FatalErrorInFunction
               << "Unknown source type. Valid source types are:"
                << sourceTypeNames_ << nl << exit(FatalError);
        }
    }

    if (debug)
    {
        Pout<< "Selected source size = " << cellId_.size() << endl;
    }
}


CML::scalar CML::fieldValues::cellSource::volume() const
{
    return gSum(filterField(mesh().V()));
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void CML::fieldValues::cellSource::initialise(const dictionary& dict)
{
    setCellZoneCells();

    if (nCells_ == 0)
    {
        WarningInFunction
            << type() << " " << name_ << ": "
            << sourceTypeNames_[source_] << "(" << sourceName_ << "):" << nl
            << "    Source has no cells - deactivating" << endl;

        active_ = false;
        return;
    }

    volume_ = volume();

    Info<< type() << " " << name_ << ":"
        << sourceTypeNames_[source_] << "(" << sourceName_ << "):" << nl
        << "    total cells  = " << nCells_ << nl
        << "    total volume = " << volume_
        << nl << endl;

    if (dict.readIfPresent("weightField", weightFieldName_))
    {
        Info<< "    weight field = " << weightFieldName_;
    }

    Info<< nl << endl;
}


void CML::fieldValues::cellSource::writeFileHeader(const label i)
{
    writeCommented(file(), "Source : ");
    file() << sourceTypeNames_[source_] << " " << sourceName_ << endl;
    writeCommented(file(), "Cells  : ");
    file() << nCells_ << endl;
    writeCommented(file(), "Volume : ");
    file() << volume_ << endl;

    writeCommented(file(), "Time");
    if (writeVolume_)
    {
        file() << tab << "Volume";
    }

    forAll(fields_, i)
    {
        file()
            << tab << operationTypeNames_[operation_]
            << "(" << fields_[i] << ")";
    }

    file() << endl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fieldValues::cellSource::cellSource
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    fieldValue(name, obr, dict, typeName, loadFromFiles),
    source_(sourceTypeNames_.read(dict.lookup("source"))),
    operation_(operationTypeNames_.read(dict.lookup("operation"))),
    nCells_(0),
    cellId_(),
    weightFieldName_("none"),
    writeVolume_(dict.lookupOrDefault("writeVolume", false))
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fieldValues::cellSource::~cellSource()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fieldValues::cellSource::read(const dictionary& dict)
{
    fieldValue::read(dict);

    if (active_)
    {
        // no additional info to read
        initialise(dict);
    }
}


void CML::fieldValues::cellSource::write()
{
    fieldValue::write();

    if (active_)
    {
        if (Pstream::master())
        {
            file() << obr_.time().value();
        }

        if (writeVolume_)
        {
            volume_ = volume();
            if (Pstream::master())
            {
                file() << tab << volume_;
            }
            Info(log_)<< "    total volume = " << volume_ << endl;
        }

        forAll(fields_, i)
        {
            const word& fieldName = fields_[i];
            bool processed = false;

            processed = processed || writeValues<scalar>(fieldName);
            processed = processed || writeValues<vector>(fieldName);
            processed = processed || writeValues<sphericalTensor>(fieldName);
            processed = processed || writeValues<symmTensor>(fieldName);
            processed = processed || writeValues<tensor>(fieldName);

            if (!processed)
            {
                WarningInFunction
                    << "Requested field " << fieldName
                    << " not found in database and not processed"
                    << endl;
            }
        }

        if (Pstream::master())
        {
            file()<< endl;
        }

        Info(log_)<< endl;
    }
}


// ************************************************************************* //
