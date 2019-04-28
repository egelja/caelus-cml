/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on OpenFOAM.

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
    CML::searchableCylinderWithDirections

Description
    Searching on sphere

SourceFiles
    searchableCylinderWithDirections.cpp

Contributors/Copyright:
    2009, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef searchableCylinderWithDirections_H
#define searchableCylinderWithDirections_H

#include "searchableCylinder.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class searchableCylinderWithDirections Declaration
\*---------------------------------------------------------------------------*/

class searchableCylinderWithDirections
:
    public searchableCylinder
{
private:

   //- Names of regions
    mutable wordList regions_;

    //- 'left' point - duplicate from parent class as it is private
    const point point1_;

    //- 'right' point - duplicate from parent class as it is private
    const point point2_;

    //- Disallow default bitwise copy construct
    searchableCylinderWithDirections(const searchableCylinderWithDirections&);

    //- Disallow default bitwise assignment
    void operator=(const searchableCylinderWithDirections&);

    void recalcIndex(
         List<pointIndexHit>& info
    ) const;

public:

    //- Runtime type information
    TypeName("searchableCylinderWithDirections");


    // Constructors

       //- Construct from dictionary (used by searchableSurface)
        searchableCylinderWithDirections
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~searchableCylinderWithDirections();


    // Member Functions

    virtual tmp<pointField>  coordinates() const;

    virtual const wordList& regions() const;

    virtual label size() const { return 3; }

    //- From a set of points and indices get the region
    virtual void getRegion
    (
        const List<pointIndexHit>&,
        labelList& region
     ) const;

    // regIOobject implementation

        // Multiple point queries.

    virtual void findNearest
    (
        const pointField& sample,
        const scalarField& nearestDistSqr,
        List<pointIndexHit>&
    ) const;

    virtual void findLine
    (
        const pointField& start,
        const pointField& end,
        List<pointIndexHit>&
    ) const;

    virtual void findLineAny
    (
        const pointField& start,
        const pointField& end,
        List<pointIndexHit>&
    ) const;

    //- Get all intersections in order from start to end.
    virtual void findLineAll
    (
        const pointField& start,
        const pointField& end,
        List<List<pointIndexHit> >&
    ) const;

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
