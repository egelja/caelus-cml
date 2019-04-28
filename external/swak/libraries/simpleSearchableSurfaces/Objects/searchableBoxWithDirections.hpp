/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
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
    CML::searchableBoxWithDirections

Description
    Searching on sphere

SourceFiles
    searchableBoxWithDirections.cpp

Contributors/Copyright:
    2009, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef searchableBoxWithDirections_H
#define searchableBoxWithDirections_H

#include "searchableBox.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class searchableBoxWithDirections Declaration
\*---------------------------------------------------------------------------*/

class searchableBoxWithDirections
:
    public searchableBox
{
private:

   //- Names of regions
    mutable wordList regions_;

   //- Disallow default bitwise copy construct
    searchableBoxWithDirections(const searchableBoxWithDirections&);

    //- Disallow default bitwise assignment
    void operator=(const searchableBoxWithDirections&);

public:

    //- Runtime type information
    TypeName("searchableBoxWithDirections");


    // Constructors

       //- Construct from dictionary (used by searchableSurface)
        searchableBoxWithDirections
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~searchableBoxWithDirections();


    // Member Functions

    virtual const wordList& regions() const;

    //- From a set of points and indices get the region
    virtual void getRegion
    (
        const List<pointIndexHit>&,
        labelList& region
     ) const;

    // regIOobject implementation


     bool writeData(Ostream&) const
        {
            NotImplemented;
            return false;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
