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

#include "boundaryRegion.hpp"
#include "IOMap.hpp"
#include "OFstream.hpp"
#include "stringListOps.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::boundaryRegion::boundaryRegion()
:
    Map<dictionary>()
{}


CML::boundaryRegion::boundaryRegion
(
    const objectRegistry& registry,
    const word& name,
    const fileName& instance
)
:
    Map<dictionary>()
{
    readDict(registry, name, instance);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::boundaryRegion::~boundaryRegion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::label CML::boundaryRegion::append(const dictionary& dict)
{
    label maxId = -1;
    forAllConstIter(Map<dictionary>, *this, iter)
    {
        if (maxId < iter.key())
        {
            maxId = iter.key();
        }
    }

    insert(++maxId, dict);
    return maxId;
}


CML::Map<CML::word> CML::boundaryRegion::names() const
{
    Map<word> lookup;

    forAllConstIter(Map<dictionary>, *this, iter)
    {
        lookup.insert
        (
            iter.key(),
            iter().lookupOrDefault<word>
            (
                "Label",
                "boundaryRegion_" + CML::name(iter.key())
            )
        );
    }

    return lookup;
}


CML::Map<CML::word> CML::boundaryRegion::names
(
    const UList<wordRe>& patterns
) const
{
    Map<word> lookup;

    forAllConstIter(Map<dictionary>, *this, iter)
    {
        word lookupName = iter().lookupOrDefault<word>
        (
            "Label",
            "boundaryRegion_" + CML::name(iter.key())
        );

        if (findStrings(patterns, lookupName))
        {
            lookup.insert(iter.key(), lookupName);
        }
    }

    return lookup;
}


CML::Map<CML::word> CML::boundaryRegion::boundaryTypes() const
{
    Map<word> lookup;

    forAllConstIter(Map<dictionary>, *this, iter)
    {
        lookup.insert
        (
            iter.key(),
            iter().lookupOrDefault<word>("BoundaryType", "patch")
        );
    }

    return lookup;
}


CML::label CML::boundaryRegion::findIndex(const word& name) const
{
    if (name.empty())
    {
        return -1;
    }

    forAllConstIter(Map<dictionary>, *this, iter)
    {
        if (iter().lookupOrDefault<word>("Label", word::null) == name)
        {
            return iter.key();
        }
    }

    return -1;
}


CML::word CML::boundaryRegion::boundaryType(const word& name) const
{
    word bndType("patch");

    label id = this->findIndex(name);
    if (id >= 0)
    {
        operator[](id).readIfPresent<word>("BoundaryType", bndType);
    }

    return bndType;
}


void CML::boundaryRegion::readDict
(
    const objectRegistry& registry,
    const word& name,
    const fileName& instance
)
{
    clear();

    // read constant/dictName
    IOMap<dictionary> ioObj
    (
        IOobject
        (
            name,
            instance,
            registry,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE,
            false
        )
    );

    if (ioObj.headerOk())
    {
        *this = ioObj;
    }
    else
    {
        Info<< "no constant/boundaryRegion information available" << endl;
    }
}


void CML::boundaryRegion::writeDict
(
    const objectRegistry& registry,
    const word& name,
    const fileName& instance
) const
{
    // write constant/dictName
    IOMap<dictionary> ioObj
    (
        IOobject
        (
            name,
            instance,
            registry,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        )
    );

    ioObj.note() =
        "persistent data for thirdParty mesh <-> CAELUS translation";

    Info<< "Writing " << ioObj.name() << " to " << ioObj.objectPath() << endl;

    OFstream os(ioObj.objectPath());
    ioObj.writeHeader(os);
    os << *this;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::boundaryRegion::operator=(const boundaryRegion& rhs)
{
    Map<dictionary>::operator=(rhs);
}


void CML::boundaryRegion::operator=(const Map<dictionary>& rhs)
{
    Map<dictionary>::operator=(rhs);
}


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

void CML::boundaryRegion::rename(const dictionary& mapDict)
{
    if (mapDict.empty())
    {
        return;
    }

    // Use 1st pass to collect all the regions to be changed
    // and 2nd pass to relabel regions.
    // This avoid re-matching any renamed regions

    Map<word> mapping;
    forAllConstIter(dictionary, mapDict, iter)
    {
        word oldName(iter().stream());

        label id = this->findIndex(oldName);
        if (id >= 0)
        {
            mapping.insert(id, iter().keyword());
        }
    }

    forAllConstIter(Map<word>, mapping, iter)
    {
        dictionary& dict = operator[](iter.key());

        Info<< "rename patch: " << iter()
            << " <- " << word(dict.lookup("Label")) << nl;

        dict.set("Label", iter());
    }
}


// ************************************************************************* //
