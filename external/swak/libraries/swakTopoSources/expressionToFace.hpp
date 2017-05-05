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
    CML::expressionToFace

Description
    A topoSetSource to select a faceSet from a logical expression on the cells

    Face is added if owner and neighbour cell have two different values

SourceFiles
    expressionToFace.cpp

Contributors/Copyright:
    2010-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef expressionToFace_H
#define expressionToFace_H

#include "topoSetSource.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "exprString.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class expressionToFace Declaration
\*---------------------------------------------------------------------------*/

class expressionToFace
:
    public topoSetSource
{

    //- Add usage string
    static addToUsageTable usage_;

    //- The logical expression to use
    exprString expression_;

    //- dictionary with additional data
    autoPtr<dictionary> dict_;

    // Private Member Functions

    //- Depending on face to cell option add to or delete from cellSet.
    void combine(topoSet& set, const bool add) const;

public:

    //- Runtime type information
    TypeName("expressionToFace");

    // Constructors

        //- Construct from components
        expressionToFace
        (
            const polyMesh& mesh,
            const exprString& expression
        );

        //- Construct from dictionary
        expressionToFace
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        expressionToFace
        (
            const polyMesh& mesh,
            Istream&
        );


    // Destructor

        virtual ~expressionToFace();


    // Member Functions

        virtual void applyToSet
        (
            const topoSetSource::setAction action,
            topoSet&
        ) const;

        virtual sourceType setType() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
