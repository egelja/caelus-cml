/*---------------------------------------------------------------------------*\
Copyright (C) 2017 Applied CCM
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

    OSPRE flux limiter.

References

    [1] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

    [2] "A unified approach to the design and application of bounded 
         higher-order convection schemes", N.P., Waterson, H. Deconinck,
         VKI report 1995-1, 1995

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef OSPREFluxLimiter_HPP_
#define OSPREFluxLimiter_HPP_

#define makeOSPREFluxLimiter(classType)                                        \
template<class Type>                                                           \
CML::scalar                                                                    \
CML::classType##OSPRE<Type>::fluxLimiter(CML::scalar const r) const            \
{                                                                              \
    return CML::scalar                                                         \
    (                                                                          \
        scalar(1.5)*r*(r+scalar(1.0))/(r*(r+1)+scalar(1.0))                    \
    );                                                                         \
}                                                                              
#endif
