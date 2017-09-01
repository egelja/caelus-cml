/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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
    CML::removeSubDirs

Description
    Remove the sub-directories in a given directory under the
    postProcessesWaves2Foam directory tree.

SourceFiles
    removeSubDirs.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef removeSubDirs_HPP
#define removeSubDirs_HPP

#include "postProcessingWaves.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class removeSubDirs
:
    public postProcessingWaves
{
private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        removeSubDirs(const removeSubDirs&);

        //- Disallow default bitwise assignment
        void operator=(const removeSubDirs&);

    // Private member data

public:

    //- Runtime type information
    TypeName("removeSubDirs");

    // Constructors

        //- Construct from components
        removeSubDirs
        (
            const Time&,
            const dictionary&,
            const word&
        );

    // Destructor

        virtual ~removeSubDirs();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
