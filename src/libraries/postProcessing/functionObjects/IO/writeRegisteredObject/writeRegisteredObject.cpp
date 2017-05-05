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

#include "writeRegisteredObject.hpp"
#include "dictionary.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::writeRegisteredObject, 0);


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
}


void CML::writeRegisteredObject::execute()
{
    // Do nothing - only valid on write
}


void CML::writeRegisteredObject::end()
{
    // Do nothing - only valid on write
}


void CML::writeRegisteredObject::write()
{
    forAll(objectNames_, i)
    {
        if (obr_.foundObject<regIOobject>(objectNames_[i]))
        {
            regIOobject& obj =
                const_cast<regIOobject&>
                (
                    obr_.lookupObject<regIOobject>(objectNames_[i])
                );
            // Switch off automatic writing to prevent double write
            obj.writeOpt() = IOobject::NO_WRITE;
            obj.write();
        }
        else
        {
            WarningIn
            (
                "CML::writeRegisteredObject::read(const dictionary&)"
            )   << "Object " << objectNames_[i] << " not found in "
                << "database. Available objects:" << nl << obr_.sortedToc()
                << endl;
        }

    }
}


// ************************************************************************* //
