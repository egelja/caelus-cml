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

Contributors/Copyright:
    2009, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "searchableSphereWithFix.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(searchableSphereWithFix, 0);
addToRunTimeSelectionTable(searchableSurface, searchableSphereWithFix, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::searchableSphereWithFix::searchableSphereWithFix
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSphere(
        io,
        dict
    ),
    centre_(dict.lookup("centre")),
    radius_(readScalar(dict.lookup("radius")))
{
    WarningInFunction
        << "This class is only a workaround for the original"
        << " searchableSphere-implementation which cause a division "
        << "by zero and is present in OF up to 2.2. Should not be used "
        << "for versions where this is fixed"
        << endl;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::searchableSphereWithFix::~searchableSphereWithFix()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::pointIndexHit CML::searchableSphereWithFix::findNearest
(
    const point& sample,
    const scalar nearestDistSqr
) const
{
    pointIndexHit info(false, sample, -1);

    const vector n(sample-centre_);
    scalar magN = mag(n);

    if (nearestDistSqr > sqr(magN-radius_))
    {
        if (magN < ROOTVSMALL)
        {
            //            info.rawPoint() = centre_ + vector(1,0,0)/magN*radius_;
            info.rawPoint() = centre_ + vector(1,0,0)/ROOTVSMALL*radius_;
        }
        else
        {
            info.rawPoint() = centre_ + n/magN*radius_;
        }
        info.setHit();
        info.setIndex(0);
    }

    return info;
}

void CML::searchableSphereWithFix::findNearest
(
    const pointField& samples,
    const scalarField& nearestDistSqr,
    List<pointIndexHit>& info
) const
{
    info.setSize(samples.size());

    forAll(samples, i)
    {
        info[i] = findNearest(samples[i], nearestDistSqr[i]);
    }
}

// ************************************************************************* //
