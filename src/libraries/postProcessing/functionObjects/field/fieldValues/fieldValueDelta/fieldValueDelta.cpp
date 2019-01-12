/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2014 OpenFOAM Foundation
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

#include "fieldValueDelta.hpp"
#include "ListOps.hpp"
#include "Time.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace fieldValues
    {
        defineTypeNameAndDebug(fieldValueDelta, 0);
    }

    template<>
    const char*
    NamedEnum<fieldValues::fieldValueDelta::operationType, 5>::names[] =
    {
        "add",
        "subtract",
        "min",
        "max",
        "average"
    };

    const NamedEnum<fieldValues::fieldValueDelta::operationType, 5>
        fieldValues::fieldValueDelta::operationTypeNames_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fieldValues::fieldValueDelta::fieldValueDelta
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    functionObjectFile(obr, name, typeName),
    name_(name),
    obr_(obr),
    loadFromFiles_(loadFromFiles),
    log_(true),
    operation_(opSubtract),
    source1Ptr_(nullptr),
    source2Ptr_(nullptr)
{
    read(dict);
}


void CML::fieldValues::fieldValueDelta::writeFileHeader(const label i)
{
    const wordList& fields1 = source1Ptr_->fields();
    const wordList& fields2 = source2Ptr_->fields();

    DynamicList<word> commonFields(fields1.size());
    forAll(fields1, i)
    {
        label index = findIndex(fields2, fields1[i]);
        if (index != -1)
        {
            commonFields.append(fields1[i]);
        }
    }

    Ostream& os = file();

    writeHeaderValue(os, "Source1", source1Ptr_->name());
    writeHeaderValue(os, "Source2", source2Ptr_->name());
    writeHeaderValue(os, "Operation", operationTypeNames_[operation_]);
    writeCommented(os, "Time");

    forAll(commonFields, i)
    {
        os  << tab << commonFields[i];
    }

    os  << endl;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fieldValues::fieldValueDelta::~fieldValueDelta()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fieldValues::fieldValueDelta::read(const dictionary& dict)
{
    log_ = dict.lookupOrDefault<Switch>("log", true);
    source1Ptr_.reset
    (
        fieldValue::New
        (
            name_ + ".source1",
            obr_,
            dict.subDict("source1"),
            loadFromFiles_,
            false
        ).ptr()
    );
    source2Ptr_.reset
    (
        fieldValue::New
        (
            name_ + ".source2",
            obr_,
            dict.subDict("source2"),
            loadFromFiles_,
            false
        ).ptr()
    );

    operation_ = operationTypeNames_.read(dict.lookup("operation"));
}


void CML::fieldValues::fieldValueDelta::write()
{
    functionObjectFile::write();

    source1Ptr_->write();
    source2Ptr_->write();

    if (Pstream::master())
    {
        file()<< obr_.time().value();
    }

    Info(log_)<< type() << " " << name_ << " output:" << endl;

    bool found = false;
    processFields<scalar>(found);
    processFields<vector>(found);
    processFields<sphericalTensor>(found);
    processFields<symmTensor>(found);
    processFields<tensor>(found);

    if (Pstream::master())
    {
        file()<< endl;
    }

    if (log_)
    {
        if (!found)
        {
            Info<< "    none" << endl;
        }
        else
        {
            Info<< endl;
        }
    }
}


void CML::fieldValues::fieldValueDelta::execute()
{
    // Do nothing
}


void CML::fieldValues::fieldValueDelta::end()
{
    // Do nothing
}


void CML::fieldValues::fieldValueDelta::timeSet()
{
    // Do nothing
}


void CML::fieldValues::fieldValueDelta::updateMesh(const mapPolyMesh&)
{
    // Do nothing
}


void CML::fieldValues::fieldValueDelta::movePoints(const pointField&)
{
    // Do nothing
}


// ************************************************************************* //
