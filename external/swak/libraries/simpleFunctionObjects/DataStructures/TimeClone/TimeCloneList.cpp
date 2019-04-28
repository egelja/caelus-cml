/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
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

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "TimeCloneList.hpp"
#include "DebugOStream.hpp"

namespace CML {
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(TimeCloneList, 0);

label TimeCloneList::count_=0;

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

TimeCloneList::TimeCloneList(const dictionary &dict)
{
    Dbug << "Construction" << endl;
    label nrSteps=readLabel(dict.lookup("numberOfTimestepsToStore"));
    if(nrSteps<1) {
        FatalErrorInFunction
            << "Number of timesteps must be bigger than 0 in " << dict.name()
                << endl
                << exit(FatalError);
    }
    storedTimes_.resize(nrSteps,nullptr);
    if(count_>0) {
        bool ok=dict.lookupOrDefault<bool>(
            "moreThanOneInstanceOfTimeCloneListIsOK",
            false
        );
        if(!ok) {
            FatalErrorInFunction
                << "There are already " << count_ << " other instances of "
                    << "TimeCloneList. " << nl
                    << "As this data structure potentially uses a lot of "
                    << "memory you must confirm with the option "
                    << "'moreThanOneInstanceOfTimeCloneListIsOK' in "
                    << dict.name() << " that you want one more instance"
                    << endl
                    << exit(FatalError);
        }
    }
    count_++;
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

TimeCloneList::~TimeCloneList()
{
    Dbug << "Destruction" << endl;
    clear();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void TimeCloneList::clear()
{
    Dbug << "clear" << endl;
    forAll(storedTimes_,i) {
        if(storedTimes_[i]!=nullptr) {
            Dbug << "Removing entry " << i << endl;
            delete storedTimes_[i];
            storedTimes_[i]=nullptr;
        }
    }
}

void TimeCloneList::copy(const Time &t)
{
    Dbug << "copy: t=" << t.timeName() << endl;
    const label last=storedTimes_.size()-1;
    if(storedTimes_[last]!=0) {
        Dbug << "Removing last entry" << endl;
        delete storedTimes_[last];
        storedTimes_[last]=nullptr;
    }
    Dbug << "Shifting entries" << endl;
    for(label i=last;i>0;i--) {
        storedTimes_[i]=storedTimes_[i-1];
    }
    Dbug << "Adding new entry" << endl;
    storedTimes_[0]=new TimeClone();

    Dbug << "Actual copying" << endl;
    return storedTimes_[0]->copy(t);
}

bool TimeCloneList::write(const bool force)
{
    Dbug << "write. Force: " << force << endl;

    forAll(storedTimes_,i) {
        if(storedTimes_[i]!=nullptr) {
            Dbug << "Writing entry " << i << endl;
            storedTimes_[i]->write(force);
        }
    }

    Dbug << "Clearing entries" << endl;
    clear();

    return true;
}

// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * Friend Operators * * * * * * * * * * * * * * //

} // end namespace

// ************************************************************************* //
