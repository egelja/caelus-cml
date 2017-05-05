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

#include "basicSource.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::basicSource::writeHeader(Ostream& os) const
{
    os  << indent << name_ << nl
        << indent << token::BEGIN_BLOCK << incrIndent << nl;
}


void CML::basicSource::writeFooter(Ostream& os) const
{
    os  << decrIndent << indent << token::END_BLOCK << endl;
}


void CML::basicSource::writeData(Ostream& os) const
{
    os.writeKeyword("active") << active_ << token::END_STATEMENT << nl;
    os.writeKeyword("timeStart") << timeStart_ << token::END_STATEMENT << nl;
    os.writeKeyword("duration") << duration_ << token::END_STATEMENT << nl;
    os.writeKeyword("selectionMode")
        << selectionModeTypeNames_[selectionMode_] << nl;

    switch (selectionMode_)
    {
        case smPoints:
        {
            os.writeKeyword("points") << points_
                << token::END_STATEMENT << nl;
            break;
        }
        case smCellSet:
        {
            os.writeKeyword("cellSet") << cellSetName_
                << token::END_STATEMENT << nl;
            break;
        }
        case smCellZone:
        {
            os.writeKeyword("cellZone") << cellSetName_
                << token::END_STATEMENT << nl;
            break;
        }
        case smAll:
        {
            break;
        }
        default:
        {
            FatalErrorIn("basicSource::writeData(Ostream&) const")
                << "Unknown selectionMode "
                << selectionMode_
                << abort(FatalError);
        }
    }
}


bool CML::basicSource::read(const dictionary& dict)
{
    active_ = readBool(dict.lookup("active"));
    timeStart_ = readScalar(dict.lookup("timeStart"));
    duration_  = readScalar(dict.lookup("duration"));

    coeffs_ = dict.subDict(type() + "Coeffs");

    return true;
}


// ************************************************************************* //
