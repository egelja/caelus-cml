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

#include "scaleSearchableSurface.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(scaleSearchableSurface, 0);
addToRunTimeSelectionTable(searchableSurface, scaleSearchableSurface, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::scaleSearchableSurface::scaleSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    transformationSearchableSurface(io,dict),
    scale_(dict.lookup("scale"))
{
    scalar mini=min(mag(scale_.x()),min(mag(scale_.y()),mag(scale_.z())));
    if(mini<SMALL) {
        FatalErrorInFunction
            << "Scaling vector " << scale_ << " has a component that is almost zero\n"
            << " -> Division by zero while inverse operation"
            <<endl
            <<abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::scaleSearchableSurface::~scaleSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::point CML::scaleSearchableSurface::transform(const point &p) const
{
    return point
        (
            p.x()*scale_.x(),
            p.y()*scale_.y(),
            p.z()*scale_.z()
        );
}

CML::point CML::scaleSearchableSurface::inverseTransform(const point &p) const
{
    return point
        (
            p.x()/scale_.x(),
            p.y()/scale_.y(),
            p.z()/scale_.z()
        );
}

void CML::scaleSearchableSurface::getNormal
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
        normal[i]=inverseTransform(iNormal[i]);
        scalar len=mag(normal[i]);
        if(len>SMALL) {
            normal[i]/=len;
        }
    }
}

void CML::scaleSearchableSurface::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    delegate().boundingSpheres(
        centres,
        radiusSqr
    );
    scalar maxScale=mag(scale_.x());
    if(mag(scale_.y())>maxScale) {
        maxScale=mag(scale_.y());
    }
    if(mag(scale_.z())>maxScale) {
        maxScale=mag(scale_.z());
    }
    forAll(centres,i) {
        radiusSqr[i]=radiusSqr[i]*maxScale*maxScale;
    }
}

// ************************************************************************* //
