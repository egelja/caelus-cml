/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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

Class
    BJLimiter

Description
    Barth-Jespersen multidimensional limiter

SourceFiles


References

    [1] “The Design and Application of Upwind Schemes on Unstructured Meshes,” 
         Barth, T. J. and Jespersen, D. C., AIAA paper 89-0366, Jan. 1989.

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef BJLimiter_HPP_
#define BJLimiter_HPP_


namespace CML
{

class BJLimiter
{

public:

    BJLimiter()
    {}

    inline scalar limiter
    (
        scalar const cellVolume,
        scalar const deltaOneMax,
        scalar const deltaOneMin,
        scalar deltaTwo
    )
    {
        deltaTwo = stabilise(deltaTwo,VSMALL);
        return min(max(max(deltaOneMax/deltaTwo,0),
                   max(deltaOneMin/deltaTwo,0)),1);
    }
	
    inline scalar limiter
    (
        scalar const cellVolume,
        scalar const deltaOneMax,
        scalar const deltaOneMin,
        vector const gradPhiP,
        vector const gradPhiN,
        vector const& d
    )
    {
        return limiter(cellVolume,deltaOneMax,deltaOneMin, d & gradPhiP);
    }
	
    inline vector limiter
    (
        scalar const cellVolume,
        vector const deltaOneMax,
        vector const deltaOneMin,
        tensor const gradPhiP,
        tensor const gradPhiN,
        vector const& d
    )
    {
        vector const deltaTwo = d & gradPhiP;
	return vector
        (
            limiter(cellVolume,deltaOneMax[0],deltaOneMin[0],deltaTwo[0]),
            limiter(cellVolume,deltaOneMax[1],deltaOneMin[1],deltaTwo[1]),
            limiter(cellVolume,deltaOneMax[2],deltaOneMin[2],deltaTwo[2])
	);
    }
};

}

#endif

