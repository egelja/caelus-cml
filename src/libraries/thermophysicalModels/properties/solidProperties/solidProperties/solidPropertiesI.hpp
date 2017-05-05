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

#include "specie.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline CML::scalar CML::solidProperties::rho() const
{
    return rho_;
}


inline CML::scalar CML::solidProperties::Cp() const
{
    return Cp_;
}


inline CML::scalar CML::solidProperties::K() const
{
    return K_;
}


inline CML::scalar CML::solidProperties::Hf() const
{
    return Hf_;
}


inline CML::scalar CML::solidProperties::H(const scalar T) const
{
    return Hs(T) + Hf_;
}


inline CML::scalar CML::solidProperties::Hs(const scalar T) const
{
    return Cp_*(T - specie::Tstd);
}


inline CML::scalar CML::solidProperties::emissivity() const
{
    return emissivity_;
}


// ************************************************************************* //
