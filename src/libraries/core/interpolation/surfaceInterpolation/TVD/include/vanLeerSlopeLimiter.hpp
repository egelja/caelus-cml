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

    VanLeer slope limiter.

References

    [1] "Towards the ultimate conservative difference scheme II. Monotonicity 
         and Conservation Combined in Second Order Scheme", B. Van Leer,
         J. Comp. Phys. 14 (4), pp 361-370, 1974

    [2] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef vanLeerSlopeLimiter_HPP_
#define vanLeerSlopeLimiter_HPP_

#define makeVanLeerSlopeLimiter(classType)                                     \
template<class Type>                                                           \
CML::scalar                                                                    \
CML::classType##VanLeer<Type>::slopeLimiter(CML::scalar const r) const         \
{                                                                              \
    return CML::scalar                                                         \
    (                                                                          \
        max(0,4*r/((r+1+VSMALL)*(r+1+VSMALL)))                                 \
    );                                                                         \
}                                                                              
#endif
