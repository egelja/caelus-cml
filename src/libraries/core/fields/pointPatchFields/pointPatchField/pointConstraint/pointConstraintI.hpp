/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::pointConstraint::pointConstraint()
:
    Tuple2<label, vector>(0, vector::zero)
{}


inline CML::pointConstraint::pointConstraint(Istream& is)
:
    Tuple2<label, vector>(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::pointConstraint::applyConstraint(const vector& cd)
{
    if (first() == 0)
    {
        first() = 1;
        second() = cd;
    }
    else if (first() == 1)
    {
        vector planeNormal = cd ^ second();
        scalar magPlaneNormal = mag(planeNormal);

        if (magPlaneNormal > 1e-3)
        {
            first() = 2;
            second() = planeNormal/magPlaneNormal;
        }
    }
    else if (first() == 2)
    {
        if (mag(cd & second()) > 1e-3)
        {
            first() = 3;
            second() = vector::zero;
        }
    }
}


void CML::pointConstraint::combine(const pointConstraint& pc)
{
    if (first() == 0)
    {
        operator=(pc);
    }
    else if (first() == 1)
    {
        // Save single normal
        vector n = second();
        // Apply to supplied point constaint
        operator=(pc);
        applyConstraint(n);
    }
    else if (first() == 2)
    {
        if (pc.first() == 0)
        {}
        else if (pc.first() == 1)
        {
            applyConstraint(pc.second());
        }
        else if (pc.first() == 2)
        {
            // Both constrained to line. Same (+-)direction?
            if (mag(second() & pc.second()) <= (1.0-1e-3))
            {
                // Different directions
                first() = 3;
                second() = vector::zero;
            }
        }
        else
        {
            first() = 3;
            second() = vector::zero;
        }
    }
}


CML::tensor CML::pointConstraint::constraintTransformation() const
{
    if (first() == 0)
    {
        return I;
    }
    else if (first() == 1)
    {
        return I - sqr(second());
    }
    else if (first() == 2)
    {
        return sqr(second());
    }
    else
    {
        return tensor::zero;
    }
}


// ************************************************************************* //
