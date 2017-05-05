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

Class
    CML::profileModelList

Description
    Base class for profile models

SourceFiles
    profileModelList.cpp

\*---------------------------------------------------------------------------*/

#ifndef profileModelList_H
#define profileModelList_H

#include "PtrList.hpp"
#include "profileModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class profileModelList Declaration
\*---------------------------------------------------------------------------*/

class profileModelList
:
    public PtrList<profileModel>
{

protected:

    // Protected data

        //- Dictionary
        const dictionary dict_;


public:

    //- Constructor
    profileModelList(const dictionary& dict, const bool readFields = true);

    //- Destructor
    ~profileModelList();


    // Member Functions

        //- Set blade->profile addressing
        void connectBlades
        (
            const List<word>& names,
            List<label>& addr
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
