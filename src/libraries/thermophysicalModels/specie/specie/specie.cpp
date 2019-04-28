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

\*---------------------------------------------------------------------------*/

#include "specie.hpp"
#include "IOstreams.hpp"
#include "constants.hpp"

/* * * * * * * * * * * * * * * public constants  * * * * * * * * * * * * * * */

namespace CML
{
    defineTypeNameAndDebug(specie, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::specie::specie(const dictionary& dict)
:
    name_(dict.dictName()),
    Y_(dict.subDict("specie").lookupOrDefault("massFraction", 1.0)),
    molWeight_(readScalar(dict.subDict("specie").lookup("molWeight")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::specie::write(Ostream& os) const
{
    dictionary dict("specie");
    if (Y_ != 1)
    {
        dict.add("massFraction", Y_);
    }
    dict.add("molWeight", molWeight_);
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const specie& st)
{
    st.write(os);
    os.check("Ostream& operator<<(Ostream& os, const specie& st)");
    return os;
}
