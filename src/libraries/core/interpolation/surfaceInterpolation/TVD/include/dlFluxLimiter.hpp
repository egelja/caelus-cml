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

    A TVD property staisfying linear upwind scheme. The limiter is 
    based on Venkatakrishana limiter. Largely eliminates unsteady 
    switching of the limiter on unstructured meshes and promotes 
    convergence.

References

    [1] “On the Accuracy of Limiters and Convergence to Steady-State Solutions,”
         Venkatakrishnan, V., AIAA paper 93-0880, Jan. 1993.

    [2] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef dlFluxLimiter_HPP_
#define dlFluxLimiter_HPP_

#define makeDLFluxLimiter(classType)                                           \
template<class Type>                                                           \
CML::scalar                                                                    \
CML::classType##DL<Type>::fluxLimiter(CML::scalar const r) const               \
{                                                                              \
    return CML::scalar                                                         \
    (                                                                          \
        max(min((r+1)/2*r*(r+2)/(r*(r+1)+2+VSMALL),2), 0)                      \
    );                                                                         \
}                                                                              
#endif
