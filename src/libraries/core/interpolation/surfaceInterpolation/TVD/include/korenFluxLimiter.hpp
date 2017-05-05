/*---------------------------------------------------------------------------*\
Copyright (C) 2015 - 2016 Applied CCM
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

Description

    Koren flux limiter.

References

    [1] "Upwind Discretization of the Steady Navier-Stokes Equations",
         B. Koren, International Journal for Numerical Methods in Fluids,
         Vol 11, pp 99-117, 1990

    [2] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef korenFluxLimiter_HPP_
#define korenFluxLimiter_HPP_

#define makeKorenFluxLimiter(classType)                                        \
template<class Type>                                                           \
CML::scalar                                                                    \
CML::classType##Koren<Type>::fluxLimiter(CML::scalar const r) const            \
{                                                                              \
    return CML::scalar                                                         \
    (                                                                          \
        max(min(min(2*r,(2+r)/3),2),0)                                         \
    );                                                                         \
}                                                                              
#endif
