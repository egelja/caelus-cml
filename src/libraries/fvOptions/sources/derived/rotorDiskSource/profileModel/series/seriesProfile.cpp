/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "seriesProfile.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "IFstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(seriesProfile, 0);
    addToRunTimeSelectionTable(profileModel, seriesProfile, dictionary);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

CML::scalar CML::seriesProfile::evaluateDrag
(
    const scalar& xIn,
    const List<scalar>& values
) const
{
    scalar result = 0.0;

    forAll(values, i)
    {
        result += values[i]*cos(i*xIn);
    }

    return result;
}


CML::scalar CML::seriesProfile::evaluateLift
(
    const scalar& xIn,
    const List<scalar>& values
) const
{
    scalar result = 0.0;

    forAll(values, i)
    {
        // note: first contribution always zero since sin(0) = 0, but
        // keep zero base to be consitent with drag coeffs
        result += values[i]*sin(i*xIn);
    }

    return result;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::seriesProfile::seriesProfile
(
    const dictionary& dict,
    const word& modelName
)
:
    profileModel(dict, modelName),
    CdCoeffs_(),
    ClCoeffs_()
{
    if (readFromFile())
    {
        IFstream is(fName_);
        is  >> CdCoeffs_ >> ClCoeffs_;
    }
    else
    {
        dict.lookup("CdCoeffs") >> CdCoeffs_;
        dict.lookup("ClCoeffs") >> ClCoeffs_;
    }


    if (CdCoeffs_.empty())
    {
        FatalErrorInFunction
            << "CdCoeffs must be specified" << exit(FatalError);
    }
    if (ClCoeffs_.empty())
    {
        FatalErrorInFunction
            << "ClCoeffs must be specified" << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::seriesProfile::Cdl(const scalar alpha, scalar& Cd, scalar& Cl) const
{
    Cd = evaluateDrag(alpha, CdCoeffs_);
    Cl = evaluateLift(alpha, ClCoeffs_);
}


// ************************************************************************* //
