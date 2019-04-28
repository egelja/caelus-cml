/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "findCellParticle.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::findCellParticle::findCellParticle
(
    const polyMesh& mesh,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti,
    const point& end,
    const label data
)
:
    particle(mesh, coordinates, celli, tetFacei, tetPti),
    start_(position()),
    end_(end),
    data_(data)
{}


CML::findCellParticle::findCellParticle
(
    const polyMesh& mesh,
    const vector& position,
    const label celli,
    const point& end,
    const label data
)
:
    particle(mesh, position, celli),
    start_(this->position()),
    end_(end),
    data_(data)
{}


CML::findCellParticle::findCellParticle
(
    const polyMesh& mesh,
    Istream& is,
    bool readFields
)
:
    particle(mesh, is, readFields)
{
    if (readFields)
    {
        if (is.format() == IOstream::ASCII)
        {
            is >> start_ >> end_;
            data_ = readLabel(is);
        }
        else
        {
            is.read
            (
                reinterpret_cast<char*>(&start_),
                sizeof(start_) + sizeof(end_) + sizeof(data_)
            );
        }
    }

    is.check(FUNCTION_NAME);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::findCellParticle::move
(
    Cloud<findCellParticle>& cloud,
    trackingData& td,
    const scalar maxTrackLen
)
{
    td.switchProcessor = false;
    td.keepParticle = true;

    while (td.keepParticle && !td.switchProcessor && stepFraction() < 1)
    {
        const scalar f = 1 - stepFraction();
        trackToAndHitFace(f*(end_ - start_), f, cloud, td);
    }

    if (stepFraction() == 1 || !td.keepParticle)
    {
        // Hit endpoint or patch. If patch hit could do fancy stuff but just
        // to use the patch point is good enough for now.
        td.cellToData()[cell()].append(data());
        td.cellToEnd()[cell()].append(position());
    }

    return td.keepParticle;
}


bool CML::findCellParticle::hitPatch(Cloud<findCellParticle>&, trackingData&)
{
    return false;
}


void CML::findCellParticle::hitWedgePatch
(
    Cloud<findCellParticle>&,
    trackingData& td
)
{
    // Remove particle
    td.keepParticle = false;
}


//void CML::findCellParticle::hitSymmetryPlanePatch
//(
//    Cloud<findCellParticle>&,
//    trackingData& td
//)
//{
    // Remove particle
//    td.keepParticle = false;
//}


void CML::findCellParticle::hitSymmetryPatch
(
    Cloud<findCellParticle>&,
    trackingData& td
)
{
    // Remove particle
    td.keepParticle = false;
}


void CML::findCellParticle::hitCyclicPatch
(
    Cloud<findCellParticle>&,
    trackingData& td
)
{
    // Remove particle
    td.keepParticle = false;
}


void CML::findCellParticle::hitCyclicAMIPatch
(
    Cloud<findCellParticle>&,
    trackingData& td,
    const vector&
)
{
    // Remove particle
    td.keepParticle = false;
}


void CML::findCellParticle::hitCyclicACMIPatch
(
    Cloud<findCellParticle>&,
    trackingData& td,
    const vector&
)
{
    // Remove particle
    td.keepParticle = false;
}


void CML::findCellParticle::hitProcessorPatch
(
    Cloud<findCellParticle>&,
    trackingData& td
)
{
    // Remove particle
    td.switchProcessor = true;
}


void CML::findCellParticle::hitWallPatch
(
    Cloud<findCellParticle>&,
    trackingData& td
)
{
    // Remove particle
    td.keepParticle = false;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const findCellParticle& p)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const particle&>(p)
            << token::SPACE << p.start_
            << token::SPACE << p.end_
            << token::SPACE << p.data_;
    }
    else
    {
        os  << static_cast<const particle&>(p);
        os.write
        (
            reinterpret_cast<const char*>(&p.start_),
            sizeof(p.start_) + sizeof(p.end_) + sizeof(p.data_)
        );
    }

    os.check(FUNCTION_NAME);
    return os;
}


// ************************************************************************* //
