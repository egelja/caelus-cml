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
    CML::invertSearchableSurface

Description
    Invert the volume of the wrapped surface

SourceFiles
    invertSearchableSurface.ccp

Contributors/Copyright:
    2009, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef invertSearchableSurface_H
#define invertSearchableSurface_H

#include "wrapperSearchableSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class invertSearchableSurface Declaration
\*---------------------------------------------------------------------------*/

class invertSearchableSurface
:
    public wrapperSearchableSurface
{
private:
       //- Disallow default bitwise copy construct
        invertSearchableSurface(const invertSearchableSurface&);

        //- Disallow default bitwise assignment
        void operator=(const invertSearchableSurface&);

public:

    //- Runtime type information
    TypeName("invert");


    // Constructors

        //- Construct from dictionary (used by searchableSurface)
        invertSearchableSurface
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~invertSearchableSurface();


    // Member Functions

    void getVolumeType
    (
        const pointField& points,
        List<volumeType>& volType
    ) const;

    //- From a set of points and indices get the normal
    void getNormal
    (
        const List<pointIndexHit>&,
        vectorField& normal
    ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
