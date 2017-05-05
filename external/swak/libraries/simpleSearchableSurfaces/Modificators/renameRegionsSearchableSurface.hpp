/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright held by original author
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
    CML::renameRegionsSearchableSurface

Description
    Rename the regions

SourceFiles
    renameRegionsSearchableSurface.cpp

Contributors/Copyright:
    2009, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef renameRegionsSearchableSurface_H
#define renameRegionsSearchableSurface_H

#include "wrapperSearchableSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class renameRegionsSearchableSurface Declaration
\*---------------------------------------------------------------------------*/

class renameRegionsSearchableSurface
:
    public wrapperSearchableSurface
{
private:

    wordList regions_;

        //- Disallow default bitwise copy construct
        renameRegionsSearchableSurface(const renameRegionsSearchableSurface&);

        //- Disallow default bitwise assignment
        void operator=(const renameRegionsSearchableSurface&);

public:

    //- Runtime type information
    TypeName("renameRegions");


    // Constructors

        //- Construct from dictionary (used by searchableSurface)
        renameRegionsSearchableSurface
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~renameRegionsSearchableSurface();


    // Member Functions

        virtual label size() const { return delegate().size(); }

        virtual tmp<pointField> coordinates() const
        {
            return delegate().coordinates();
        }

        virtual const wordList& regions() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
