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

    A TVD property staisfying linear upwind scheme based on the UMIST 
    limiter to prevent unwanted oscillations. Implementation suitable for
    scalar fields. The limiter corresponds to the symmetric QUICK limiter.

References

    [1] "A Pressure-Velocity Solution Strategy for Compressible Flow and its 
         Application to Shock-Boundary Layer interaction using 2nd-Moment 
         Turbulence Closure", F.S, Lien, M.A. Leshziner, J. of Fluids 
         Eng.-Tans. of ASME, 115, pp. 717-725, 1993

    [2] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef umistFluxLimiter_HPP_
#define umistFluxLimiter_HPP_

#define makeUMISTFluxLimiter(classType)                                        \
template<class Type>                                                           \
CML::scalar                                                                    \
CML::classType##UMIST<Type>::fluxLimiter(CML::scalar const r) const            \
{                                                                              \
    return CML::scalar                                                         \
    (                                                                          \
        max                                                                    \
        (                                                                      \
            0,                                                                 \
            min                                                                \
            (                                                                  \
                min                                                            \
                (                                                              \
                    min                                                        \
                    (                                                          \
                        2*r,                                                   \
                        0.25+0.75*r                                            \
                    ),                                                         \
                    0.75 + 0.25*r                                              \
                ),                                                             \
                2                                                              \
            )                                                                  \
        )                                                                      \
    );                                                                         \
}                                                                              
#endif
