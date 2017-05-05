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

#include "kinematicParcelInjectionData.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const CML::point& CML::kinematicParcelInjectionData::x() const
{
    return x_;
}


inline const CML::vector& CML::kinematicParcelInjectionData::U() const
{
    return U_;
}


inline CML::scalar CML::kinematicParcelInjectionData::d() const
{
    return d_;
}


inline CML::scalar CML::kinematicParcelInjectionData::rho() const
{
    return rho_;
}


inline CML::scalar CML::kinematicParcelInjectionData::mDot() const
{
    return mDot_;
}


inline CML::point& CML::kinematicParcelInjectionData::x()
{
    return x_;
}


inline CML::vector& CML::kinematicParcelInjectionData::U()
{
    return U_;
}


inline CML::scalar& CML::kinematicParcelInjectionData::d()
{
    return d_;
}


inline CML::scalar& CML::kinematicParcelInjectionData::rho()
{
    return rho_;
}


inline CML::scalar& CML::kinematicParcelInjectionData::mDot()
{
    return mDot_;
}


// ************************************************************************* //
