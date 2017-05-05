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

Class
    CML::lduScheduleEntry

Description
    Struct to hold the patch index and the initialisation flag for
    the patch schedule

\*---------------------------------------------------------------------------*/

#ifndef lduSchedule_H
#define lduSchedule_H

#include "List.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

struct lduScheduleEntry;
Ostream& operator<<(Ostream& os, const lduScheduleEntry& lb);


/*---------------------------------------------------------------------------*\
                      Class lduSchedule Declaration
\*---------------------------------------------------------------------------*/

struct lduScheduleEntry
{
    label patch;
    bool init;

    friend bool operator!=
    (
        const lduScheduleEntry& lb1,
        const lduScheduleEntry& lb2
    )
    {
        return true;
    }

    friend Ostream& operator<<(Ostream& os, const lduScheduleEntry& lb)
    {
        os  << lb.patch << token::SPACE << lb.init;
        return os;
    }
};


typedef List<lduScheduleEntry> lduSchedule;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
