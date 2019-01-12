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

Contributors/Copyright:
    2009, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "rotateSearchableSurface.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "transform.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(rotateSearchableSurface, 0);
addToRunTimeSelectionTable(searchableSurface, rotateSearchableSurface, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::rotateSearchableSurface::rotateSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    transformationSearchableSurface(io,dict)
{
    vector from(dict.lookup("rotateFrom"));
    vector to(dict.lookup("rotateTo"));

    if(mag(from)<SMALL || mag(to)<SMALL) {
        FatalErrorInFunction
            << "Vector " << from << " or " << to << " close to zero"
            << endl
            << abort(FatalError);
    }

    from/=mag(from);
    to/=mag(to);

    rotation_ = rotationTensor(from,to);
    backRotation_ = rotationTensor(to,from);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::rotateSearchableSurface::~rotateSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::point CML::rotateSearchableSurface::transform(const point &p) const
{
    return CML::transform(rotation_,p);
}

CML::point CML::rotateSearchableSurface::inverseTransform(const point &p) const
{
    return CML::transform(backRotation_,p);
}

void CML::rotateSearchableSurface::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& normal
) const
{
    vectorField iNormal;

    transformationSearchableSurface::getNormal
        (
            info,
            iNormal
        );

    normal.setSize(iNormal.size());

    forAll(normal,i) {
        normal[i]=transform(iNormal[i]);
    }
}

void CML::rotateSearchableSurface::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    delegate().boundingSpheres(
        centres,
        radiusSqr
    );
    forAll(centres,i) {
        centres[i]=CML::transform(rotation_,centres[i]);
    }
}

// ************************************************************************* //
