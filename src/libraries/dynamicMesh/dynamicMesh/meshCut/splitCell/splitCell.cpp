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

#include "splitCell.hpp"
#include "error.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from cell number and parent
CML::splitCell::splitCell(const label cellI, splitCell* parent)
:
    cellI_(cellI),
    parent_(parent),
    master_(nullptr),
    slave_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::splitCell::~splitCell()
{
    splitCell* myParent = parent();

    if (myParent)
    {
        // Make sure parent does not refer to me anymore.
        if (myParent->master() == this)
        {
            myParent->master() = nullptr;
        }
        else if (myParent->slave() == this)
        {
            myParent->slave() = nullptr;
        }
        else
        {
            FatalErrorInFunction << "this not equal to"
                << " parent's master or slave pointer" << endl
                << "Cell:" << cellLabel() << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::splitCell::isMaster() const
{
    splitCell* myParent = parent();

    if (!myParent)
    {
        FatalErrorInFunction << "parent not set"
            << "Cell:" << cellLabel() << abort(FatalError);

        return false;
    }
    else if (myParent->master() == this)
    {
        return true;
    }
    else if (myParent->slave() == this)
    {
        return false;
    }
    else
    {
        FatalErrorInFunction << "this not equal to"
            << " parent's master or slave pointer" << endl
            << "Cell:" << cellLabel() << abort(FatalError);

        return false;
    }
}


bool CML::splitCell::isUnrefined() const
{
    return !master() && !slave();
}


CML::splitCell* CML::splitCell::getOther() const
{
    splitCell* myParent = parent();

    if (!myParent)
    {
        FatalErrorInFunction << "parent not set"
            << "Cell:" << cellLabel() << abort(FatalError);

        return nullptr;
    }
    else if (myParent->master() == this)
    {
        return myParent->slave();
    }
    else if (myParent->slave() == this)
    {
        return myParent->master();
    }
    else
    {
        FatalErrorInFunction << "this not equal to"
            << " parent's master or slave pointer" << endl
            << "Cell:" << cellLabel() << abort(FatalError);

        return nullptr;
    }
}


// ************************************************************************* //
