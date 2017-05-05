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

inline CML::scalar CML::Ar::rho(scalar p, scalar T) const
{
    return rho_.f(p, T);
}


inline CML::scalar CML::Ar::pv(scalar p, scalar T) const
{
    return pv_.f(p, T);
}


inline CML::scalar CML::Ar::hl(scalar p, scalar T) const
{
    return hl_.f(p, T);
}


inline CML::scalar CML::Ar::Cp(scalar p, scalar T) const
{
    return Cp_.f(p, T);
}


inline CML::scalar CML::Ar::h(scalar p, scalar T) const
{
    return h_.f(p, T);
}


inline CML::scalar CML::Ar::Cpg(scalar p, scalar T) const
{
    return Cpg_.f(p, T);
}


inline CML::scalar CML::Ar::B(scalar p, scalar T) const
{
    return B_.f(p, T);
}


inline CML::scalar CML::Ar::mu(scalar p, scalar T) const
{
    return mu_.f(p, T);
}


inline CML::scalar CML::Ar::mug(scalar p, scalar T) const
{
    return mug_.f(p, T);
}


inline CML::scalar CML::Ar::K(scalar p, scalar T) const
{
    return K_.f(p, T);
}


inline CML::scalar CML::Ar::Kg(scalar p, scalar T) const
{
    return Kg_.f(p, T);
}


inline CML::scalar CML::Ar::sigma(scalar p, scalar T) const
{
    return sigma_.f(p, T);
}


inline CML::scalar CML::Ar::D(scalar p, scalar T) const
{
    return D_.f(p, T);
}


inline CML::scalar CML::Ar::D(scalar p, scalar T, scalar Wb) const
{
    return D_.f(p, T, Wb);
}


// ************************************************************************* //
