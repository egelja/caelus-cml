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
    CML::pointPatchFieldMapperPatchRef

Description
    CML::pointPatchFieldMapperPatchRef

\*---------------------------------------------------------------------------*/

#ifndef pointPatchFieldMapperPatchRef_H
#define pointPatchFieldMapperPatchRef_H

#include "pointPatchFieldMapper.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class pointPatchFieldMapperPatchRef Declaration
\*---------------------------------------------------------------------------*/

class pointPatchFieldMapperPatchRef
:
    public pointPatchFieldMapper
{
    // Private data

        const pointPatch& sourcePatch_;
        const pointPatch& targetPatch_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        pointPatchFieldMapperPatchRef(const pointPatchFieldMapperPatchRef&);

        //- Disallow default bitwise assignment
        void operator=(const pointPatchFieldMapperPatchRef&);


public:

    // Constructors

        //- Construct from components
        pointPatchFieldMapperPatchRef
        (
            const pointPatch& source,
            const pointPatch& target
        )
        :
            sourcePatch_(source),
            targetPatch_(target)
        {}


    // Member functions

        const pointPatch& sourcePatch() const
        {
            return sourcePatch_;
        }

        const pointPatch& targetPatch() const
        {
            return targetPatch_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
