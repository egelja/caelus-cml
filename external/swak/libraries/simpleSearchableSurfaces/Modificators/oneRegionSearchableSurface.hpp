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
    CML::oneRegionSearchableSurface

Description
    Makes the whole surface one region

SourceFiles
    oneRegionSearchableSurface.cpp

Contributors/Copyright:
    2009, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef oneRegionSearchableSurface_H
#define oneRegionSearchableSurface_H

#include "wrapperSearchableSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class oneRegionSearchableSurface Declaration
\*---------------------------------------------------------------------------*/

class oneRegionSearchableSurface
:
    public wrapperSearchableSurface
{
private:

    wordList name_;

        //- Disallow default bitwise copy construct
        oneRegionSearchableSurface(const oneRegionSearchableSurface&);

        //- Disallow default bitwise assignment
        void operator=(const oneRegionSearchableSurface&);

public:

    //- Runtime type information
    TypeName("oneRegion");


    // Constructors

        //- Construct from dictionary (used by searchableSurface)
        oneRegionSearchableSurface
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~oneRegionSearchableSurface();


    // Member Functions

        virtual label size() const { return 1; }

        virtual tmp<pointField> coordinates() const;

        virtual const wordList& regions() const;


            //- From a set of points and indices get the region
            virtual void getRegion
            (
                const List<pointIndexHit>&,
                labelList& region
            ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
