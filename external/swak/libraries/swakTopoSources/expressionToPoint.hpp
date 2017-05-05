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
    CML::expressionToPoint

Description
    A topoSetSource to select the point from a logical expression

SourceFiles
    expressionToPoint.cpp

Contributors/Copyright:
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef expressionToPoint_H
#define expressionToPoint_H

#include "topoSetSource.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "exprString.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class expressionToPoint Declaration
\*---------------------------------------------------------------------------*/

class expressionToPoint
:
    public topoSetSource
{
    // Private data

        //- Add usage string
        static addToUsageTable usage_;

        //- The logical expression to use
        exprString expression_;

        //- dictionary with additional data
        autoPtr<dictionary> dict_;

        void combine(topoSet& set, const bool add) const;

public:

    //- Runtime type information
    TypeName("expressionToPoint");

    // Constructors

        //- Construct from components
        expressionToPoint
        (
            const polyMesh& mesh,
            const exprString& expression
        );

        //- Construct from dictionary
        expressionToPoint
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        expressionToPoint
        (
            const polyMesh& mesh,
            Istream&
        );


    // Destructor

        virtual ~expressionToPoint();


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
