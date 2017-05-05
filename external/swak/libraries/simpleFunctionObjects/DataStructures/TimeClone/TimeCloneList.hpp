/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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

Class
    TimeCloneList

Description
    List of N time-clones. The oldest is always removed. After writing all are removed

    Uses a list of Ptr because PtrList does not allow the direct manipulation of the pointer values
SourceFiles
    TimeCloneList.cpp

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef TimeCloneList_H
#define TimeCloneList_H

#include "TimeClone.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class TimeCloneList Declaration
\*---------------------------------------------------------------------------*/

class TimeCloneList
{
    // Private data

    //- the actual time-thing
    List<TimeClone*> storedTimes_;


    // Private Member Functions

    //- Disallow default bitwise copy construct
    TimeCloneList(const TimeCloneList&);

    //- Disallow default bitwise assignment
    void operator=(const TimeCloneList&);

    void clear();

    static label count_;

public:

    // Static data members

    TypeName("TimeCloneList");

    // Constructors

    //- Construct null
    TimeCloneList(const dictionary &dict);

    // Selectors

    //- Make a copy
    void copy(const Time &);

    //- Write the time-directory
    bool write(const bool force=false);

    //- Destructor
    virtual ~TimeCloneList();


    // Member Functions

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
