/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "timeActivatedFileUpdate.hpp"
#include "objectRegistry.hpp"
#include "Time.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(timeActivatedFileUpdate, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::timeActivatedFileUpdate::updateFile()
{
    label i = lastIndex_;
    while
    (
        i < timeVsFile_.size()-1
     && timeVsFile_[i+1].first() < obr_.time().value()
    )
    {
        i++;
    }

    if (i > lastIndex_)
    {
        Info<< nl << type() << ": copying file" << nl << timeVsFile_[i].second()
            << nl << "to:" << nl << fileToUpdate_ << nl << endl;

        cp(timeVsFile_[i].second(), fileToUpdate_);
        lastIndex_ = i;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::timeActivatedFileUpdate::timeActivatedFileUpdate
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    name_(name),
    obr_(obr),
    active_(true),
    fileToUpdate_(dict.lookup("fileToUpdate")),
    timeVsFile_(),
    lastIndex_(-1)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::timeActivatedFileUpdate::~timeActivatedFileUpdate()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::timeActivatedFileUpdate::read(const dictionary& dict)
{
    if (active_)
    {
        dict.lookup("fileToUpdate") >> fileToUpdate_;
        dict.lookup("timeVsFile") >> timeVsFile_;

        lastIndex_ = -1;
        fileToUpdate_.expand();

        Info<< type() << ": time vs file list:" << nl;
        forAll(timeVsFile_, i)
        {
            timeVsFile_[i].second() = timeVsFile_[i].second().expand();
            if (!isFile(timeVsFile_[i].second()))
            {
                FatalErrorInFunction
                    << "File: " << timeVsFile_[i].second() << " not found"
                    << nl << exit(FatalError);
            }

            Info<< "    " << timeVsFile_[i].first() << tab
                << timeVsFile_[i].second() << endl;
        }
        Info<< endl;

        updateFile();
    }
}


void CML::timeActivatedFileUpdate::execute()
{
    if (active_)
    {
        updateFile();
    }
}


void CML::timeActivatedFileUpdate::end()
{
    if (active_)
    {
        execute();
    }
}


void CML::timeActivatedFileUpdate::timeSet()
{
    // Do nothing
}


void CML::timeActivatedFileUpdate::write()
{
    // Do nothing
}


// ************************************************************************* //
