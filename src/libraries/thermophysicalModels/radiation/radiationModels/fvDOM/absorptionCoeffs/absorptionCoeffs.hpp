/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::absorptionCoeffs

Description
    Absorption coefficients class used in greyMeanAbsorptionEmission and
    wideBandAbsorptionEmission

SourceFiles
    absorptionCoeffs.cpp

\*---------------------------------------------------------------------------*/

#ifndef absorptionCoeffs_HPP
#define absorptionCoeffs_HPP

#include "List.hpp"
#include "IOstreams.hpp"
#include "IOdictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                      Class absorptionCoeffs Declaration
\*---------------------------------------------------------------------------*/

class absorptionCoeffs
{
public:

    static const int nCoeffs_ = 6;
    typedef FixedList<scalar, nCoeffs_> coeffArray;


private:

    // Temperature limits of applicability for functions

    scalar Tcommon_;

    scalar Tlow_;

    scalar Thigh_;


    // Polynomial using inverse temperatures
    bool invTemp_;

    coeffArray highACoeffs_;
    coeffArray lowACoeffs_;


    //- Check given temperature is within the range of the fitted coeffs
    void checkT(const scalar T) const;


public:


    // Null constructor
    absorptionCoeffs()
    {}


    //- Destructor
    ~absorptionCoeffs()
    {}


    // Member functions

    //- Return the coefficients corresponding to the given temperature
    const coeffArray& coeffs(const scalar T) const;

    // Initialise from a dictionary
    void initialise(const dictionary&);


    // Access Functions

    inline bool invTemp() const
    {
        return  invTemp_;
    }

    inline scalar Tcommon() const
    {
        return  Tcommon_;
    }

    inline scalar Tlow() const
    {
        return  Tlow_;
    }

    inline scalar Thigh() const
    {
        return  Thigh_;
    }

    inline const coeffArray& highACoeffs() const
    {
        return  highACoeffs_;
    }

    inline const coeffArray& lowACoeffs() const
    {
        return  lowACoeffs_;
    }

};


} // End namespace CML
} // End namespace radiation


#endif
