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

    A TVD property staisfying linear upwind scheme.

    MG is short for Michalak-Olivier-Gooch Limiter.

References

    [1] "Accuracy Preserving Limiter for the High-Order Accurate Solution 
         of the Euler Equations", C., Michalak, and C. Olivier-Gooch",
         J. of Compt. Phys. 228 (23), 00/ 8693-8711, Elsevier, 2009

    [2] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef mgFluxLimiter_HPP_
#define mgFluxLimiter_HPP_

#define makeMGFluxLimiter(classType)                                           \
template<class Type>                                                           \
CML::scalar                                                                    \
CML::classType##MG<Type>::fluxLimiter(CML::scalar const r) const               \
{                                                                              \
    CML::scalar l = 0;                                                         \
    if (r <= 1.8)                                                              \
    {                                                                          \
        l = max(r*(1.59-r*r*4/27), 0);                                         \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        l = 2.0;                                                               \
    }                                                                          \
    return l;                                                                  \
}                                                                              
#endif
