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

#include "writeRegisteredObject.hpp"
#include "dictionary.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(writeRegisteredObject, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::writeRegisteredObject::writeRegisteredObject
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    name_(name),
    exclusiveWriting_(false),
    obr_(obr),
    objectNames_()
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::writeRegisteredObject::~writeRegisteredObject()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::writeRegisteredObject::read(const dictionary& dict)
{
    dict.lookup("objectNames") >> objectNames_;
    dict.readIfPresent("exclusiveWriting", exclusiveWriting_);
}


void CML::writeRegisteredObject::execute()
{
    // Do nothing - only valid on write
}


void CML::writeRegisteredObject::end()
{
    // Do nothing - only valid on write
}


void CML::writeRegisteredObject::timeSet()
{
    // Do nothing - only valid on write
}


void CML::writeRegisteredObject::write()
{
    Info<< type() << " " << name_ << " output:" << nl;

    DynamicList<word> allNames(obr_.toc().size());
    forAll(objectNames_, i)
    {
        wordList names(obr_.names<regIOobject>(objectNames_[i]));

        if (names.size())
        {
            allNames.append(names);
        }
        else
        {
            WarningInFunction
                << "Object " << objectNames_[i] << " not found in "
                << "database. Available objects:" << nl << obr_.sortedToc()
                << endl;
        }
    }

    forAll(allNames, i)
    {
        regIOobject& obj =
            const_cast<regIOobject&>
            (
                obr_.lookupObject<regIOobject>(allNames[i])
            );

        if (exclusiveWriting_)
        {
            // Switch off automatic writing to prevent double write
            obj.writeOpt() = IOobject::NO_WRITE;
        }

        Info<< "    writing object " << obj.name() << nl << endl;

        obj.write();
    }
}


// ************************************************************************* //
