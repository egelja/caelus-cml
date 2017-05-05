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

#include "cloudInfo.hpp"
#include "dictionary.hpp"
#include "kinematicCloud.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
defineTypeNameAndDebug(cloudInfo, 0);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void CML::cloudInfo::writeFileHeader(const label i)
{
    writeHeader(file(), "Cloud information");
    writeCommented(file(), "Time");
    writeTabbed(file(), "nParcels");
    writeTabbed(file(), "mass");
    file() << endl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::cloudInfo::cloudInfo
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    functionObjectFile(obr, name),
    name_(name),
    obr_(obr),
    active_(true)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::cloudInfo::~cloudInfo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::cloudInfo::read(const dictionary& dict)
{
    if (active_)
    {
        functionObjectFile::resetNames(dict.lookup("clouds"));

        Info<< type() << " " << name_ << ": ";
        if (names().size())
        {
            Info<< "applying to clouds:" << nl;
            forAllConstIter(wordHashSet, names(), iter)
            {
                Info<< "    " << iter.key() << nl;
            }
            Info<< endl;
        }
        else
        {
            Info<< "no clouds to be processed" << nl << endl;
        }
    }
}


void CML::cloudInfo::execute()
{
    // Do nothing
}


void CML::cloudInfo::end()
{
    // Do nothing
}


void CML::cloudInfo::timeSet()
{
    // Do nothing
}


void CML::cloudInfo::write()
{
    if (active_)
    {
        functionObjectFile::write();

        label i = 0;
        forAllConstIter(wordHashSet, names(), iter)
        {
            const word& cloudName = iter.key();

            const kinematicCloud& cloud =
                obr_.lookupObject<kinematicCloud>(cloudName);

            label nParcels = returnReduce(cloud.nParcels(), sumOp<label>());
            scalar massInSystem =
                returnReduce(cloud.massInSystem(), sumOp<scalar>());

            if (Pstream::master())
            {
                file(i)
                    << obr_.time().value() << token::TAB
                    << nParcels << token::TAB
                    << massInSystem << endl;
            }

            i++;
        }
    }
}


// ************************************************************************* //
