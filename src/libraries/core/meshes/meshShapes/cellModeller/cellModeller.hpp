/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::cellModeller

Description
    A static collection of cell models, and a means of looking them up.

SourceFiles
    cellModeller.cpp
    cellModellerIO.cpp
    globalCellModeller.cpp

\*---------------------------------------------------------------------------*/

#ifndef cellModeller_H
#define cellModeller_H

#include "cellModel.hpp"
#include "PtrList.hpp"
#include "HashTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class cellModeller Declaration
\*---------------------------------------------------------------------------*/

class cellModeller
{
    // Private data

        //- PtrList of models
        static PtrList<cellModel> models_;

        //- List of model pointers
        static List<cellModel*> modelPtrs_;

        //- HashTable of model pointers
        static HashTable<const cellModel*> modelDictionary_;


public:

    // Constructors

        //- Construct from central "cellModels" file
        cellModeller();

    //- Destructor
    ~cellModeller();


    // Member functions

        //- Look up a model by name and return a pointer to the model or nullptr
        static const cellModel* lookup(const word&);

        //- Look up a model by index and return a pointer to the model or nullptr
        static const cellModel* lookup(const label i)
        {
            return modelPtrs_[i];
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
