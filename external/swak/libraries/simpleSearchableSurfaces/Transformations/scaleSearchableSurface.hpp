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
    CML::scaleSearchableSurface

Description
    Searching on scaled surface

SourceFiles
    scaleSearchableSurface.cpp

Contributors/Copyright:
    2009, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef scaleSearchableSurface_H
#define scaleSearchableSurface_H

#include "transformationSearchableSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class scaleSearchableSurface Declaration
\*---------------------------------------------------------------------------*/

class scaleSearchableSurface
:
    public transformationSearchableSurface
{
private:

    // Private Member Data

    vector scale_;


        //- Disallow default bitwise copy construct
        scaleSearchableSurface(const scaleSearchableSurface&);

        //- Disallow default bitwise assignment
        void operator=(const scaleSearchableSurface&);

protected:

    // Do the transformation for a point
    virtual point transform(const point &) const;

    // Do the inverse transformation for a point
    virtual point inverseTransform(const point &) const;

public:

    //- Runtime type information
    TypeName("scale");


    // Constructors

        //- Construct from components
        scaleSearchableSurface(const IOobject& io, const treeBoundBox& bb);

        //- Construct from dictionary (used by transformationSearchableSurface)
        scaleSearchableSurface
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~scaleSearchableSurface();

    //- From a set of points and indices get the normal
    virtual void getNormal
    (
        const List<pointIndexHit>&,
        vectorField& normal
    ) const;

    virtual void boundingSpheres
    (
        pointField& centres,
        scalarField& radiusSqr
    ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
